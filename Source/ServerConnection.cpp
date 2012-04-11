#include "ServerConnection.h"
#include <iostream>
#include <string>
using namespace std;
ServerConnection::ServerConnection()
{
	WSADATA wsaData = {0};
	WSAStartup(MAKEWORD(2,2), &wsaData);
	this->mPort = 10000;
	this->mIp = "127.0.0.1";
	this->mServerSocket = Connection(socket( AF_INET, SOCK_STREAM, IPPROTO_IP ));
	this->mServer = false;

}
ServerConnection::~ServerConnection()
{
	closesocket(this->mServerSocket.sock);
	if(this->mServer)
		closesocket(this->mClientSocket[0].sock);
	WSACleanup();
}
void ServerConnection::SetIP(char ip[])
{
	this->mIp = ip;
}
void ServerConnection::SetPort(int port)
{
	this->mPort = port;
}
int ServerConnection::InitializeConnection()
{
	sockaddr_in saServer = {0};
	saServer.sin_family = PF_INET;
	saServer.sin_port = htons(this->mPort);
	saServer.sin_addr.s_addr = inet_addr(this->mIp);
	connect(this->mServerSocket.sock, (sockaddr*)&saServer, sizeof( sockaddr ));
	int a = ::WSAGetLastError();
	bool err = WSAEWOULDBLOCK == a;
	fd_set write;
	
	FD_ZERO(&write);
    FD_SET(this->mServerSocket.sock, &write);

    TIMEVAL timeOut;
	timeOut.tv_sec = 10;
	
	int res = select(0,  NULL, &write, NULL, &timeOut);

	if(res != 0)
	{
		this->mServer = false;
		if(FD_ISSET(this->mServerSocket.sock, &write))
        {
            //connected
        }

	}
	else
	{
		closesocket(this->mServerSocket.sock);
		this->mServerSocket.sock = socket( AF_INET, SOCK_STREAM, IPPROTO_IP );
		this->mServer = true;
		sockaddr_in saListen = {0};
		saListen.sin_family = PF_INET;
		saListen.sin_port = htons(this->mPort);
		saListen.sin_addr.s_addr = htonl(INADDR_ANY);
		bind(this->mServerSocket.sock, (sockaddr*)&saListen, sizeof(sockaddr));
		listen(this->mServerSocket.sock, 5);
		this->ListenForClient();
	}
	return 1;
}
int ServerConnection::ListenForClient()
{
	sockaddr_in saClient = {0};
	int nSALen = sizeof(sockaddr);
	int ret = 0;

	SOCKET sock = accept(this->mServerSocket.sock, (sockaddr*)&saClient, &nSALen );

	if(sock != INVALID_SOCKET)
	{
		ret = 1;
		this->mClientSocket.push_back(Connection(sock));
	}
	return ret;
}
void ServerConnection::Read()
{
	if(this->mServer)
	{
		int numBytes = recv(this->mClientSocket[0].sock, this->mClientSocket[0].buf + this->mClientSocket[0].numCharsInBuf, BUFFER_SIZE - this->mClientSocket[0].numCharsInBuf, 0);
		this->mClientSocket[0].numCharsInBuf += numBytes;
	}
	else
	{
		int numBytes = recv(this->mServerSocket.sock, this->mServerSocket.buf + this->mServerSocket.numCharsInBuf, BUFFER_SIZE - this->mServerSocket.numCharsInBuf, 0);
		this->mServerSocket.numCharsInBuf += numBytes;
	}
}
void ServerConnection::Write()
{
	if(this->mServer)
	{
		int numBytes = send(this->mClientSocket[0].sock, this->mClientSocket[0].bufW, this->mClientSocket[0].numCharsInBufW, 0 );
		if(numBytes != this->mClientSocket[0].numCharsInBufW) 
		{        
			this->mClientSocket[0].numCharsInBufW -= numBytes;
			memmove(this->mClientSocket[0].bufW, this->mClientSocket[0].bufW + numBytes, this->mClientSocket[0].numCharsInBufW);
		}
		else this->mClientSocket[0].numCharsInBufW = 0;
	}
	else
	{
		int numBytes = send(this->mServerSocket.sock, this->mServerSocket.bufW, this->mServerSocket.numCharsInBufW, 0 );
		if(numBytes != this->mServerSocket.numCharsInBufW) 
		{        
			this->mServerSocket.numCharsInBufW -= numBytes;
			memmove(this->mServerSocket.bufW, this->mServerSocket.bufW + numBytes, this->mServerSocket.numCharsInBufW);
		}
		else this->mServerSocket.numCharsInBufW = 0;
	}
}

