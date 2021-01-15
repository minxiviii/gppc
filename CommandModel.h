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
const string arrCommands[CommandsCount] = { "DELAY", "ISET", "VSET" };

class CommandModel
{
private:
	int commandType;
	char command[20];
	int commandLength;
	uint32_t delayMs;

	string current;
	int step;

public:
	CommandModel(const uint32_t ch, const string& action, const string& value, int step = 0);

	uint32_t getDelay();
	int getCommandType();
	char* getCommand();
	int getCommandLength();
	string getCurrent();
	int getStep();
};