#include "pch.h"
#include "UDP_HallSensor.h"

UDP_HallSensor::UDP_HallSensor()
	: recv_thread(INVALID_HANDLE_VALUE)
	, thread_run(FALSE)
	, sock(INVALID_SOCKET)
{
}

UDP_HallSensor::~UDP_HallSensor()
{
	CloseSocket();
}

void UDP_HallSensor::Init(CallbackFunc cb, void* ctx)
{
	this->callbackfunc = cb;
	this->context = ctx;
}

BOOL UDP_HallSensor::OpenSocket(int port)
{
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == INVALID_SOCKET)
	{
		cout << "socket()" << endl;
		return FALSE;
	}

	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(port);
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

	int retval = bind(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR)
	{
		cout << "bind()" << endl;
		return FALSE;
	}

	recv_thread = CreateThread(NULL, 0, RecvThread, this, 0, 0);

	return TRUE;
}

void UDP_HallSensor::CloseSocket()
{
	if (sock == INVALID_SOCKET) { return; }
	closesocket(sock);
	ThreadClose();

	sock = INVALID_SOCKET;
}

void UDP_HallSensor::ThreadClose()
{
	if (recv_thread == INVALID_HANDLE_VALUE) { return; }

	thread_run = FALSE;
	switch (WaitForSingleObject(recv_thread, 1000))
	{
	case WAIT_OBJECT_0:
		break;

	case WAIT_TIMEOUT:
		TerminateThread(recv_thread, WAIT_TIMEOUT);
		break;
	}

	CloseHandle(recv_thread);
	recv_thread = INVALID_HANDLE_VALUE;
}

#include <sstream>
//#include <iostream>
//#include <vector>
//using namespace std;

DWORD WINAPI UDP_HallSensor::RecvThread(void* data)
{
	UDP_HallSensor* hallSensor = (UDP_HallSensor*)data;
	
	const int kBufferSize = 512;
	char buffer[kBufferSize] = { 0, };
		
	SOCKADDR_IN clientaddr;
	
	char* start_p = buffer;
	char* cur_p = buffer;
	hallSensor->thread_run = TRUE;
	while (hallSensor->thread_run)
	{
		int addrlen = sizeof(clientaddr);
		int retval = recvfrom(hallSensor->sock, buffer, kBufferSize, 0, (SOCKADDR*)&clientaddr, &addrlen);

		if (retval == 0)
		{
			cout << ("socket end") << endl;
			hallSensor->thread_run = FALSE;
			continue;
		}
		else if (retval < 0)
		{
			cout << ("recvfrom()") << endl;
			continue;
		}

		buffer[retval] = 0;

		string line = string(buffer);

		//cout << "UDP : " << buffer << endl;

		//ZDongleData receive_data(onebyte);
		hallSensor->callbackfunc((void*)&line, hallSensor->context);

		Sleep(0);
	}

	return 0;
}