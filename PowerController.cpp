#include "pch.h"
#include "PowerController.h"

/********************* PowerPort *********************/
PowerPort::PowerPort(DWORD dwPortNumber, CtrlCommand_CB callback, void* handle, BOOL bEnableLog)
	: m_dwPortNumber(dwPortNumber)
	, m_callback(callback)
	, m_handle(handle)
	, m_bEnableLog(bEnableLog)
	, m_bThreadRun(FALSE)
	, m_hSemaphore(INVALID_HANDLE_VALUE)
	, m_hScheduleThread(INVALID_HANDLE_VALUE)
	, m_bStart(FALSE)
{}

PowerPort::~PowerPort()
{
	ClearSchedule();
	ThreadClose();
}

void PowerPort::InitSchedule()
{
	m_hSemaphore = CreateSemaphore(NULL, 0, 1, NULL);
	m_hScheduleThread = CreateThread(NULL, 0, ScheduleThread, this, 0, 0);
}

void PowerPort::AddSchedule(CString& action, CString& value, int step)
{
	m_schedule.push_back(CommandModel(m_dwPortNumber, action, value, step));
}

void PowerPort::ClearSchedule()
{
	m_schedule.clear();
}

void PowerPort::StartSchedule()
{
	if (m_bStart == TRUE) { return; }
	m_bStart = TRUE;
	ReleaseSemaphore(m_hSemaphore, 1, NULL);
}

void PowerPort::ThreadClose()
{
	if (m_hScheduleThread == INVALID_HANDLE_VALUE) { return; }
	
	m_bThreadRun = FALSE;
	if (m_hSemaphore != INVALID_HANDLE_VALUE) { ReleaseSemaphore(m_hSemaphore, 1, NULL); }

	switch (WaitForSingleObject(m_hScheduleThread, 3000))
	{
	case WAIT_OBJECT_0:
		break;

	case WAIT_TIMEOUT:
		TerminateThread(m_hScheduleThread, WAIT_TIMEOUT);
		break;
	}

	CloseHandle(m_hScheduleThread);
	CloseHandle(m_hSemaphore);
	m_hSemaphore = m_hScheduleThread = INVALID_HANDLE_VALUE;
}

DWORD WINAPI PowerPort::ScheduleThread(void* data)
{
	PowerPort* port = (PowerPort*)data;

	port->m_bThreadRun = TRUE;
	while (port->m_bThreadRun)
	{
		WaitForSingleObject(port->m_hSemaphore, INFINITE);
		for (int i = 0; i < (port->m_schedule).size() && port->m_bThreadRun; i++)
		{
			if (port->m_callback != NULL)
			{
				switch ((port->m_schedule)[i].getCommandType())
				{
				case eCmdDelay:
					port->step = (port->m_schedule)[i].getStep();
					if (port->step < 4)
					{
						Sleep((port->m_schedule)[i].getDelay());
					}
					break;
				case eCmdISet:
					port->current = (port->m_schedule)[i].getCurrent();
					port->step = (port->m_schedule)[i].getStep();
					port->m_callback((port->m_schedule)[i].getCommand(), (port->m_schedule)[i].getCommandLength(), port->m_handle);
					break;
				case eCmdVset:
					port->m_callback((port->m_schedule)[i].getCommand(), (port->m_schedule)[i].getCommandLength(), port->m_handle);
					break;
				}
			}
		}
		port->m_bStart = FALSE;
		//port->step = 0;
	}

	return 0;
}

/*********************    PowerController    *********************/
PowerController::PowerController() : id(0) {}
PowerController::~PowerController() { Deinit(); }

void PowerController::Init(int id, int port_count, CallbackFunc cb, void* ctx)
{
	this->id = id;
	this->recv_thread = INVALID_HANDLE_VALUE;
	this->thread_run = FALSE;
	this->callbackfunc = cb;
	this->context = ctx;

	for (int i = 0; i < port_count; i++)
	{
		this->powerport.push_back(PowerPort(i + 1, Command_CB, this));
	}

	for (int i = 0; i < port_count; i++) { this->powerport[i].InitSchedule(); }	
}

void PowerController::Deinit()
{
	ResetSchedule();
	powerport.clear();
	ClosePort();
}

size_t PowerController::GetPortCount()
{
	return powerport.size();
}

BOOL PowerController::ConnectSerial(CString port, CString buadrate)
{
	CString pullpath(_T("\\\\.\\") + port);

	BOOL rst = OpenPort(pullpath);
	if (rst == FALSE) { return rst; }

	InitPort(_ttoi(buadrate), NOPARITY, 8, ONESTOPBIT, FLOWNONE);

	return rst;
}

BOOL PowerController::DisconnectSerial()
{
	ResetSchedule();
	return ClosePort();
}

int PowerController::GetID() { return id; }

void PowerController::SendCommand(int port_index, CString& action, CString& value)
{
	CommandModel commandModel = CommandModel(port_index + 1, action, value);

	switch (commandModel.getCommandType())
	{
	case eCmdISet:
		powerport[port_index].current = value;
		SendCommand((BYTE*)commandModel.getCommand(), commandModel.getCommandLength());
		break;
	case eCmdVset:
		SendCommand((BYTE*)commandModel.getCommand(), commandModel.getCommandLength());
		break;
	}
}

void PowerController::SendCommand(CString& command)
{
	int len = command.GetLength();
	char* cmd = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, command, -1, cmd, len + 1, NULL, NULL);

	SendCommand((BYTE*)cmd, len);
}

void PowerController::SendCommand(BYTE* buffer, int len)
{
	WriteSerial(buffer, len);
}

void PowerController::StartScheduler(int port_index)
{
	if (port_index < GetPortCount())
	{
		powerport[port_index].StartSchedule();
	}
}

BOOL PowerController::AddSchedule(int port_index, CString& action, CString& value, int step)
{
	if (port_index >= (int)GetPortCount()) { return FALSE; }

	powerport[port_index].AddSchedule(action, value, step);
}

void PowerController::ResetSchedule()
{
	for (size_t i = 0; i < GetPortCount(); i++)
	{
		powerport[i].ClearSchedule();
	}
}

BOOL PowerController::isScheduling(int port_index)
{
	BOOL rst = FALSE;
	if (port_index < GetPortCount())
	{
	   rst = powerport[port_index].isRun();
	}

	return rst;
}

void PowerController::ResetSchedule(int port_index)
{
	if (port_index < GetPortCount())
	{
		powerport[port_index].ClearSchedule();
	}
}

CString PowerController::GetCurrent(int port_index)
{
	CString c;
	if (port_index < GetPortCount())
	{
		c = powerport[port_index].GetCurrent();
	}

	return c;
}

void PowerController::Command_CB(char* buffer, int len, void* handle)
{
	PowerController* p = (PowerController*)handle;

	p->WriteSerial((BYTE*)buffer, len);
}

int PowerController::GetStep(int port_index)
{
	int step(0);
	if (port_index < GetPortCount())
	{
		step = powerport[port_index].GetStep();
	}

	return step;
}