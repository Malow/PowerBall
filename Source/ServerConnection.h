#pragma once
#include "stdafx.h"
#include <stdio.h>
#include <tchar.h>
//#include <winsock2.h>

#pragma comment(lib, "WS2_32.lib")
#define BUFFER_SIZE 256
class ServerConnection
{
private:
	
	struct Connection 
	{
		HANDLE handle;
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

	char				*mIp;
	int					mPort;
	Connection*			mServerSocket;
	vector<Connection*>	mClientSocket;
	int					mNumClients;
	bool				mServer;
public:
				ServerConnection();
	virtual		~ServerConnection();
	void		SetIP(char ip[]);
	void		SetPort(int port);
	void		InitializeConnection();
	void		Read();
	void		Write();
	void		Update();
	void		SetupFDSets(fd_set& ReadFDs, fd_set& WriteFDs, fd_set& ExceptFDs);
	bool		IsServer() const {return this->mServer;}
	int			GetNumConnections() const {return this->mNumClients + 1;}

	bool		GetReadBuffer(char* bufOut, int size, int clientIndex);
	void		SetWriteBuffer(char* buf, int size, int clientIndex);
	void		Close();

	
	static DWORD WINAPI		TalkToClient(void* param);
	static DWORD WINAPI		TalkToServer(void* param);
	static DWORD WINAPI		ListenForClient(void* param);

};