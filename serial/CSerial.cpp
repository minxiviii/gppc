#include "pch.h"
#include "CSerial.h"

CSerial::CSerial() : m_hComm(INVALID_HANDLE_VALUE), m_dwErrorCode(ERR_NONE) {}

CSerial::CSerial(const TCHAR* lpszPortName, const DWORD dwBaudrate, const BYTE byParity,
                 const BYTE byDatabits, const BYTE byStopbits, const BYTE byFlow)
    : m_hComm(INVALID_HANDLE_VALUE), m_dwErrorCode(ERR_NONE)
{
    if (OpenPort(lpszPortName))
    {
        InitPort(dwBaudrate, byParity, byDatabits, byStopbits, byFlow);
    }
}

CSerial::~CSerial()
{
    ClosePort();
}

BOOL CSerial::OpenPort(const CString& portname)
{
    if (IsOpen()) { ClosePort(); }
    
    
    m_hComm = CreateFile(portname, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
 
    if (!IsOpen())
    {
        m_dwErrorCode = ERR_NOTOPEN;
        return FALSE;
    }
	else
	{
        
        m_strPortName = portname;
		memset( &m_osRead, 0, sizeof(OVERLAPPED));
		memset( &m_osWrite, 0, sizeof(OVERLAPPED));
		m_osRead.hEvent = CreateEvent( NULL, TRUE, FALSE, NULL );
		m_osWrite.hEvent = CreateEvent( NULL, TRUE, FALSE, NULL );
	}

    return TRUE;
}

CString CSerial::GetPortName()
{
    return m_strPortName;
}

BOOL CSerial::ClosePort()
{
    if (!IsOpen())
    {
        m_dwErrorCode = ERR_NOTOPEN;
        return FALSE;
    }
	
	CloseHandle(m_osRead.hEvent);
	CloseHandle(m_osWrite.hEvent);
    CloseHandle(m_hComm);
    m_hComm = INVALID_HANDLE_VALUE;

    return TRUE;
}

BOOL CSerial::InitPort(const DWORD dwBaudrate, const BYTE byParity, const BYTE byDatabits,
                       const BYTE byStopbits, const BYTE byFlow)
{
    if (!IsOpen())
    {
        m_dwErrorCode = ERR_NOTOPEN;
        return FALSE;
    }
    
    if (!SetupComm(m_hComm, COMM_RX_QSIZE, COMM_TX_QSIZE))
    {
        m_dwErrorCode = ERR_SETUPCOMM;
        return FALSE;
    }
    
    if (!PurgeComm(m_hComm, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR))
    {
        m_dwErrorCode = ERR_PURGECOMM;
        return FALSE;
    }
    
    if (!SetDCB(dwBaudrate, byParity, byDatabits, byStopbits, byFlow))
    {
        return FALSE;
    }
   
    return TRUE;
}

BOOL CSerial::SetTimeouts(DWORD dwRIT, DWORD dwRTTM, DWORD dwRTTC, DWORD dwWTTM, DWORD dwWTTC)
{
    COMMTIMEOUTS timeouts;
    
    timeouts.ReadIntervalTimeout         = dwRIT;
    timeouts.ReadTotalTimeoutMultiplier  = dwRTTM;	// Non-Blocking
    timeouts.ReadTotalTimeoutConstant    = dwRTTC;

    timeouts.WriteTotalTimeoutMultiplier = dwWTTM;
    timeouts.WriteTotalTimeoutConstant   = dwWTTC;
    
    if (!SetCommTimeouts(m_hComm, &timeouts))
    {
        m_dwErrorCode = ERR_SETCOMMTIMEOUTS;
        return FALSE;
    }

    return TRUE;
}

BOOL CSerial::SetDCB(const DWORD dwBaudrate, const BYTE byParity, const BYTE byDatabits,
                     const BYTE byStopbits, const BYTE byFlow)
{
    DCB dcb;
    
    if (!IsOpen())
    {
        m_dwErrorCode = ERR_NOTOPEN;
        return FALSE;
    }

    if (!GetCommState(m_hComm, &dcb))
    {
        m_dwErrorCode = ERR_GETCOMMSTATE;
        return FALSE;
    }

    dcb.BaudRate = dwBaudrate;
    dcb.fBinary  = TRUE;
    dcb.fParity  = FALSE;
    dcb.fNull    = FALSE;
    dcb.ByteSize = byDatabits;
    dcb.Parity   = byParity;
    dcb.fParity  = TRUE;
    dcb.StopBits = byStopbits;
    
    switch (byFlow)
    {
    case FLOWHARD:             // Hardware Flow
        dcb.fOutxCtsFlow = TRUE;                    // Enable CTS monitoring
        dcb.fOutxDsrFlow = TRUE;                    // Enable DSR monitoring
        dcb.fDtrControl  = DTR_CONTROL_HANDSHAKE;   // Enable DTR handshaking
        dcb.fOutX        = FALSE;                   // Disable XON/XOFF for transmission
        dcb.fInX         = FALSE;                   // Disable XON/XOFF for receiving
        dcb.fRtsControl  = RTS_CONTROL_HANDSHAKE;   // Enable RTS handshaking
        break;

    case FLOWSOFT:             // Software Flow
        dcb.fOutxCtsFlow = FALSE;                   // Disable CTS (Clear To Send)
        dcb.fOutxDsrFlow = FALSE;                   // Disable DSR (Data Set Ready)
        dcb.fDtrControl  = DTR_CONTROL_DISABLE;     // Disable DTR (Data Terminal Ready)
        dcb.fOutX        = TRUE;                    // Enable XON/XOFF for transmission
        dcb.fInX         = TRUE;                    // Enable XON/XOFF for receiving
        dcb.fRtsControl  = RTS_CONTROL_DISABLE;     // Disable RTS (Ready To Send)
        break;

    case FLOWNONE: default:    // NONE Flow
        dcb.fOutxCtsFlow = FALSE;                   // Disable CTS monitoring
        dcb.fOutxDsrFlow = FALSE;                   // Disable DSR monitoring
        dcb.fDtrControl  = DTR_CONTROL_DISABLE;     // Disable DTR monitoring
        dcb.fOutX        = FALSE;                   // Disable XON/XOFF for transmission
        dcb.fInX         = FALSE;                   // Disable XON/XOFF for receiving
        dcb.fRtsControl  = RTS_CONTROL_DISABLE;     // Disable RTS (Ready To Send)
        break;
    }

    if (!SetCommState(m_hComm, &dcb))
    {
        m_dwErrorCode = ERR_SETCOMMSTATE;
        return FALSE;
    }

    return TRUE;
}

BOOL CSerial::GetDCB(DCB &dcb)
{
    if (!IsOpen())
    {
        m_dwErrorCode = ERR_NOTOPEN;
        return FALSE;
    }
    
    if (!GetCommState(m_hComm, &dcb))
    {
        m_dwErrorCode = ERR_GETCOMMSTATE;
        return FALSE;
    }
    
    return TRUE;
}

BOOL CSerial::IsOpen()
{
	return (m_hComm == INVALID_HANDLE_VALUE) ? FALSE : TRUE;
}

HANDLE CSerial::GetSerialHandle()
{
	HANDLE hHandle = m_hComm;
	return hHandle;
}

DWORD CSerial::GetLastError()
{
    DWORD dwErr = m_dwErrorCode;
    m_dwErrorCode = ERR_NONE;

    return dwErr;
}

DWORD CSerial::WriteSerial(const BYTE* pbyDataBuf, const DWORD nDataSize)
{
	DWORD	dwWritten, dwError, dwErrorFlags;
	COMSTAT	comstat;

		//--> 인자로 들어온 버퍼의 내용을 nToWrite 만큼 쓰고.. 쓴 갯수를.,dwWrite 에 넘김.
	if (!WriteFile(m_hComm, pbyDataBuf, nDataSize, &dwWritten, &m_osWrite))
	{
		//--> 아직 전송할 문자가 남았을 경우..
		if (GetLastError() == ERROR_IO_PENDING)
		{
			// 읽을 문자가 남아 있거나 전송할 문자가 남아 있을 경우 Overapped IO의
			// 특성에 따라 ERROR_IO_PENDING 에러 메시지가 전달된다.
			//timeouts에 정해준 시간만큼 기다려준다.
			while (!GetOverlappedResult( m_hComm, &m_osWrite, &dwWritten, TRUE))
			{
				dwError = GetLastError();
				if (dwError != ERROR_IO_INCOMPLETE)
				{
					ClearCommError(m_hComm, &dwErrorFlags, &comstat);
					break;
				}
			}
		}
		else
		{
			dwWritten = 0;
			ClearCommError( m_hComm, &dwErrorFlags, &comstat);
		}
	}

    return dwWritten;
}

DWORD CSerial::ReadSerial(BYTE* const pbyBuf, const DWORD nBufSize)
{
	DWORD	dwRead,dwError, dwErrorFlags;
	COMSTAT comstat;

	//--- system queue에 도착한 byte수만 미리 읽는다.
	ClearCommError( m_hComm, &dwErrorFlags, &comstat);

	//--> 시스템 큐에서 읽을 거리가 있으면..
	dwRead = comstat.cbInQue;

	
	if(dwRead > 0)
	{
		//--> 버퍼에 일단 읽어들이는데.. 만일..읽어들인값이 없다면..

		if(!ReadFile( m_hComm, pbyBuf, nBufSize, &dwRead, &m_osRead) )
		{
			//--> 읽을 거리가 남았으면..
			if (GetLastError() == ERROR_IO_PENDING)
			{
				//--------- timeouts에 정해준 시간만큼 기다려준다.
				while (! GetOverlappedResult( m_hComm, &m_osRead, &dwRead, TRUE))
				{
					dwError = GetLastError();
					if (dwError != ERROR_IO_INCOMPLETE)
					{
						ClearCommError( m_hComm, &dwErrorFlags, &comstat);
						break;
					}
				}
			}
			else
			{
				dwRead = 0;
				ClearCommError( m_hComm, &dwErrorFlags, &comstat);
			}
		}
	}


	//--> 실제 읽어들인 갯수를 리턴.
	return dwRead;

}
