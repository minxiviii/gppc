#pragma once

#include ".\serial\CSerial.h"
#include "CommandModel.h"

/********************* PowerPort *********************/
class PowerPort
{
private:
	PowerPort();
	DWORD m_dwPortNumber;
	vector<CommandModel> m_schedule;

	HANDLE m_hScheduleThread;
	HANDLE m_hSemaphore;
	BOOL m_bThreadRun;
	BOOL m_bEnableLog;
	BOOL m_bStart;

	void* m_handle;
	CtrlCommand_CB m_callback;
	static DWORD WINAPI ScheduleThread(void* data);

public:
	PowerPort(DWORD dwPortNumber, CtrlCommand_CB callback, void* handle, BOOL bEnableLog = TRUE);
	~PowerPort();
	void InitSchedule();
	void AddSchedule(CString& action, CString& value);
	void ClearSchedule();
	void StartSchedule();
	void ThreadClose();
	BOOL isRun() {
		return m_bStart;
	}
};

/*********************    PowerController    *********************/
class PowerController : public CSerial
{
private:
	int id;
	void* context;
	CallbackFunc callbackfunc;

	BOOL thread_run;
	HANDLE recv_thread;
	static DWORD WINAPI RecvThread(void* data);
	void ThreadClose();

	vector<PowerPort> powerport;
	static void Command_CB(char* buffer, int len, void* handle);

public:
	PowerController();
	~PowerController();

	void Init(int id, int port_count, CallbackFunc cb, void* ctx);
	void Deinit();

	int GetID();
	
	size_t GetPortCount();
	BOOL ConnectSerial(CString port, CString buadrate = _T("38400"));
	BOOL DisconnectSerial();

	void SendCommand(int port_index, CString& action, CString& value);
	void SendCommand(CString& command);
	void SendCommand(BYTE* buffer, int len);

	void StartScheduler(int port_index);
	BOOL isScheduling(int port_index);
	BOOL AddSchedule(int port_index, CString& action, CString& value);
	void ResetSchedule();
	void ResetSchedule(int port_index);
};