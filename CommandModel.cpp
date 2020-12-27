#include "pch.h"
#include "CommandModel.h"

/********************* CommandModel *********************/
CommandModel::CommandModel(DWORD ch, CString& action, CString& value)
{
	if (action.CompareNoCase(arrCommands[eCmdDelay]) == 0)
	{
		this->commandType = eCmdDelay;
		delayMs = (DWORD)_ttoi(value);
	}
	else if (action.CompareNoCase(arrCommands[eCmdISet]) == 0)
	{
		this->commandType = eCmdISet;
		double val = _wtof(value);
		if (val < 0) { val = 0; }
		else if (val > AMax) { val = AMax; }

		sprintf_s(command, "ISET%d:%.3f\r\n", ch, val);
		commandLength = strlen(command);
	}
	else if (action.CompareNoCase(arrCommands[eCmdVset]) == 0)
	{
		this->commandType = eCmdVset;
		double val = _wtof(value);
		if (val < 0) { val = 0; }
		else if (val > VMax) { val = VMax; }

		sprintf_s(command, "VSET%d:%.3f\r\n", ch, val);
		commandLength = strlen(command);
	}
	else
	{
		this->commandType = CommandsCount;
	}
}

DWORD CommandModel::getDelay() { return delayMs; }
int CommandModel::getCommandType() { return commandType; }
char* CommandModel::getCommand() { return command; }
size_t CommandModel::getCommandLength() { return commandLength; }
