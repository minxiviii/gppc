#include "pch.h"
#include "Loadcell.h"

Loadcell::Loadcell()
	: id(0)
	, recv_thread(INVALID_HANDLE_VALUE)
	, thread_run(FALSE)
{}

Loadcell::~Loadcell()
{
	DisconnectSerial();
}

BOOL Loadcell::ConnectSerial(CString port, CString buadrate)
{
	CString pullpath(_T("\\\\.\\") + port);

	BOOL rst = OpenPort(pullpath);
	if (rst == FALSE) { return rst; }

	//InitPort(_ttoi(buadrate), NOPARITY, 8, ONESTOPBIT, FLOWNONE);
	InitPort(_ttoi(buadrate), EVENPARITY, 7, ONESTOPBIT, FLOWNONE);
	recv_thread = CreateThread(NULL, 0, RecvThread, this, 0, 0);

	return rst;
}

BOOL Loadcell::DisconnectSerial()
{
	ThreadClose();
	return ClosePort();
}

void Loadcell::Init(int id, CallbackFunc cb, void* ctx)
{
	this->id = id;
	this->callbackfunc = cb;
	this->context = ctx;
}

int Loadcell::GetID()
{
	return this->id;
}

void Loadcell::ThreadClose()
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

DWORD WINAPI Loadcell::RecvThread(void* data)
{
	Loadcell* loadcell = (Loadcell*)data;

	const int kBufferSize = 256;
	BYTE buffer[kBufferSize] = { 0, };
	DWORD receive_length(0);
	BYTE onebyte;
	DWORD event(0);

	loadcell->thread_run = TRUE;
	SetCommMask(loadcell->m_hComm, EV_RXCHAR);

	while (loadcell->thread_run)
	{
		event = 0;
		WaitCommEvent(loadcell->m_hComm, &event, NULL);
		if (event & EV_RXCHAR)
		{
			while (loadcell->ReadSerial(&onebyte, 1) > 0)
			{
				if (receive_length < kBufferSize)
				{
					buffer[receive_length++] = onebyte;
					if (onebyte == '\r')
					{
						//int weight[CellCount] = { 0, };
						int count = receive_length / DataLenPerCell;
						for (int i = 0; i < count; i++)
						{
							int pos = i * DataLenPerCell;
							//if (buffer[pos] == 'S')
							{
								int minus = (buffer[pos + 1] == '-') ? -1 : 1;
								char data[6] = { 0, };
								memcpy_s(data, 5, &buffer[pos + 2], 5);
								int new_w = atoi(data);
								loadcell->weight[i] = (new_w == 99999 ? 0 : new_w) * minus;
								//cout << weight[i] << endl;
							}
						}

						//aRecvBuf[dwRecvSize] = 0;
						//cout << count << endl;
						//cout << (char*)buffer << endl;
						loadcell->callbackfunc((void*)&loadcell->id, loadcell->context);
						receive_length = 0;
						Sleep(0);
					}
					else if (onebyte == '\n')
					{
						receive_length--;
					}
				}
				else
				{
					receive_length = 0;
				}
			}
		}
	}

	return 0;
}

const int* Loadcell::GetWeights()
{
	return weight;
}

const int Loadcell::GetWeight(int index)
{
	return weight[index];
}