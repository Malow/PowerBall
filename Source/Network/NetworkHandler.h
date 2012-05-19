#pragma once
#include "..\stdafx.h"
#include <stdio.h>
#include <tchar.h>
#include "ServerInfo.h"
#include "MsgHandler.h"
#include <winsock2.h>
#include "Connection.h"

#pragma comment(lib, "WS2_32.lib")

class NetworkHandler
{
protected:
	vector<ServerInfo>	mServerInfos;
	ServerInfo			mCurrentServer;
	vector<Connection*>	mConnections;
	bool				mServer;

	/*! Thread for communication between server and client. */
	static DWORD WINAPI		Communicate(void* param);
	static DWORD WINAPI		CommunicateServer(void* param);
	static DWORD WINAPI		CommunicateClient(void* param);

	/*! Thread for the LAN host that listens for new client conections. */
	static DWORD WINAPI		ListenForClient(void* param);

public:
	void		ProcessMSGs();
	void		Send(const char* buf, const int size, const int clientIndex = 0);
				NetworkHandler();
	virtual		~NetworkHandler();

	/*! Returns the number of connections on the network. */
	int			GetNumConnections() const {return this->mConnections.size();}
	void		DropPlayer(int index);
	/*! Initializes the connection. */
	void		Host(ServerInfo server);
	bool		Connect(ServerInfo server);
	ServerInfo	ConnectTo(string ip);

	/*! Broadcasts a msgs that servers will pick up on the lan and reply to it. */
	vector<ServerInfo>	FindServers();

	/*! Returns true if you're the LAN host. */
	bool		IsServer() const {return this->mServer;}

	/*! Closes network. */
	void		Close();
};