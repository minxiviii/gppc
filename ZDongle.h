#pragma once

#include ".\serial\CSerial.h"

class ZDongleData
{
private:
	int value;
public:
	ZDongleData() :value(0) { value = 0; }
	ZDongleData(const int v):value(v) {}
	void SetId(const int v) { value = v; }
	int GetId() { return value; }
};

class ZDongle : public CSerial
{
private:
	CallbackFunc callbackfunc;
	void* context;
	
	BOOL thread_run;
	HANDLE recv_thread;
	static DWORD WINAPI RecvThread(void* data);
	void ThreadClose();

public:
	ZDongle();
	~ZDongle();
	void Init(CallbackFunc cb, void* ctx);
	BOOL ConnectSerial(CString port, CString buadrate = _T("115200"));
	BOOL DisconnectSerial();
};