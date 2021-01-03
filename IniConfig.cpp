#include "pch.h"
#include "IniConfig.h"

const CString IniConfig::kFilepath(_T(".\\config.ini"));

const CString IniConfig::kSectionSerial(_T("SERIAL_"));

const CString IniConfig::kKeyPort(_T("PORT"));
const CString IniConfig::kKeyBaudrate(_T("BAUDRATE"));
const CString IniConfig::kKeyParity(_T("PARITY"));
const CString IniConfig::kKeyDatabits(_T("DATABITS"));
const CString IniConfig::kKeyStopbits(_T("STOPBITS"));
const CString IniConfig::kKeyFlowControl(_T("FLOWCONTROL"));

void IniConfig::Write(const CString& section, const CString& key, const CString& value)
{
	WritePrivateProfileString(section, key, value, kFilepath);
}

CString IniConfig::Read(const CString& section, const CString& key)
{
	CString data;
	WCHAR buffer[256];

	GetPrivateProfileString(section, key, _T(""), buffer, sizeof(buffer)/2, kFilepath);

	data = buffer;

	return data;
}

