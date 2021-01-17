#include "pch.h"
#include "CommandModel.h"

/********************* CommandModel *********************/
CommandModel::CommandModel(const uint32_t ch, const string& action, const string& value, int step)
	: step(step)
	, commandLength(0)
	, delayMs(0)
{
	if (action.compare(arrCommands[eCmdDelay]) == 0)
	{
		this->commandType = eCmdDelay;
		delayMs = atoi(value.c_str());
		//cout << "step = " << step << ", delay = " << delayMs << endl;
	}
	else if (action.compare(arrCommands[eCmdISet]) == 0)
	{
		this->commandType = eCmdISet;
		double val = atof(value.c_str());
		if (val < 0) { val = 0; }
		else if (val > AMax) { val = AMax; }
		
		this->current = value;
		
		sprintf_s(command, "ISET%d:%.3f\r\n", ch, val);
		commandLength = (int)strlen(command);
	}
	else if (action.compare(arrCommands[eCmdVset]) == 0)
	{
		this->commandType = eCmdVset;
		double val = atof(value.c_str());
		if (val < 0) { val = 0; }
		else if (val > VMax) { val = VMax; }

		sprintf_s(command, "VSET%d:%.3f\r\n", ch, val);
		commandLength = (int)strlen(command);
	}
	else
	{
		this->commandType = CommandsCount;
	}
}

uint32_t CommandModel::getDelay() { return delayMs; }
int CommandModel::getCommandType() { return commandType; }
char* CommandModel::getCommand() { return command; }
int CommandModel::getCommandLength() { return commandLength; }
string CommandModel::getCurrent() { return current; }
int CommandModel::getStep() { return step; }
