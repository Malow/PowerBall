#include "NetworkHandler.h"

NetworkHandler::NetworkHandler()
{
	WSADATA wsaData = {0};
	WSAStartup(MAKEWORD(2,2), &wsaData);
	this->mServer = false;
}
NetworkHandler::~NetworkHandler()
{
	for(int i = 0; i < this->mConnections.size(); i++)
	{
		this->mConnections[i]->Stop();
		Sleep(20);
		delete this->mConnections[i];
	}
	WSACleanup();
}
vector<ServerInfo> NetworkHandler::FindServers()
{
	this->mServerInfos.clear();
	Connection* temp = new Connection("", -1, CONNECT_PORT);
	temp->Start();
	char buf[3] = "HI";
	temp->Broadcast(buf, sizeof(buf));
	
	char buffer[BUFFER_SIZE];
	sockaddr_in from;
	for(int i = 0; i < 10; i++)
	{
		if(temp->Receive(buffer, from) > 0)
		if(buffer[0] == 'H' && buffer[1] == 'E' && buffer[2] == 'Y')
		{
			int offset = 3;
			ServerInfo si(buffer, offset);
			si.SetIP(inet_ntoa(from.sin_addr));
			this->mServerInfos.push_back(si);
		}

		Sleep(100);
	}
	temp->Stop();
	delete temp;
	return this->mServerInfos;
}
ServerInfo	NetworkHandler::ConnectTo(string ip)
{
	if(this->mConnections.size() > 0)
	{
		for(int i = 0; i < this->mConnections.size(); i++)
			delete this->mConnections[i];
		this->mConnections.clear();
	}

	this->mCurrentServer = ServerInfo();
	char recvResponse[BUFFER_SIZE];

	Connection* temp = new Connection(ip, CONNECT_PORT);
	temp->Start();
	
	char requestServerInfo[3] = "HI";
	temp->Send(requestServerInfo, sizeof(requestServerInfo));


	sockaddr_in from;
	for(int i = 0; i < 10; i++)
	{
		if(temp->Receive(recvResponse, from) > 0)
		if(recvResponse[0] == 'H' && recvResponse[1] == 'E' && recvResponse[2] == 'Y')
		{
			int offset = 3;
			this->mCurrentServer = ServerInfo(recvResponse, offset);
			this->mCurrentServer.SetIP(inet_ntoa(from.sin_addr));
			i = 15;
		}

		Sleep(100);
	}

	if(this->mCurrentServer.GetPort() > 0)
	{
		char start[6] = "START";
		temp->Send(start, sizeof(start));

		sockaddr_in from;
		int recvBytes = 0;
		for(int i = 0; i < 10; i++)
		{
			recvBytes = temp->Receive(recvResponse, from);
			if(recvResponse[0] == 'S' && recvResponse[1] == 'T')
				i = 15;

			Sleep(50);
		}
		if(recvResponse[0] == 'S' && recvResponse[1] == 'T')
		{
			temp->SetRemoteAddress(from);
			CreateThread(0, 0, &CommunicateClient, (void*) temp, 0, 0);
			this->mConnections.push_back(temp);
		}
	}
	temp->SetIndex(0);

	return this->mCurrentServer;
}
void NetworkHandler::Host(ServerInfo server)
{		
	this->mCurrentServer = server;
	this->mServer = true;
	Connection* temp = new Connection("", -1, CONNECT_PORT); //"" remote ip and -1 remote port indicates that it wont send anything, only listen for new clients.
	temp->SetIndex(0);
	int res = temp->Start();
	this->mConnections.push_back(temp);

	CreateThread(0, 0, &ListenForClient, (void*) this, 0, 0);
}
bool NetworkHandler::Connect(ServerInfo server)
{
	for(int i = 0; i < this->mConnections.size(); i++)
		delete this->mConnections[i];
	this->mConnections.clear();

	this->mCurrentServer = server;
	char buffer[6] = "START";
	
	Connection* temp = new Connection(server.GetIP(), server.GetPort());
	temp->Start();
	temp->Send(buffer, sizeof(buffer));

	char buf[BUFFER_SIZE];
	sockaddr_in from;
	int recvBytes = 0;
	int i = 0;
	for(i = 0; i < 10; i++)
	{
		recvBytes = temp->Receive(buf, from);
		if(buf[0] == 'S' && buf[1] == 'T')
			i = 15;

		Sleep(50);
	}
	if(i < 15)
		return false;

	temp->SetRemoteAddress(from);
	temp->SetIndex(0);
	CreateThread(0, 0, &CommunicateClient, (void*) temp, 0, 0);
	this->mConnections.push_back(temp);

	return true;
}
void NetworkHandler::DropPlayer(int index)
{
	if(index > 0 && index < this->mConnections.size())
	{
		this->mConnections[index]->Stop();
		Sleep(50);
		delete this->mConnections[index];
		this->mConnections[index] = this->mConnections.back();
		this->mConnections[index]->SetIndex(index);
		this->mConnections.back() = NULL;
		this->mConnections.pop_back();
	}
}
DWORD WINAPI NetworkHandler::ListenForClient(void* param)
{
	NetworkHandler* sc = (NetworkHandler*) param;
	Connection* newCon = NULL;
	while(sc->mConnections[0]->IsRunning())
	{
		char buf[BUFFER_SIZE];
		sockaddr_in from = {0};
		
		if(sc->mConnections[0]->Receive(buf, from) > 0)
		{
			newCon = new Connection(from);
			newCon->Start();
			if(buf[0] == 'H' && buf[1] == 'I') //incoming broadcast => client wants to find you, send her server info
			{
				int offset = 3;
				char buffer[BUFFER_SIZE] = "HEY";
				sc->mCurrentServer.GetBuffer(buffer, offset); //send serverinfo
				newCon->Send(buffer, offset);
				newCon->Stop();
				delete newCon;
			}
			else if(buf[0] == 'S')
			{
				bool reconnectedClient = false;
				/*for(int i = 1; i < sc->mConnections.size(); i++) //check if its a reconnected client
				{
					if(inet_ntoa(from.sin_addr) == sc->mConnections[i]->GetRemoteIP())
					{
						sc->mConnections[i]->Stop();
						delete sc->mConnections[i];
						sc->mConnections[i] = newCon;
			
						char buffer[6] = "START";
						newCon->Send(buffer, sizeof(buffer));
						newCon->SetIndex(i);
						CreateThread(0, 0, &Communicate, (void*) newCon, 0, 0);

						MsgHandler::GetInstance().PlayerReconnect(i);

						reconnectedClient = true;
					}
				}*/
				if(!reconnectedClient && sc->GetNumConnections() < sc->mCurrentServer.GetMaxNumPlayers()) //If its not a reconnected player then add him if theres room.
				{
					sc->mConnections.push_back(newCon);
					char buffer[6] = "START";
					newCon->Send(buffer, sizeof(buffer));
					newCon->SetIndex(sc->mConnections.size() - 1);
					CreateThread(0, 0, &Communicate, (void*) newCon, 0, 0);

					sc->mCurrentServer.SetNumPlayers(sc->GetNumConnections());
				}
				else if (!reconnectedClient) //server is full.
				{
					char buffer[5] = "FULL";
					newCon->Send(buffer, sizeof(buffer));
					newCon->Stop();
					delete newCon;
				}
			}

		}
	}
	return 0;
}
DWORD WINAPI NetworkHandler::Communicate(void* param)
{
	Connection* conn = (Connection*)param;
	conn->SetBlocking(true);
	char buf[BUFFER_SIZE];
	while(conn->IsRunning())
	{
		int recvBytes = conn->Receive(buf);
		if(recvBytes > 0) //maybe while loop here and read all packets and then sleep thread
		{
			MsgHandler::GetInstance().ProcessMSG(buf, recvBytes, conn->GetIndex());
		}
		else if(recvBytes == 0)
			Sleep(10);
	}

	return 0;
}
DWORD WINAPI NetworkHandler::CommunicateClient(void* param)
{
	Connection* conn = (Connection*)param;
	conn->SetBlocking(true);
	char buf[BUFFER_SIZE];
	while(conn->IsRunning())
	{
		int recvBytes = conn->Receive(buf);
		if(recvBytes > 0)
		{
			MsgHandler::GetInstance().ProcessMSG(buf, recvBytes, conn->GetIndex());
		}
		else if(recvBytes == 0)
			Sleep(10);
	}

	return 0;
}
void NetworkHandler::ProcessMSGs()
{	/*
	int i = 0;
	if(this->IsServer())
		 i =  1;
	for(; i < this->mConnections.size(); i++)
	{
		Connection* conn = this->mConnections[i];
		//conn->SetBlocking(true);
		char buf[BUFFER_SIZE];
		int recvBytes = 1;
		while(recvBytes > 0)
		{
			recvBytes = conn->Receive(buf);
			if(recvBytes > 0) //maybe while loop here and read all packets and then sleep thread
			{
				MsgHandler::GetInstance().ProcessMSG(buf, recvBytes, conn->GetIndex());
			}
		
			//::Sleep(50 - 5);	
		}
	}*/
}
void NetworkHandler::Send(const char* buf, const int size, const int clientIndex)
{
	this->mConnections[clientIndex]->Send(buf, size);
}
void NetworkHandler::Close()
{
	for(int i = 0; i < this->mConnections.size(); i++)
	{
		this->mConnections[i]->Stop();
	}
}