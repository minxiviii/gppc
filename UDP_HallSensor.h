#pragma once

#include <WinSock2.h>

class UDP_HallSensor
{
private:
	CallbackFunc callbackfunc;
	void* context;

	BOOL thread_run;
	HANDLE recv_thread;
	static DWORD WINAPI RecvThread(void* data);
	void ThreadClose();
	
	SOCKET sock;

public:
	UDP_HallSensor();
	~UDP_HallSensor();
	void Init(CallbackFunc cb, void* ctx);
	BOOL OpenSocket(int port = 9000);
	void CloseSocket();
};