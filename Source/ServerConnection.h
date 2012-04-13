#pragma once
#include "stdafx.h"
#include <stdio.h>
#include <tchar.h>
	
#include <winsock2.h>	// KOMMENTERA UT OM DU FÅR REDEFINITION ERRORS

#pragma comment(lib, "WS2_32.lib")
#define BUFFER_SIZE 256
class ServerConnection
{
private:
	struct Connection 
	{
		SOCKET sock;
		char buf[BUFFER_SIZE];
		char bufW[BUFFER_SIZE];
		int numCharsInBuf;
		int numCharsInBufW;
		Connection(){}
		Connection(SOCKET socket)
		{ 
			this->sock = socket;
			this->numCharsInBuf = 0;
			this->numCharsInBufW = 0;
		}
	};

	char *mIp;
	int mPort;
	Connection mServerSocket;
	vector<Connection> mClientSocket;
	bool mServer;
public:
	ServerConnection();
	virtual ~ServerConnection();
	void SetIP(char ip[]);
	void SetPort(int port);
	int InitializeConnection();
	int ListenForClient();
	void Read();
	void Write();
	void Update();
	void SetupFDSets(fd_set& ReadFDs, fd_set& WriteFDs, 
        fd_set& ExceptFDs, SOCKET ListeningSocket = INVALID_SOCKET);
	bool IsServer() const {return this->mServer;}

	void GetReadBuffer(char* bufOut, int size);
	void SetWriteBuffer(char* buf, int size);
};