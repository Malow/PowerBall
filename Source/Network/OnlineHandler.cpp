#include "OnlineHandler.h"

OnlineHandler::OnlineHandler()
{
	WSADATA wsaData = {0};
	WSAStartup(MAKEWORD(2,2), &wsaData);
	this->mConnection = new Connection("88.129.206.221", CONNECT_PORT, 0, IPPROTO_IP);
	this->mConnection->SetBlocking(true);
}
OnlineHandler::~OnlineHandler()
{
	this->mConnection->Stop();
	Sleep(20);
	delete this->mConnection;
	WSACleanup();
}
vector<ServerInfo> OnlineHandler::FindServers()
{
	this->mServerInfos.clear();
	char buffer[17] = "GET SERVER LIST";
	buffer[16] = 10;
	int asd = this->mConnection->Send(buffer, 17);
	Sleep(500);
	return this->mServerInfos;
}
bool CSC(char* buf, string text)
{
	bool same = true;
	for(int i = 0; i < text.size(); i++)
	{
		if(buf[i] != text[i])
			same = false;
	}
	return same;
}
bool OnlineHandler::Connect(string accName, string accPass)
{
	if (this->mConnection->Start() != 0)
	{
		return false;
	}
	char bufRecv[1024] = {0}; 
	char bufSend[1024];  
	int offset = 0;

	this->mConnection->Receive(bufRecv); //rec = AUTH URSELF
	if(!CSC(bufRecv, "AUTH YOURSELF"))
		return false;

	::AddToBuf(bufSend, offset, accName);
	offset--;
	::AddToBuf(bufSend, offset, ' ');
	::AddToBuf(bufSend, offset, accPass);
	offset--;
	::AddToBuf(bufSend, offset, 10); //gotta add a 10 @ the end.

	int ret = this->mConnection->Send(bufSend, offset);
	
	this->mConnection->Receive(bufRecv);
	
	if(!CSC(bufRecv, "AUTH SUCCESSFUL"))
		return false;

	this->mConnection->Receive(bufRecv);
	
	if(!CSC(bufRecv, "NOW IN LOBBY"))
		return false;

	CreateThread(0, 0, &Communicate, (void*) this, 0, 0); //ENTERED LOBBY @ SERVER -> CONNECTION SUCCESSFUL -> START THE COMMUNICATION

	return true;
}
DWORD WINAPI OnlineHandler::Communicate(void* param)
{
	OnlineHandler* oh = (OnlineHandler*)param;
	Connection* conn = oh->mConnection;
	char buf[BUFFER_SIZE];
	while(conn->IsRunning())
	{
		int recvBytes = conn->Receive(buf);
		if(recvBytes > 0)
		{
			if(CSC(buf, "ALIVE CHECK"))
			{
				char alive[] = "I AM ALIVE";
				conn->Send(alive, sizeof(alive));
			}
			else if(CSC(buf, "SERVER:"))
			{
				int offset = 7;
				int gameID = GetFromBufferI(buf, offset);
				ServerInfo si(buf, offset);
				si.SetID(gameID);
				oh->mServerInfos.push_back(si);
			}
			else MsgHandler::GetInstance().ProcessMSG(buf, recvBytes, conn->GetIndex());
		}
		
		//::Sleep(50 - 5);	
	}

	return 0;
}
void OnlineHandler::Send(const char* buf, const int size, const int clientIndex)
{
	this->mConnection->Send(buf, size);
}
void OnlineHandler::Close()
{
	this->mConnection->Stop();
}