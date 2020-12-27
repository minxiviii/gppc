#include "pch.h"
#include "CSV.h"


BOOL CSV::Open()
{
	SYSTEMTIME st;
	GetLocalTime(&st);
	WCHAR filename[128] = { 0, };
	wsprintf(filename, _T("CnC_%04d%02d%02d-%02d%02d%02d.csv"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
	m_hCSV = CreateFile(filename, GENERIC_WRITE | GENERIC_READ, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (m_hCSV == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
}

void CSV::Write(const string& str)
{
	if (m_hCSV == INVALID_HANDLE_VALUE) { return; }

	DWORD written;
	WriteFile(m_hCSV, str.c_str(), str.length(), &written, NULL);
}


void CSV::Close()
{
	if (m_hCSV != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hCSV);
		m_hCSV = INVALID_HANDLE_VALUE;
	}
}

BOOL CSV::isOpen()
{
	return (m_hCSV != INVALID_HANDLE_VALUE) ? true : false;
}