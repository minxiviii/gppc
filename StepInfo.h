#pragma once

class StepGroup
{
private:
	/* string for ui*/
	string start_current_text;
	string end_current_text;
	string interval_text;

	float start_current;
	float end_current;
	float interval;

	int dec_start;
	int dec_end;
	int iter;
	int gain;

	int delayms;
	int step;

	vector<int> output_ports;
	StepGroup();

public:
	StepGroup(string start_current, string end_current, string interval, vector<int> output_ports, int delayms = 0, int step = 0);
	~StepGroup();

	string GetStartCurrentText();
	string GetEndCurrentText();
	string GetIntervalText();

	float GetStartCurrent();
	float GetEndCurrent();
	float GetInterval();

	vector<int> GetOutputPorts();
	BOOL IsOver();
	void ResetCurrent();
	void Gain();
	float GetCurrnet();

	int GetDelayms();
	int GetStep();
};