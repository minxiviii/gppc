#pragma once

class IniConfig
{
public:
	static const CString kFilepath;
	static const CString kSectionSerial;
	static const CString kKeyPort;
	static const CString kKeyBaudrate;
	static const CString kKeyParity;
	static const CString kKeyDatabits;
	static const CString kKeyStopbits;
	static const CString kKeyFlowControl;

	static void IniConfig::Write(const CString& section, const CString& key, const CString& value);
	static CString IniConfig::Read(const CString& section, const CString& key);
};

