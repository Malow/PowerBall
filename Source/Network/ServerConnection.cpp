#include "ServerConnection.h"

ServerConnection::ServerConnection()
{
	WSADATA wsaData = {0};
	WSAStartup(MAKEWORD(2,2), &wsaData);
	this->mPort = 10000;
	this->mServerSocket = new Connection(socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP ));
    u_long noBlocking = 1;
    ioctlsocket(this->mServerSocket->sock, FIONBIO, &noBlocking);

	this->mServer = false;
	this->mNumClients = 0;
}
ServerConnection::~ServerConnection()
{
	if(this->mServer)
	{
		//DWORD exitCode;
		//GetExitCodeThread(this->mServerSocket->handle, &exitCode);
		//ExitThread(exitCode);
		closesocket(this->mServerSocket->sock);
		this->mServerSocket->running = false;
		CloseHandle(this->mServerSocket->handle);
		for(int i = 0; i < this->mNumClients; i++)
		{
			//GetExitCodeThread(this->mClientSocket[i]->handle, &exitCode);
			//ExitThread(exitCode);
			this->mClientSocket[i]->running = false;
			closesocket(this->mClientSocket[i]->sock);
			CloseHandle(this->mClientSocket[i]->handle);
			SAFE_DELETE(this->mClientSocket[i]);
		}
	}
	else 
	{
		closesocket(this->mServerSocket->sock);
		this->mServerSocket->running = false;
		//CloseHandle(this->mServerSocket->handle);
	}
	WSACleanup();
	SAFE_DELETE(this->mServerSocket);
}
void ServerConnection::SetPort(const int port)
{
	this->mPort = port;
}
vector<ServerInfo> ServerConnection::FindServers()
{
	this->mServerInfos.clear();
	sockaddr_in adress = {0};
	adress.sin_family = AF_INET;
	adress.sin_port = htons((u_short)this->mPort);

	bind(this->mServerSocket->sock, (const sockaddr*)&adress, sizeof(sockaddr_in));

	DWORD nonBlocking = 1;
	ioctlsocket(this->mServerSocket->sock, FIONBIO, &nonBlocking);
	
    char ac[80];
    gethostname(ac, sizeof(ac));
    hostent *local = gethostbyname(ac);
    string ip = "";
	BOOL broadcast = TRUE;
    for (int i = 0; local->h_addr_list[i] != 0; ++i) 
	{
        struct in_addr addr;
        memcpy(&addr, local->h_addr_list[i], sizeof(struct in_addr));
        string ip = inet_ntoa(addr);

		u_long hostAddr = inet_addr(ip.c_str());   // local IP address
		u_long netMask = inet_addr("255.255.255.0");   // netmask
		u_long netAddr = hostAddr & netMask;
		u_long broadcastAddr = netAddr | (~netMask); //directional broadcast address (only broadcasting on the LAN subnet)

		setsockopt(this->mServerSocket->sock, SOL_SOCKET, SO_BROADCAST, (char *)&broadcast, sizeof(broadcast));
	
		char buffer[10] = "HI";
	
		sockaddr_in address;
		address.sin_family = AF_INET;
		address.sin_addr.s_addr = broadcastAddr;
		address.sin_port = htons((u_short)this->mPort);

		sendto( this->mServerSocket->sock, buffer, sizeof(buffer), 0, (sockaddr*)&address, sizeof(sockaddr_in) );
    }



	
	broadcast = FALSE;

	setsockopt(this->mServerSocket->sock, SOL_SOCKET, SO_BROADCAST, (char *)&broadcast, sizeof(broadcast));

	
	sockaddr_in from;
	int fromLength = sizeof( from );
	for(int i = 0; i < 10; i++)
	{
		char buf[BUFFER_SIZE];
		int recvBytes = 0;
		recvBytes = recvfrom(this->mServerSocket->sock, buf, sizeof(buf), 0, (sockaddr*)&from, &fromLength );

		if(recvBytes > 0)
		if(buf[0] == 'H' && buf[1] == 'E' && buf[2] == 'Y')
		{
			int offset = 3;
			ServerInfo temp(buf, offset);
			temp.SetIP(inet_ntoa(from.sin_addr));
			this->mServerInfos.push_back( temp );
			//this->mIp = inet_ntoa(from.sin_addr);
		}

		Sleep(100);
	}
	return this->mServerInfos;
}
void ServerConnection::Host(ServerInfo server)
{		
	this->mCurrentServer = server;
	this->mServer = true;
	sockaddr_in adress = {0};
	adress.sin_family = AF_INET;
	adress.sin_addr.s_addr = htonl(INADDR_ANY);
	adress.sin_port = htons((u_short)this->mPort);

	bind(this->mServerSocket->sock, (const sockaddr*)&adress, sizeof(sockaddr_in));

	this->mServerSocket->handle = CreateThread(0, 0, &ListenForClient, (void*) this, 0, 0);
}
void ServerConnection::Connect(ServerInfo server)
{
	this->mCurrentServer = server;
	char buffer[10] = "START";
	sockaddr_in adress = {0};
	adress.sin_family = AF_INET;
	adress.sin_port = htons((u_short)this->mPort);
	adress.sin_addr.s_addr = inet_addr(server.GetIP().c_str());

	sendto( this->mServerSocket->sock, buffer, sizeof(buffer), 0, (sockaddr*)&adress, sizeof(sockaddr_in) );
	
	char buf[BUFFER_SIZE];
	sockaddr_in from;
	int fromLength = sizeof( from );
	
	int recvBytes = 0;
	while(recvBytes <= 0)
	{
		recvBytes = recvfrom(this->mServerSocket->sock, buf, sizeof(buf), 0, (sockaddr*)&from, &fromLength );
		int a = ::WSAGetLastError();
	}
	this->mServerSocket->adress = from;
	//bind(this->mServerSocket->sock, (const sockaddr*)&from, sizeof(from));

	//DWORD nonBlocking = 1;
	//ioctlsocket(this->mServerSocket->sock, FIONBIO, &nonBlocking);


	this->mServerSocket->handle = CreateThread(0, 0, &CommunicateClient, (void*) this->mServerSocket, 0, 0) ;
}
DWORD WINAPI ServerConnection::ListenForClient(void* param)
{
	ServerConnection* sc = (ServerConnection*) param;
	while(sc->mServerSocket->running)
	{
		char buf[BUFFER_SIZE];
		sockaddr_in from;
		int fromLength = sizeof( from );

		int recvBytes = recvfrom(sc->mServerSocket->sock, buf, sizeof(buf), 0, (sockaddr*)&from, &fromLength );
		
		if(recvBytes > 0)
		{
			if(buf[0] == 'H' && buf[1] == 'I') //incoming broadcast => client wants to find you
			{
				sc->mCurrentServer.SetNumPlayers(sc->GetNumConnections());
				int offset = 3;
				char buffer[BUFFER_SIZE] = "HEY";
				sc->mCurrentServer.GetBuffer(buffer, offset);
				sendto(sc->mServerSocket->sock, buffer, sizeof(buffer), 0, (sockaddr*)&from, sizeof(sockaddr_in));
			}
			else if(buf[0] == 'S')
			{
				bool reconnectedClient = false;
				Connection* client = NULL;
				for(int i = 0; i < sc->mClientSocket.size(); i++)
				{
					if(inet_ntoa(from.sin_addr) == inet_ntoa(sc->mClientSocket[i]->adress.sin_addr))
					{
						//reconnection from client
						reconnectedClient = true;
						client = sc->mClientSocket[i];
					}
				}
				if(!reconnectedClient && sc->GetNumConnections() < sc->mCurrentServer.GetMaxNumPlayers())
				{
					SOCKET sock = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
					sc->mPort++;
					sockaddr_in newSockAdress = from;
					newSockAdress.sin_family = AF_INET;
					newSockAdress.sin_port = htons((u_short) sc->mPort);

					bind(sock, (const sockaddr*)&newSockAdress, sizeof(sockaddr_in));

					DWORD nonBlocking = 1;
					ioctlsocket(sock, FIONBIO, &nonBlocking);

					client = new Connection(sock);
					sc->mClientSocket.push_back(client);
					sc->mNumClients++;
			
					char buffer[10] = "START";
					sendto( client->sock, buffer, sizeof(buffer), 0, (sockaddr*)&from, sizeof(sockaddr_in));

					client->adress = from;
					client->index = sc->GetNumConnections() - 1;
					client->handle = CreateThread(0, 0, &Communicate, (void*) client, 0, 0);
				}
				else if (reconnectedClient)
				{
					client->running = false;
					//char buffer[10] = "START";
					
					//sendto( client->sock, buffer, sizeof(buffer), 0, (sockaddr*)&client->adress, sizeof(sockaddr_in));

					
					SOCKET sock = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
					sockaddr_in newSockAdress = from;
					newSockAdress.sin_family = AF_INET;
					newSockAdress.sin_port =  client->adress.sin_port;

					bind(sock, (const sockaddr*)&newSockAdress, sizeof(sockaddr_in));

					DWORD nonBlocking = 1;
					ioctlsocket(sock, FIONBIO, &nonBlocking);
					
					int index = client->index;
					delete client;
					client = new Connection(sock);
					sc->mClientSocket[index - 1] = client;
			
					char buffer[10] = "START";
					sendto( client->sock, buffer, sizeof(buffer), 0, (sockaddr*)&from, sizeof(sockaddr_in));

					client->adress = from;
					client->index = index;
					client->handle = CreateThread(0, 0, &Communicate, (void*) client, 0, 0);
					MsgHandler::GetInstance().PlayerReconnect(index);
				}
				else
				{
					//char buffer[10] = "FULL";
					//sendto( client->sock, buffer, sizeof(buffer), 0, (sockaddr*)&from, sizeof(sockaddr_in));
				}
			}
		
		}
		
		//if(sc->GetNumConnections() >= sc->mCurrentServer.GetMaxNumPlayers())
			//sc->mServerSocket->running = false;
	}
	return 0;
}
bool ServerConnection::GetReadBuffer(char* bufOut, const int size, const int clientIndex)
{ 
	bool somethingToRead = false;
	if(this->mServer)
	{
		if(clientIndex >= 0 && clientIndex < this->mNumClients)
		{
			if(this->mClientSocket[clientIndex]->numCharsInBuf > 0)
			{
				for(int i = 0; i < size; i++)
					bufOut[i] = this->mClientSocket[clientIndex]->buf[i];
				this->mClientSocket[clientIndex]->numCharsInBuf = 0;
				somethingToRead = true;
			}
		}
	}
	else
	{
		if(this->mServerSocket->numCharsInBuf > 0)
		{
			for(int i = 0; i < size; i++)
				bufOut[i] = this->mServerSocket->buf[i];
			this->mServerSocket->numCharsInBuf = 0;
			somethingToRead = true;
		}
	}
	return somethingToRead;
}
void ServerConnection::SetWriteBuffer(const char* buf, const int size, const int clientIndex)
{
	if(this->mServer)
	{
		if(clientIndex >= 0 && clientIndex < this->mNumClients)
		{
			for(int i = 0; i < size; i++)
				this->mClientSocket[clientIndex]->bufW[this->mClientSocket[clientIndex]->numCharsInBufW + i] = buf[i];
			this->mClientSocket[clientIndex]->numCharsInBufW += size;
		}
	}
	else
	{
		for(int i = 0; i < size; i++)
			this->mServerSocket->bufW[this->mServerSocket->numCharsInBufW + i] = buf[i];
		this->mServerSocket->numCharsInBufW += size;
	}
}
DWORD WINAPI ServerConnection::Communicate(void* param)
{
	Connection* conn = (Connection*)param;
    u_long noBlocking = 0;
    ioctlsocket(conn->sock, FIONBIO, &noBlocking);
	while(conn->running)
	{
		int recvBytes = recvfrom(conn->sock, (char*)conn->buf, sizeof(conn->buf), 0, 0, 0);
		if(recvBytes > 0)
		{
			conn->numCharsInBuf = recvBytes;
			MsgHandler::GetInstance().ProcessMSG(conn->buf, recvBytes, conn->index);
		}
		
		//::Sleep(50 - 5);	
	}

	return 0;
}
DWORD WINAPI ServerConnection::CommunicateClient(void* param)
{
	Connection* conn = (Connection*)param;
    u_long noBlocking = 0;
    ioctlsocket(conn->sock, FIONBIO, &noBlocking);
	while(conn->running)
	{
		int recvBytes = recvfrom(conn->sock, (char*)conn->buf, sizeof(conn->buf), 0, 0, 0);
		if(recvBytes > 0)
		{
			conn->numCharsInBuf = recvBytes;
			MsgHandler::GetInstance().ProcessMSG(conn->buf, recvBytes, conn->index);
		}
		
		//::Sleep(25 - 5);	
	}

	return 0;
}
void ServerConnection::Send(int index)
{
	
	Connection* conn = this->mServerSocket;
	if(this->mServer)
		conn = this->mClientSocket[index];

	if(conn->running)
	{
   // u_long noBlocking = 1;
    //ioctlsocket(conn->sock, FIONBIO, &noBlocking);
		if(conn->numCharsInBufW > 0)
		{
			int sentBytes = sendto( conn->sock, (char*)conn->bufW, conn->numCharsInBufW, 0, (sockaddr*)&conn->adress, sizeof(sockaddr_in) );
			
			if(sentBytes > 0)
				conn->numCharsInBufW -= sentBytes;
		}
	}
    //noBlocking = 0;
    //ioctlsocket(conn->sock, FIONBIO, &noBlocking);
}
void ServerConnection::Close()
{
	this->mServerSocket->running = false;
	closesocket(this->mServerSocket->sock);
	this->mServerSocket->sock = INVALID_SOCKET;
	if(this->mServer)
	{
		for(int i = 0; i < this->mNumClients; i++)
		{
			this->mClientSocket[i]->running = false;
			closesocket(this->mClientSocket[i]->sock);
			this->mClientSocket[i]->sock = INVALID_SOCKET;
		}
	}
	WSACleanup();
}