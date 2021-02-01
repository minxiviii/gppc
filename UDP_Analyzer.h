#pragma once

#include <WinSock2.h>

class UDP_Analyzer
{
public:
	static const string kMsgStart;
	static const string kMsgRunning;
	static const string kMsgFinish;

private:
	SOCKET sock;
	SOCKADDR_IN serveraddr;

public:
	UDP_Analyzer();
	~UDP_Analyzer();
	BOOL OpenSocket(const int port = 5000, const string& target_ip = "127.0.0.1");
	void CloseSocket();
	void Send(const string& data);
};