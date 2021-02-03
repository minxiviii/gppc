#include "pch.h"
#include "PowerController.h"

/********************* PowerPort *********************/
PowerPort::PowerPort(uint32_t dwPortNumber, CtrlCommand_CB callback, void* handle)
	: m_dwPortNumber(dwPortNumber)
	, m_callback(callback)
	, m_handle(handle)
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

void PowerPort::AddSchedule(const string& action, const string& value, int step)
{
	m_schedule.push_back(CommandModel(m_dwPortNumber, action, value, step));
}

inline void PowerPort::ClearSchedule()
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

inline string PowerPort::GetCurrent() { return current; }
inline int PowerPort::GetStep() { return step; }
inline BOOL PowerPort::isRun() { return m_bStart; }
inline void PowerPort::SetCurrent(const string& current) { this->current = current; }

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
					Sleep((port->m_schedule)[i].getDelay());					
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

inline int PowerController::GetPortCount()
{
	return (int)powerport.size();
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

void PowerController::SendCommand(const int port_index, const string& action, const string& value)
{
	CommandModel commandModel = CommandModel(port_index + 1, action, value);

	switch (commandModel.getCommandType())
	{
	case eCmdISet:
		powerport[port_index].SetCurrent(value);
		//powerport[port_index].current = value;
		SendCommand((BYTE*)commandModel.getCommand(), commandModel.getCommandLength());
		break;
	case eCmdVset:
		SendCommand((BYTE*)commandModel.getCommand(), commandModel.getCommandLength());
		break;
	}
}

void PowerController::SendCommand(const string& command)
{
	SendCommand((BYTE*)command.c_str(), command.length());
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

BOOL PowerController::AddSchedule(const int port_index, const string& action, const string& value, int step)
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
	return (port_index < GetPortCount()) ? powerport[port_index].isRun() : FALSE;
}

void PowerController::ResetSchedule(int port_index)
{
	if (port_index < GetPortCount())
	{
		powerport[port_index].ClearSchedule();
	}
}

string PowerController::GetCurrent(int port_index)
{
	static string empty;
	return (port_index < GetPortCount()) ? powerport[port_index].GetCurrent() : empty;
}

void PowerController::Command_CB(char* buffer, int len, void* handle)
{
	PowerController* p = (PowerController*)handle;
	p->WriteSerial((BYTE*)buffer, len);
}

int PowerController::GetStep(int port_index)
{
	return (port_index < GetPortCount()) ? powerport[port_index].GetStep() : -1;
}