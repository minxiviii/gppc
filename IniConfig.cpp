#include "pch.h"
#include "IniConfig.h"

const CString kFilepath = _T(".\\config.ini");
const CString kSectionSerial = _T("GPP_SERIAL");

const CString kKeyBaudrate = _T("BAUDRATE");
const CString kKeyParity = _T("PARITY");
const CString kKeyDatabits = _T("DATABITS");
const CString kKeyStopbits = _T("STOPBITS");
const CString kKeyFlowControl = _T("FLOWCONTROL");

void IniConfig::Init()
{
	//WritePrivateProfileString(kSectionSerial, kKeyBaudrate, _T("asdf"), kFilepath);
}

