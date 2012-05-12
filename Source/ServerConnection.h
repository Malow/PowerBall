#pragma once
#include "stdafx.h"
#include <stdio.h>
#include <tchar.h>
#include "ServerInfo.h"
#include "MsgHandler.h"
#include <winsock2.h>
#include <WinSock.h> //for le nupcomputer

#pragma comment(lib, "WS2_32.lib")
#define BUFFER_SIZE 1024
#define PACKET_QUEUE_SIZE 10
class ServerConnection
{
private:
	
	struct Connection 
	{
		sockaddr_in adress;
		HANDLE handle;
		SOCKET sock;
		char buf[BUFFER_SIZE];
		char bufW[BUFFER_SIZE];
		int numCharsInBuf;
		int numCharsInBufW;
		int index;
		volatile bool running;
		Connection(){}
		Connection(SOCKET socket)
		{ 
			this->index = 0;
			this->sock = socket;
			this->numCharsInBuf = 0;
			this->numCharsInBufW = 0;
			this->running = true;
		}
	};
	vector<ServerInfo>	mServerInfos;
	ServerInfo			mCurrentServer;
	int					mPort;
	Connection*			mServerSocket;
	vector<Connection*>	mClientSocket;
	int					mNumClients;
	bool				mServer;

	/*! Thread for communication between server and client. */
	static DWORD WINAPI		Communicate(void* param);
	static DWORD WINAPI		CommunicateClient(void* param);

	/*! Thread for the LAN host that listens for new client conections. */
	static DWORD WINAPI		ListenForClient(void* param);

public:
	void Send(int index);
				ServerConnection();
	virtual		~ServerConnection();

	/*! Sets the network port. */
	void		SetPort(const int port);

	/*! Sets the buffer that will be sent over network. */
	void		SetWriteBuffer(const char* buf, const int size, const int clientIndex);

	/*! Returns the number of connections on the network. */
	int			GetNumConnections() const {return this->mNumClients + 1;}

	/*! Returns the buffer with data thats been received from other connections. */
	bool		GetReadBuffer(char* bufOut, const int size, const int clientIndex);

	/*! Initializes the connection. */
	void		Host(ServerInfo server);
	void		Connect(ServerInfo server);

	/*! Broadcasts a msgs that servers will pick up on the lan and reply to it. */
	vector<ServerInfo>	FindServers();

	/*! Returns true if you're the LAN host. */
	bool		IsServer() const {return this->mServer;}

	/*! Closes LAN. */
	void		Close();
};