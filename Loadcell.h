#pragma once

#include ".\serial\CSerial.h"

const int CellCount = 6;
const int DataLenPerCell = 7;

class Loadcell : public CSerial
{
private:
	int id;
	void* context;
	CallbackFunc callbackfunc;

	BOOL thread_run;
	HANDLE recv_thread;
	static DWORD WINAPI RecvThread(void* data);
	void ThreadClose();

	int weight[CellCount];

public:
	Loadcell();
	~Loadcell();
	void Init(int id, CallbackFunc cb, void* ctx);
	int GetID();
	BOOL ConnectSerial(CString port, CString buadrate = _T("19200"));
	BOOL DisconnectSerial();
	const int* GetWeights();
	const int GetWeight(int index);
};