void ServerConnection::SetupFDSets(fd_set& ReadFDs, fd_set& WriteFDs, fd_set& ExceptFDs, SOCKET ListeningSocket) 
{
    FD_ZERO(&ReadFDs);
    FD_ZERO(&WriteFDs);
    FD_ZERO(&ExceptFDs);

    
	if(this->mServer)
	{

		if (this->mClientSocket[0].numCharsInBuf < BUFFER_SIZE) 
		{
			FD_SET(this->mClientSocket[0].sock, &ReadFDs);
		}

		if (this->mClientSocket[0].numCharsInBufW > 0) 
		{
			FD_SET(this->mClientSocket[0].sock, &WriteFDs);
		}

		FD_SET(this->mClientSocket[0].sock, &ExceptFDs);
	}
	else
	{
		
		if (this->mServerSocket.numCharsInBuf < BUFFER_SIZE) 
		{
			FD_SET(this->mServerSocket.sock, &ReadFDs);
		}

		if (this->mServerSocket.numCharsInBufW > 0) 
		{
			FD_SET(this->mServerSocket.sock, &WriteFDs);
		}

		FD_SET(this->mServerSocket.sock, &ExceptFDs);
	}

}

void ServerConnection::Update()
{
	
	fd_set ReadFDs, WriteFDs, ExceptFDs;
	SetupFDSets(ReadFDs, WriteFDs, ExceptFDs, this->mServerSocket.sock);
	if(this->mServer)
	{
		if (select(0, &ReadFDs, &WriteFDs, NULL, 0) > 0) 
		{
			if (FD_ISSET(this->mClientSocket[0].sock, &ReadFDs)) 
			{
				Read();
				FD_CLR(this->mClientSocket[0].sock, &ReadFDs);
			}
			if (FD_ISSET(this->mClientSocket[0].sock, &WriteFDs)) 
			{
				Write();
				FD_CLR(this->mClientSocket[0].sock, &WriteFDs);
			}
		}
	}
	else
	{
		if (select(0, &ReadFDs, &WriteFDs, NULL, 0) > 0) 
		{
			if (FD_ISSET(this->mServerSocket.sock, &ReadFDs)) 
			{
				Read();
				FD_CLR(this->mServerSocket.sock, &ReadFDs);
			}
			if (FD_ISSET(this->mServerSocket.sock, &WriteFDs)) 
			{
				Write();
				FD_CLR(this->mServerSocket.sock, &WriteFDs);
			}
		}
		
	}

}

void ServerConnection::GetReadBuffer(char* bufOut, int size)
{
	if(this->mServer)
	{
		
		for(int i = 0; i < size; i++)
			bufOut[i] = this->mClientSocket[0].buf[i];
		this->mClientSocket[0].numCharsInBuf = 0;
	}
	else
	{
		
		for(int i = 0; i < size; i++)
			bufOut[i] = this->mServerSocket.buf[i];
		this->mServerSocket.numCharsInBuf = 0;
	}
}
void ServerConnection::SetWriteBuffer(char* buf, int size)
{
	if(this->mServer)
	{
		for(int i = 0; i < size; i++)
			this->mClientSocket[0].bufW[i] = buf[i];
		this->mClientSocket[0].numCharsInBufW = size;
	}
	else
	{
		for(int i = 0; i < size; i++)
			this->mServerSocket.bufW[i] = buf[i];
		this->mServerSocket.numCharsInBufW = size;
	}
}