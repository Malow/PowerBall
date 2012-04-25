#include "ServerConnection.h"

volatile bool running = true;
ServerConnection::ServerConnection()
{
	WSADATA wsaData = {0};
	WSAStartup(MAKEWORD(2,2), &wsaData);
	this->mPort = 10000;
	this->mServerSocket = new Connection(socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP ));
	running = true;
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
		running = false;
		CloseHandle(this->mServerSocket->handle);
		for(int i = 0; i < this->mNumClients; i++)
		{
			//GetExitCodeThread(this->mClientSocket[i]->handle, &exitCode);
			//ExitThread(exitCode);
			closesocket(this->mClientSocket[i]->sock);
			CloseHandle(this->mClientSocket[i]->handle);
			SAFE_DELETE(this->mClientSocket[i]);
		}
	}
	else closesocket(this->mServerSocket->sock);
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

	u_long hostAddr = inet_addr("79.138.27.63");   // local IP address
    u_long netMask = inet_addr("255.255.255.0");   // LAN netmask
    u_long netAddr = hostAddr & netMask;
    u_long broadcastAddr = netAddr | (~netMask); //directional broadcast address (only broadcasting on the LAN subnet)

	BOOL broadcast = TRUE;

	setsockopt(this->mServerSocket->sock, SOL_SOCKET, SO_BROADCAST, (char *)&broadcast, sizeof(broadcast));
	
	char buffer[10] = "HI";
	
	sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = broadcastAddr;
	address.sin_port = htons(this->mPort);

	sendto( this->mServerSocket->sock, buffer, sizeof(buffer), 0, (sockaddr*)&address, sizeof(sockaddr_in) );

	
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

	//DWORD nonBlocking = 1;
	//ioctlsocket(this->mServerSocket->sock, FIONBIO, &nonBlocking);

	/*
	unsigned char packet_data[256];
    unsigned int maximum_packet_size = sizeof( packet_data );
	sockaddr_in from;
    int fromLength = sizeof( from );

	while(true)
	{
		int received_bytes = recvfrom(this->mServerSocket->sock, (char*)packet_data, maximum_packet_size,
                                   0, (sockaddr*)&from, &fromLength );
	}*/

	this->mServerSocket->handle = CreateThread(0, 0, &ListenForClient, (void*) this, 0, 0);
}
void ServerConnection::Connect(ServerInfo server)
{
	this->mCurrentServer = server;
	char buffer[10] = "START";
	sockaddr_in adress = {0};
	adress.sin_family = AF_INET;
	adress.sin_port = htons((u_short)this->mPort);
	adress.sin_addr.s_addr = inet_addr(server.GetIP().c_str()); //om sätts innan bind så skickas det till en själv :P

	sendto( this->mServerSocket->sock, buffer, sizeof(buffer), 0, (sockaddr*)&adress, sizeof(sockaddr_in) );
	
	char buf[BUFFER_SIZE];
	sockaddr_in from;
	int fromLength = sizeof( from );
	
	int recvBytes = 0;
	while(recvBytes <= 0)
	{
		recvBytes = recvfrom(this->mServerSocket->sock, buf, sizeof(buf), 0, (sockaddr*)&from, &fromLength );
	}
	this->mServerSocket->adress = from;
	//bind(this->mServerSocket->sock, (const sockaddr*)&from, sizeof(from));

	//DWORD nonBlocking = 1;
	//ioctlsocket(this->mServerSocket->sock, FIONBIO, &nonBlocking);


	CreateThread(0, 0, &TalkToClient, (void*) this->mServerSocket, 0, 0) ;
}
DWORD WINAPI ServerConnection::ListenForClient(void* param)
{
	ServerConnection* sc = (ServerConnection*) param;
	while(running)
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
				SOCKET sock = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
				sc->mPort++;
				sockaddr_in newSockAdress = from;
				newSockAdress.sin_family = AF_INET;
				newSockAdress.sin_port = htons((u_short) sc->mPort);

				bind(sock, (const sockaddr*)&newSockAdress, sizeof(sockaddr_in));

				DWORD nonBlocking = 1;
				ioctlsocket(sock, FIONBIO, &nonBlocking);

				Connection* client = new Connection(sock);
				sc->mClientSocket.push_back(client);
				sc->mNumClients++;
			
				char buffer[10] = "START";
				sendto( client->sock, buffer, sizeof(buffer), 0, (sockaddr*)&from, sizeof(sockaddr_in));

				client->adress = from;
				client->handle = CreateThread(0, 0, &TalkToClient, (void*) client, 0, 0);
			}
		
		}
	}
	return 0;
}
void ServerConnection::Read()
{
	if(!this->mServer)
	{
		int numBytes = recv(this->mServerSocket->sock, this->mServerSocket->buf + this->mServerSocket->numCharsInBuf, BUFFER_SIZE - this->mServerSocket->numCharsInBuf, 0);
		this->mServerSocket->numCharsInBuf += numBytes;
	}
}
void ServerConnection::Write()
{
	if(!this->mServer)
	{
		int numBytes = send(this->mServerSocket->sock, this->mServerSocket->bufW, this->mServerSocket->numCharsInBufW, 0 );
		if(numBytes != this->mServerSocket->numCharsInBufW) 
		{        
			this->mServerSocket->numCharsInBufW -= numBytes;
			memmove(this->mServerSocket->bufW, this->mServerSocket->bufW + numBytes, this->mServerSocket->numCharsInBufW);
		}
		else this->mServerSocket->numCharsInBufW = 0;
	}
}

