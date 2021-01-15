#pragma once

#include ".\serial\CSerial.h"
#include "CommandModel.h"

/********************* PowerPort *********************/
class PowerPort
{
private:
	PowerPort();
	uint32_t m_dwPortNumber;
	vector<CommandModel> m_schedule;

	HANDLE m_hScheduleThread;
	HANDLE m_hSemaphore;
	BOOL m_bThreadRun;
	BOOL m_bStart;

	string current;
	int step;

	void* m_handle;
	CtrlCommand_CB m_callback;
	static DWORD WINAPI ScheduleThread(void* data);

public:
	PowerPort(uint32_t dwPortNumber, CtrlCommand_CB callback, void* handle);
	~PowerPort();
	void InitSchedule();
	void AddSchedule(const string& action, const string& value, int step = 0);
	void ClearSchedule();
	void StartSchedule();
	void ThreadClose();
	string GetCurrent();
	void SetCurrent(const string& current);
	int GetStep();
	BOOL isRun();
};

/*********************    PowerController    *********************/
class PowerController : public CSerial
{
private:
	int id;
	void* context;
	CallbackFunc callbackfunc;

	vector<PowerPort> powerport;
	static void Command_CB(char* buffer, int len, void* handle);

public:
	PowerController();
	~PowerController();

	void Init(int id, int port_count, CallbackFunc cb, void* ctx);
	void Deinit();

	int GetID();
	
	int GetPortCount();
	BOOL ConnectSerial(CString port, CString buadrate = _T("57600"));
	BOOL DisconnectSerial();

	void SendCommand(const int port_index, const string& action, const string& value);
	void SendCommand(const string& command);
	void SendCommand(BYTE* buffer, int len);

	void StartScheduler(int port_index);
	BOOL isScheduling(int port_index);
	BOOL AddSchedule(int port_index, const string& action, const string& value, int step = 0);
	void ResetSchedule();
	void ResetSchedule(int port_index);

	string GetCurrent(int port_index);
	int GetStep(int port_index);
};