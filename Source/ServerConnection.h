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
		sockaddr_in adress;
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

	/*! Thread for communication between server and client. */
	static DWORD WINAPI		TalkToClient(void* param);

	// SPLIT INTO TWO! --->>>----<<<
	/*! Thread for communication between client and server. */
	static DWORD WINAPI		TalkToServer(void* param);
	static DWORD WINAPI		ReadFromServer(void* param);
	static DWORD WINAPI		WriteToServer(void* param);

	/*! Thread for the LAN host that listens for new client conections. */
	static DWORD WINAPI		ListenForClient(void* param);

	/*! Receives data buffer on socket. */
	void		Read();

	/*! Sends data buffer over socket. */
	void		Write();

	/*! Setups the Select params for clients. */
	void		SetupFDSets(fd_set& ReadFDs, fd_set& WriteFDs, fd_set& ExceptFDs);

public:
				ServerConnection();
	virtual		~ServerConnection();

	/*! Sets the IP that the client will connect to. */
	void		SetIP(char ip[]);

	/*! Sets the network port. */
	void		SetPort(const int port);

	/*! Sets the buffer that will be sent over network. */
	void		SetWriteBuffer(const char* buf, const int size, const int clientIndex);

	/*! Returns the number of connections on the network. */
	int			GetNumConnections() const {return this->mNumClients + 1;}

	/*! Returns the buffer with data thats been received from other connections. */
	bool		GetReadBuffer(char* bufOut, const int size, const int clientIndex);

	/*! Initializes the connection, if connection fails you will become LAN host. */
	void		InitializeConnection(); //temporary for trying out LAN with multiple clients on the same computer.
	void		Host();
	void		Connect();

	/*! Sends and receives data over socket, only called by clients at the moment. */
	bool		Update();

	/*! Returns true if you're the LAN host. */
	bool		IsServer() const {return this->mServer;}

	/*! Closes LAN. */
	void		Close();
};