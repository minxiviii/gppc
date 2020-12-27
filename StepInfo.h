#pragma once

class StepGroup
{
private:
	string start_current;
	string end_current;
	string interval;
	vector<int> output_ports;

	StepGroup() {}

public:
	StepGroup(string start_current, string end_current, string interval, vector<int> output_ports);
	~StepGroup();

	string GetStartCurrent();
	string GetEndCurrent();
	string GetInterval();
	vector<int> GetOutputPorts();

	//StepGroup& operator=(const StepGroup&);
};

/*
class StepInfo
{
private:
	vector<StepGroup> step_groups;

public:
	StepInfo() : step_groups(3) {}
	~StepInfo() { step_groups.clear(); }

	void Add(StepGroup step_group)
	{
		step_groups.push_back(step_group);
	}

	void Remove(int index)
	{
		//step_groups.erase(index);
	}

	StepGroup Get(int index)
	{
		return step_groups.at(index);
	}
	
};
*/