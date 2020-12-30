#pragma once

typedef void (*CtrlCommand_CB)(char* command, int length, void* handle);

enum {
	eCmdDelay = 0,
	eCmdISet,
	eCmdVset,
	CommandsCount
};

const float VMax = 32.0f;
const float AMax = 6.2f;
const CString arrCommands[CommandsCount] = { _T("DELAY"), _T("ISET"), _T("VSET") };


class CommandModel
{
private:
	int commandType;
	char command[20];
	size_t commandLength;
	DWORD delayMs;
	CString current;

public:
	CommandModel(DWORD ch, CString& action, CString& value);

	DWORD getDelay();
	int getCommandType();
	char* getCommand();
	size_t getCommandLength();
	CString getCurrent();
};