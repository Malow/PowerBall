#pragma once
#include "..\stdafx.h"
#include <stdio.h>
#include <tchar.h>
#include "ServerInfo.h"
#include "MsgHandler.h"
#include <winsock2.h>

#pragma comment(lib, "WS2_32.lib")
#define BUFFER_SIZE 1024
#define NOT_RUNNING -666
#define CONNECT_PORT 10000
class Connection
{
private:
		sockaddr_in mLocalAddress;
		sockaddr_in mRemoteAddress;
		HANDLE mThreadHandle;
		SOCKET mSocket;
		//char mRecvBuffer[BUFFER_SIZE];
		//int mRecvBufferSize;
		int mIndex;
		int mProtocol;
		volatile bool mRunning;
public:
				Connection(string remoteIP, int remotePort, int localPort = 0, int protocol = IPPROTO_UDP); //maybe add local IP too, just going with inaddr_any atm
				Connection(sockaddr_in remoteAddress, int localPort = 0, int protocol = IPPROTO_UDP);
	virtual		~Connection();

	void		SetIndex(const int index) { this->mIndex = index; }
	
	void		SetRemoteAddress(sockaddr_in remote) { this->mRemoteAddress = remote; }
	/*! Sets the network port. */
	/*void		SetPort(int port) { this->mAdress.sin_port = htons((u_short) port); }
	void		SetIP(string ip) { this->mAdress.sin_addr.s_addr = inet_addr(ip.c_str()); }
	void		SetAddress(sockaddr_in address) { this->mAdress = address; }
	int			GetPort() { return (int)this->mAdress.sin_port; }
	string		GetIP() { return inet_ntoa(this->mAdress.sin_addr); }
	*/
	string		GetRemoteIP() { return inet_ntoa(this->mRemoteAddress.sin_addr); }
	int 		SetBlocking(bool blocking);
	
	bool		IsRunning(){return this->mRunning;}
	int			GetIndex() { return this->mIndex; }

	/*! Initializes the connection. */
	int			Start();
	
	int			Send(const char* buf, const int size);
	int			Broadcast(const char* buf, const int size);
	int			Receive(char (&buf)[BUFFER_SIZE], sockaddr_in &from = sockaddr_in());
	/*! Closes Connection. */
	void		Stop();

	/*! Returns the buffer with data thats been received from other connections. */
	//bool		GetRecvBuffer(char* bufOut);

};