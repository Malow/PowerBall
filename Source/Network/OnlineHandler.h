#pragma once
#include "..\stdafx.h"
#include <stdio.h>
#include <tchar.h>
#include "ServerInfo.h"
#include "MsgHandler.h"
#include <winsock2.h>
#include "Connection.h"
#include "BufferFunctions.h"
#pragma comment(lib, "WS2_32.lib")

class OnlineHandler
{
private:
	vector<ServerInfo>	mServerInfos;
	//ServerInfo			mCurrentServer;
	Connection*			mConnection;

	/*! Thread for communication between server and client. */
	static DWORD WINAPI		Communicate(void* param);

public:
	void		Send(const char* buf, const int size, const int clientIndex = 0);
				OnlineHandler();
	virtual		~OnlineHandler();

	/*! Returns the number of connections on the network. */
	int			GetNumConnections() const {return 1;}

	/*! Initializes the connection. */
	bool		Connect(string accName, string accPass);

	/*! Broadcasts a msgs that servers will pick up on the lan and reply to it. */
	vector<ServerInfo>	FindServers();

	/*! Returns true if you're the LAN host. */
	bool		IsServer() const {return false;}

	/*! Closes network. */
	void		Close();
};