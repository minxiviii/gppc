#include "pch.h"
#include "StepInfo.h"

StepGroup::StepGroup(string start_current, string end_current, string interval, vector<int> output_ports)
	: start_current(start_current)
	, end_current(end_current)
	, interval(interval)
	, output_ports(output_ports)
{

}

StepGroup::~StepGroup() { output_ports.clear(); }

string StepGroup::GetStartCurrent() { return start_current; }
string StepGroup::GetEndCurrent() { return end_current; }
string StepGroup::GetInterval() { return interval; }
vector<int> StepGroup::GetOutputPorts() { return output_ports; }
