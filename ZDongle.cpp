#include "pch.h"
#include "ZDongle.h"

/*
ZDongle::ZDongle() : context(nullptr)
	,callbackfunc(nullptr)
	,recv_thread(INVALID_HANDLE_VALUE)
	,thread_run(FALSE)
{
}
*/

ZDongle::ZDongle()
	: recv_thread(INVALID_HANDLE_VALUE)
	, thread_run(FALSE)
{
}

ZDongle::~ZDongle()
{
	DisconnectSerial();
}

void ZDongle::Init(CallbackFunc cb, void* ctx)
{
	this->callbackfunc = cb;
	this->context = ctx;
}

BOOL ZDongle::ConnectSerial(CString port, CString buadrate)
{
	CString pullpath(_T("\\\\.\\") + port);

	BOOL rst = OpenPort(pullpath);
	if (rst == FALSE) { return rst; }

	InitPort(_ttoi(buadrate), NOPARITY, 8, ONESTOPBIT, FLOWNONE);
	recv_thread = CreateThread(NULL, 0, RecvThread, this, 0, 0);

	return rst;
}

BOOL ZDongle::DisconnectSerial()
{
	ThreadClose();
	return ClosePort();
}

void ZDongle::ThreadClose()
{
	if (recv_thread == INVALID_HANDLE_VALUE) { return; }

	thread_run = FALSE;
	SetCommMask(m_hComm, 0);
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

DWORD WINAPI ZDongle::RecvThread(void* data)
{
	ZDongle* zdongle = (ZDongle*)data;
	/*
	const int kBufferSize = 256;
	BYTE buffer[kBufferSize] = { 0, };
	DWORD receive_length(0);
	*/

	BYTE onebyte;	
	DWORD event(0);

	zdongle->thread_run = TRUE;
	SetCommMask(zdongle->m_hComm, EV_RXCHAR);

	while (zdongle->thread_run)
	{
		event = 0;
		WaitCommEvent(zdongle->m_hComm, &event, NULL);
		if (event & EV_RXCHAR)
		{
			while (zdongle->ReadSerial(&onebyte, 1) > 0)
			{
				ZDongleData receive_data(onebyte);
				zdongle->callbackfunc((void*)&receive_data, zdongle->context);
				
				Sleep(0);
			}
		}
	}

	return 0;
}