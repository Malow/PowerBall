
#include "Connection.h"

Connection::Connection(string remoteIP, int remotePort, int localPort, int protocol)
{
	int type = SOCK_DGRAM;
	if(protocol == IPPROTO_IP)
		type = SOCK_STREAM;

	this->mSocket			= socket(AF_INET, type, protocol);

	this->mProtocol			= protocol;
	this->mIndex			= -1;
	this->mRunning			= false;

	this->mLocalAddress.sin_port = htons((u_short)localPort);
	this->mLocalAddress.sin_family = AF_INET;
	this->mLocalAddress.sin_addr.s_addr = INADDR_ANY;

	this->mRemoteAddress.sin_port = htons((u_short)remotePort);
	this->mRemoteAddress.sin_addr.s_addr = inet_addr(remoteIP.c_str());
	this->mRemoteAddress.sin_family = AF_INET;

	//this->SetBlocking(false);
}
Connection::Connection(sockaddr_in remoteAddress, int localPort, int protocol)
{
	int type = SOCK_DGRAM;
	if(protocol == IPPROTO_IP)
		type = SOCK_STREAM;

	this->mSocket			= socket(AF_INET, type, protocol);

	this->mProtocol			= protocol;
	this->mIndex			= -1;
	this->mRunning			= false;
	
	this->mLocalAddress.sin_port = htons((u_short)localPort);
	this->mLocalAddress.sin_family = AF_INET;
	this->mLocalAddress.sin_addr.s_addr = INADDR_ANY;

	this->mRemoteAddress = remoteAddress;
	this->mRemoteAddress.sin_family = AF_INET;

	//this->SetBlocking(false);
}

Connection::~Connection()
{
	closesocket(this->mSocket);
	this->mRunning = false;
}

int Connection::SetBlocking(bool blocking)
{
    u_long arg = 1; //arg == 1 -> NON-BLOCKING SOCKET Mode
	if(blocking)
		arg = 0;

	return ioctlsocket(this->mSocket, FIONBIO, &arg);
}
	
int	Connection::Start()
{
	int result = 0;
	if(this->mSocket != INVALID_SOCKET)
	{
		if(this->mProtocol == IPPROTO_IP)
		{
			result = connect(this->mSocket, (sockaddr*)&this->mRemoteAddress, sizeof( sockaddr ));
		}
		else 
		{
			result = bind(this->mSocket, (const sockaddr*)&this->mLocalAddress, sizeof(sockaddr_in));
		}
		this->mRunning = true;
	}
	else result = -1;
	return result;
}

	
int	Connection::Send(const char* buf, const int size)
{
	int result = 0;
	
	if(this->mRunning)
	{
		if(this->mProtocol == IPPROTO_IP && size > 0)
		{
			int sentBytes = send( this->mSocket, (char*)buf, size, 0);
			
			if(sentBytes != size)
				result = -1;
		}
		else if (this->mProtocol == IPPROTO_UDP && size > 0)
		{
			int sentBytes = sendto( this->mSocket, (char*)buf, size, 0, (sockaddr*)&this->mRemoteAddress, sizeof(sockaddr_in) );

			if(sentBytes != size)
				result = -1;
		}
		else result = -1;
	}
	else result = NOT_RUNNING; //connection not started

	return result;
}
int	Connection::Broadcast(const char* buf, const int size)
{
	if (this->mProtocol == IPPROTO_UDP)
	{
		char ac[80];
		gethostname(ac, sizeof(ac));
		hostent *local = gethostbyname(ac);
		string ip = "";
		BOOL broadcast = TRUE;
		for (int i = 0; local->h_addr_list[i] != 0; ++i) 
		{
			struct in_addr addr;
			memcpy(&addr, local->h_addr_list[i], sizeof(struct in_addr));
			ip = inet_ntoa(addr);

			u_long hostAddr = inet_addr(ip.c_str());   // local IP address
			u_long netMask = inet_addr("255.255.255.0");   // netmask
			u_long netAddr = hostAddr & netMask;
			u_long broadcastAddr = netAddr | (~netMask); //directional broadcast address (only broadcasting on the LAN subnet, otherwise routers usually trash the packets)

			setsockopt(this->mSocket, SOL_SOCKET, SO_BROADCAST, (char *)&broadcast, sizeof(broadcast));
	
	
			sockaddr_in address;
			address.sin_family = AF_INET;
			address.sin_addr.s_addr = broadcastAddr;
			address.sin_port = htons((u_short)CONNECT_PORT);

			sendto(this->mSocket, buf, size, 0, (sockaddr*)&address, sizeof(sockaddr_in) );
		}
	
		broadcast = FALSE;
		setsockopt(this->mSocket, SOL_SOCKET, SO_BROADCAST, (char *)&broadcast, sizeof(broadcast));
	}
	else return -1; //cannot broadcast @ tcp
	return 0;
}

int	Connection::Receive(char (&buf)[BUFFER_SIZE], sockaddr_in &from)
{
	int result = -1;
	if(this->mRunning)
	{
		int recvBytes = 0;
		int fromLength = sizeof( from );
		char temp[BUFFER_SIZE];
		if(this->mProtocol == IPPROTO_UDP)
		{
			fd_set readFD = {0};
			TIMEVAL time = {0};
			FD_SET(this->mSocket, &readFD);
			if(select(1, &readFD, NULL, NULL, &time) > 0)
			{
				recvBytes = recvfrom(this->mSocket, temp, BUFFER_SIZE, 0, (sockaddr*)&from, &fromLength);
				if(recvBytes > 0)
				{
					if(from.sin_addr.s_addr == this->mRemoteAddress.sin_addr.s_addr || this->mLocalAddress.sin_addr.s_addr == INADDR_ANY) //fix check so its from the correct sender
					{
						for(int i = 0; i < recvBytes; i++)
						{
							buf[i] = temp[i];
						}
						result = recvBytes;
					}
				}
				else result = recvBytes;
			}
			else result = 0;
		}
		else
		{
			recvBytes = recv(this->mSocket, temp, BUFFER_SIZE, 0);
			
			if(recvBytes > 0)
			{
				for(int i = 0; i < recvBytes; i++)
				{
					buf[i] = temp[i];
				}
				result = recvBytes;
			}
			else result = recvBytes;
		}
	}
	else result = NOT_RUNNING;
	return result;
}

void Connection::Stop()
{
	closesocket(this->mSocket);
	this->mSocket = INVALID_SOCKET;
	this->mRunning = false;
}