void ServerConnection::SetupFDSets(fd_set& ReadFDs, fd_set& WriteFDs, fd_set& ExceptFDs) 
{
	if(!this->mServer)
	{
		FD_ZERO(&ReadFDs);
		FD_ZERO(&WriteFDs);
		FD_ZERO(&ExceptFDs);

		if (this->mServerSocket->numCharsInBuf < BUFFER_SIZE) 
		{
			FD_SET(this->mServerSocket->sock, &ReadFDs);
		}

		if (this->mServerSocket->numCharsInBufW > 0) 
		{
			FD_SET(this->mServerSocket->sock, &WriteFDs);
		}

		FD_SET(this->mServerSocket->sock, &ExceptFDs);
	}
}

bool ServerConnection::Update()
{
	int a = WSAGetLastError();
	if(a == WSAECONNRESET) //connection lost
	{
		return false;
	}
	if(!this->mServer)
	{
		fd_set ReadFDs, WriteFDs, ExceptFDs;
		SetupFDSets(ReadFDs, WriteFDs, ExceptFDs);

		TIMEVAL timeOut;
		timeOut.tv_sec = 0;
		if (select(0, &ReadFDs, &WriteFDs, NULL, 0) > 0) 
		{
			if (FD_ISSET(this->mServerSocket->sock, &ReadFDs)) 
			{
				Read();
				FD_CLR(this->mServerSocket->sock, &ReadFDs);
			}
			if (FD_ISSET(this->mServerSocket->sock, &WriteFDs)) 
			{
				Write();
				FD_CLR(this->mServerSocket->sock, &WriteFDs);
			}
		}
	}
	return true;
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
				this->mClientSocket[clientIndex]->bufW[i] = buf[i];
			this->mClientSocket[clientIndex]->numCharsInBufW = size;
		}
	}
	else
	{
		for(int i = 0; i < size; i++)
			this->mServerSocket->bufW[i] = buf[i];
		this->mServerSocket->numCharsInBufW = size;
	}
}
DWORD WINAPI ServerConnection::TalkToClient(void* param)
{
	Connection* conn = (Connection*)param;
	while(running)
	{
		for(int i = 0; i < 2; i++)
		{
			int recvBytes = recvfrom(conn->sock, (char*)conn->buf, sizeof(conn->buf), 0, 0, 0);
			if(recvBytes > 0)
				conn->numCharsInBuf = recvBytes;
		}

		if(conn->numCharsInBufW > 0)
		{
			int sentBytes = sendto( conn->sock, (char*)conn->bufW, conn->numCharsInBufW, 0, (sockaddr*)&conn->adress, sizeof(sockaddr_in) );
			
			if(sentBytes > 0)
				conn->numCharsInBufW = 0;
		}
		::Sleep(50);	
	}

	return 0;
}
DWORD WINAPI ServerConnection::TalkToServer(void* param)
{
	Connection* conn = (Connection*)param;
	while(running)
	{
		int numBytes = recv(conn->sock, conn->buf + conn->numCharsInBuf, BUFFER_SIZE - conn->numCharsInBuf, 0);
		conn->numCharsInBuf += numBytes;

		if(conn->numCharsInBufW > 0 && conn->sock != INVALID_SOCKET)
		{
			numBytes = 0;
		
			numBytes = send(conn->sock, conn->bufW, conn->numCharsInBufW, 0 );
			if(numBytes != conn->numCharsInBufW  && conn->sock != INVALID_SOCKET) 
			{        
				conn->numCharsInBufW -= numBytes;
				memmove(conn->bufW, conn->bufW + numBytes, conn->numCharsInBufW);
			}
			else conn->numCharsInBufW = 0;
		}
		
	}
	return 0;
}
DWORD WINAPI ServerConnection::ReadFromServer(void* param)
{
	Connection* conn = (Connection*)param;
	while(running)
	{
		int numBytes = recvfrom(conn->sock, conn->buf + conn->numCharsInBuf, BUFFER_SIZE - conn->numCharsInBuf, 0, 0, 0);
		conn->numCharsInBuf += numBytes;	
	}
	return 0;
}
DWORD WINAPI ServerConnection::WriteToServer(void* param)
{
	Connection* conn = (Connection*)param;
	while(running)
	{
		if(conn->numCharsInBufW > 0 && conn->sock != INVALID_SOCKET)
		{
			int numBytes = 0;
		
			numBytes = sendto(conn->sock, conn->bufW, conn->numCharsInBufW, 0, 0, 0);
			if(numBytes != conn->numCharsInBufW  && conn->sock != INVALID_SOCKET) 
			{        
				conn->numCharsInBufW -= numBytes;
				memmove(conn->bufW, conn->bufW + numBytes, conn->numCharsInBufW);
			}
			else conn->numCharsInBufW = 0;
		}
		
	}
	return 0;
}
void ServerConnection::Close()
{
	running = false;
	closesocket(this->mServerSocket->sock);
	this->mServerSocket->sock = INVALID_SOCKET;
	if(this->mServer)
	{
		for(int i = 0; i < this->mNumClients; i++)
		{
			closesocket(this->mClientSocket[i]->sock);
			this->mClientSocket[i]->sock = INVALID_SOCKET;
		}
	}
	WSACleanup();
}