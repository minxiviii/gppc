#include "pch.h"
#include "UDP_Analyzer.h"

#include <WS2tcpip.h>

UDP_Analyzer::UDP_Analyzer()
	: sock(INVALID_SOCKET)
{
}

UDP_Analyzer::~UDP_Analyzer()
{
	CloseSocket();
}

BOOL UDP_Analyzer::OpenSocket(const int port, const string& target_ip)
{
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == INVALID_SOCKET)
	{
		cout << "socket()" << endl;
		return FALSE;
	}

	ZeroMemory(&serveraddr, sizeof(serveraddr));

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(port);
	int retval = inet_pton(AF_INET, target_ip.c_str(), &serveraddr.sin_addr.s_addr);//htonl(INADDR_ANY);
	if (retval != 1)
	{
		CloseSocket();
	}

	return TRUE;
}

void UDP_Analyzer::CloseSocket()
{
	if (sock == INVALID_SOCKET) { return; }
	closesocket(sock);

	sock = INVALID_SOCKET;
}

void UDP_Analyzer::Send(const string& data)
{
	if (sock == INVALID_SOCKET) { return; }
	sendto(sock, data.c_str(), data.length(), 0, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
}