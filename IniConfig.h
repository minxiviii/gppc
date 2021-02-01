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

	static const CString kKeyEventSelection;
	static const CString kKeyHallSensorPort;
	static const CString kKeyHallSensorStartPos;
	static const CString kKeyHallSensorFinishiPos;
	static const CString kKeyAnalyzerFlag;
	static const CString kKeyAnalyzerIp;
	static const CString kKeyAnalyzerPort;

	static const CString kKeyJsonFile;

	static void IniConfig::Write(const CString& section, const CString& key, const CString& value);
	static CString IniConfig::Read(const CString& section, const CString& key);
};

