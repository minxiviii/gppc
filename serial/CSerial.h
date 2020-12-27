#ifndef __CSERIAL_H_BY_MINX__
#define __CSERIAL_H_BY_MINX__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

enum { COMM_RX_QSIZE = 4096, COMM_TX_QSIZE = 4096 };
enum { FLOWSOFT = 0, FLOWHARD, FLOWNONE};
enum { ERR_NONE = 0,
       ERR_OPENED,
       ERR_NOTOPEN,
       ERR_SETUPCOMM,
       ERR_PURGECOMM,
       ERR_SETCOMMMASK,
       ERR_SETCOMMTIMEOUTS,
       ERR_GETCOMMSTATE,
       ERR_SETCOMMSTATE};

class CSerial
{
public:
    CSerial();
    CSerial(const TCHAR* const lpszPortName, const DWORD dwBaudrate, const BYTE byParity,
            const BYTE byDatabits, const BYTE byStopbits, const BYTE byFlow);
    virtual ~CSerial();

    BOOL OpenPort(const CString& portname);
    BOOL ClosePort();
    BOOL InitPort(const DWORD dwBaudrate, const BYTE byParity, const BYTE byDatabits,
                           const BYTE byStopbits, const BYTE byFlow);
    BOOL SetTimeouts(DWORD dwRIT, DWORD dwRTTM, DWORD dwRTTC, DWORD dwWTTM, DWORD dwWTTC);
    BOOL SetDCB(const DWORD dwBaudrate, const BYTE byParity, const BYTE byDatabits,
                         const BYTE byStopbits, const BYTE byFlow);
    BOOL GetDCB(DCB &dcb);
    BOOL IsOpen();
    HANDLE GetSerialHandle();
    DWORD GetLastError();
    DWORD WriteSerial(const BYTE* pbyDataBuf, const DWORD nDataSize);
    DWORD ReadSerial(BYTE* const pbyBuf, const DWORD nBufSize);

	OVERLAPPED m_osRead, m_osWrite;
	HANDLE m_hComm;
    CString m_strPortName;
    CString GetPortName();
protected:
    
    DWORD m_dwErrorCode;

private:
    CSerial(const CSerial&);
    CSerial& operator=(const CSerial&);
};

#endif
