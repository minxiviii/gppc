#include "pch.h"
#include "StepInfo.h"

StepGroup::StepGroup(string start_current, string end_current, string interval, vector<int> output_ports, int delayms)
	: start_current_text(start_current)
	, end_current_text(end_current)
	, interval_text(interval)
	, output_ports(output_ports)
	, delayms(delayms)
{
	if (start_current_text.length() > 0) { this->start_current = stof(start_current_text); }
	if (end_current_text.length() > 0) { this->end_current = stof(end_current_text); }
	if (interval_text.length() > 0) { this->interval = stof(interval_text); }


	dec_start = (int)(this->start_current * 100.0f);
	dec_end = (int)(this->end_current * 100.0f);

	iter = dec_start;
	gain = (int)(this->interval * 100.0f);

	if (this->start_current > this->end_current)
	{
		gain *= -1;
	}
}

StepGroup::~StepGroup() { output_ports.clear(); }

float StepGroup::GetStartCurrent() { return start_current; }
float StepGroup::GetEndCurrent() { return end_current; }
float StepGroup::GetInterval() { return interval; }

string StepGroup::GetStartCurrentText() { return start_current_text; }
string StepGroup::GetEndCurrentText() { return end_current_text; }
string StepGroup::GetIntervalText() { return interval_text; }

vector<int> StepGroup::GetOutputPorts() { return output_ports; }

BOOL StepGroup::IsOver()
{
	BOOL result(FALSE);

	if (start_current < end_current)
	{
		if (iter >= dec_end) { result = TRUE; }
	}
	else
	{
		if (iter <= dec_end) { result = TRUE; }
	}

	return result;
}

void StepGroup::Gain()
{
	iter += gain;
}

void StepGroup::ResetCurrent()
{
	iter = dec_start;
}

float StepGroup::GetCurrnet()
{
	int value = IsOver() ? dec_end : iter;
	
	return (float)value / 100.0f;
}

int StepGroup::GetDelayms()
{
	return delayms;
}