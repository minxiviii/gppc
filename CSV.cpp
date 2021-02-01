#include "pch.h"
#include "CSV.h"


BOOL CSV::Open(BOOL default_column)
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

	if (default_column)
	{
		string row;
		row = "index";
		row += ",time";
		row += ",section";
		row += ",step";
		row += ",m1,m2,m3,m4,m5,m6,m7,m8,m9,m10,m11";
		row += ",m12,m13,m14,m15,m16,m17,m18,m19,m20,m21,m22";
		row += ",m23,m24,m25,m26,m27,m28,m29,m30,m31,m32,m33";
		row += ",w1,w2,w3,w4,w5,w6";
		row += ",w7,w8,w9,w10,w11,w12";
		row += ",w13,w14,w15,w16,w17,w18";
		row += "\n";
		Write(row);
	}

	return TRUE;
}

void CSV::Write(const string& str)
{
	if (m_hCSV == INVALID_HANDLE_VALUE) { return; }

	DWORD written;
	WriteFile(m_hCSV, str.c_str(), (DWORD)str.length(), &written, NULL);
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