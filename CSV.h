#pragma once

class CSV
{
private:
	HANDLE m_hCSV;

public:
	CSV() { m_hCSV = INVALID_HANDLE_VALUE; }
	~CSV() { Close(); }

	BOOL isOpen();

	BOOL Open(BOOL default_column = TRUE);
	void Write(const string& str);
	void Close();
};