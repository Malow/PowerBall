#include "MsgHandler.h"
#include "GameNetwork.h"
//#include "Lobby.h"
MsgHandler::~MsgHandler()
{
}
void MsgHandler::Set(GameNetwork* gn, NetworkHandler*	conn)//, Lobby* lobby)
{
	this->mConn = conn;
	this->mNet = gn;
	//this->mLobby = lobby;
}
void MsgHandler::ProcessMSG(char* buffer, int size, int index)
{
	int offset = 0;
	while(offset < size)
	{
		offset += sizeof(char);
		switch(buffer[offset - sizeof(char)])
		{
		case 1:
			break;
		case 2:
			this->ReturnPing(index);
			break;
		case 3:
			this->ReceivePing(index);
			break;
		case 4:
			break;
		case 5:
			this->Close();
			break;
		case 6:
			this->ReceiveClientData(buffer, offset, index);
			break;
		case 7:
			this->ReceiveServerData(buffer, offset);
			break;

		case 8:
			this->ReceiveIdentification(buffer, offset, index);
			break;
		case 9:
			this->ReceivePlayerInfos(buffer, offset);
			break;
		case 10:
			//this->SendPlayerProfile();
			break;
		case 11:
			this->ReceiveTeamChange(buffer, offset, index);
			break;
		case 12:
			this->ReceiveCastSpell(buffer, offset, index);
			break;
		case 13:
			this->ReceiveDropPlayer(buffer, offset);
			break;
		default:
			offset = size;
			break;
		}
	}
}

void MsgHandler::Ping(int index)
{
	if(index >= 0 && index < this->mNet->GetNumPlayers())
	{
		char buffer[] = {(char)2};
		//this->mConn->SetWriteBuffer(buffer, 1, index - 1);
		QueryPerformanceCounter(&mOldTick);
		//this->mConn->Send(-1);
		this->mConn->Send(buffer, 1, index);//index - 1);
	}
}
void MsgHandler::SendClose()
{
	char identifier = (char)5;
	char buf[BUFFER_SIZE];
	int offset = 0;
	this->AddToBuffer(buf, offset, identifier);
	for(int i = 1; i < this->mNet->GetNumPlayers(); i++)
	{
		//this->mConn->SetWriteBuffer(buf, offset, i);
		this->mConn->Send(buf, offset, i);
	}
	Sleep(500);
}
void MsgHandler::SendClose(int index)
{	
	if(index > 0 && index < this->mNet->GetNumPlayers())
	{
		char identifier = (char)5;
		char buf[BUFFER_SIZE];
		int offset = 0;
		this->AddToBuffer(buf, offset, identifier);
		this->mConn->Send(buf, offset, index);
	}
}
void MsgHandler::SendClientData()
{
	if(this->mNet->GetBall(this->mNet->GetIndex())->GetNumCommands() > 0)
	{
		char identifier = (char)6;
		char bufW[BUFFER_SIZE] = {0};
		int offset = 0;
		this->AddToBuffer(bufW, offset, identifier);

		Command* command = this->mNet->GetBall(this->mNet->GetIndex())->GetNextCommand();
		while(command != NULL)
		{
			this->AddToBuffer(bufW, offset, (char)command->GetNumInputs());
			for(int i = 0; i < command->GetNumInputs(); i++)
			{
				this->AddToBuffer(bufW, offset, command->GetInput(i));
			}
			this->AddToBuffer(bufW, offset, command->GetDuration());
			this->AddToBuffer(bufW, offset, command->GetForward());

			this->mNet->GetBall(this->mNet->GetIndex())->PopCommand();
			command = this->mNet->GetBall(this->mNet->GetIndex())->GetNextCommand();
		}
		if(offset > 1)
		{
			this->AddToBuffer(bufW, offset, SEGMENT_END);
			this->AddToBuffer(bufW, offset, this->mNet->GetBall(this->mNet->GetIndex())->GetExecTime());
			//this->mConn->SetWriteBuffer(bufW, offset, 0);
			this->mConn->Send(bufW, offset, 0);
		}
	}
}
void MsgHandler::SendServerData()
{
	char identifier = (char)7;
	int numPlayers = this->mNet->GetNumPlayers();
	for(int a = 1; a < numPlayers; a++)
	{
		char bufW[BUFFER_SIZE] = {0};
		int offset = 0;
		this->AddToBuffer(bufW, offset, identifier);
		
		this->AddToBuffer(bufW, offset, (char)a);
		this->AddToBuffer(bufW, offset, (char)numPlayers);

		for(int i = 0; i < numPlayers; i++)
		{
			this->AddToBuffer(bufW, offset, (char)i);
			this->AddToBuffer(bufW, offset, (char)(int)this->mNet->GetBall(i)->GetTeam());
			
			if (this->mNet->GetServerInfo().GetGameMode() == GAMEMODE::WARLOCK)
			{
				this->AddToBuffer(bufW, offset, this->mNet->GetBall(i)->GetHP());
			}
			this->AddToBuffer(bufW, offset, this->mNet->GetBall(i)->GetPos());
			this->AddToBuffer(bufW, offset, this->mNet->GetBall(i)->GetVel());
		}
		if(this->mNet->GetServerInfo().GetGameMode() == GAMEMODE::CTF)
		{
			this->AddToBuffer(bufW, offset, this->mNet->GetFlagPos(0));
			this->AddToBuffer(bufW, offset, this->mNet->GetFlagPos(1));
		}

		
		this->AddToBuffer(bufW, offset, this->mNet->GetBall(a)->GetExecTime());

		//this->mConn->SetWriteBuffer(bufW, offset, a-1);
		this->mConn->Send(bufW, offset, a);
	}
}
void MsgHandler::SendIdentifyYourself()
{
	char identifier = (char)7;
	int numPlayers = this->mNet->GetNumPlayers();
	for(int a = 1; a < numPlayers; a++)
	{
		char bufW[BUFFER_SIZE] = {0};
		int offset = 0;
		this->AddToBuffer(bufW, offset, identifier);
		//this->mConn->SetWriteBuffer(bufW, offset, a-1);
		//this->mConn->Send(bufW, offset, a-1);
	}
}
void MsgHandler::SendPlayerInfos()
{
}
void MsgHandler::JoinTeam(TEAM id)
{
	if(!this->mNet->IsServer())
	{
		char identifier = (char)11;
		char bufW[BUFFER_SIZE] = {0};
		int offset = 0;
		this->AddToBuffer(bufW, offset, identifier);
		this->AddToBuffer(bufW, offset, (char)(int)id);
		//this->mConn->SetWriteBuffer(bufW, offset, -1);
		this->mConn->Send(bufW, offset, 0);
	}
	else this->mNet->GetBall(0)->SetTeam(id);
}
void MsgHandler::SendCastSpell(int casterIndex, int spellIndex)
{
	if(casterIndex >= 0 && casterIndex < this->mNet->GetNumPlayers())
	{
		char identifier = (char)12;
		char bufW[BUFFER_SIZE] = {0};
		int offset = 0;
		this->AddToBuffer(bufW, offset, identifier);
		this->AddToBuffer(bufW, offset, (char)(int)casterIndex);
		this->AddToBuffer(bufW, offset, (char)(int)spellIndex);
		for(int i = 1; i < this->mConn->GetNumConnections(); i++)
		{
			if(i != casterIndex)
				this->mConn->Send(bufW, offset, i);
		}
	}
}
void MsgHandler::SendDropPlayer(int index)
{
	if(index > 0 && index < this->mNet->GetNumPlayers())
	{
		this->SendClose(index);

		char identifier = (char)13;
		char bufW[BUFFER_SIZE] = {0};
		int offset = 0;
		this->AddToBuffer(bufW, offset, identifier);
		this->AddToBuffer(bufW, offset, (char)(int)index);
		for(int i = 1; i < this->mConn->GetNumConnections(); i++)
		{
			if(i != index)
				this->mConn->Send(bufW, offset, i);
		}
	}
}


void MsgHandler::ReceiveDropPlayer(char* buf, int &offset)
{
	int index = (int)GetFromBufC(buf, offset);
	if(index > 0 && index < this->mNet->GetNumPlayers() && index != this->mNet->GetIndex())
	{
		this->mNet->DropPlayer(index);
	}
}
void MsgHandler::ReceiveCastSpell(char* buf, int &offset, int index)
{
	if(index >= 0 && index < this->mNet->GetNumPlayers())
	{
		int casterIndex = (int)GetFromBufC(buf, offset);
		if(casterIndex < this->mNet->GetNumPlayers())
		{
			int spellIndex = (int)GetFromBufC(buf, offset);
			char spells[5];
			spells[0] = (char)spellIndex;
			this->mNet->GetBall(casterIndex)->AddKeyInput(spells, 1, 0, D3DXVECTOR3(), false);
		}
	}
}
void MsgHandler::ReceivePing(int index)
{
	if(index >= 0 && index < this->mNet->GetNumPlayers())
	{
		static int numberOfPings = 0; //add ALIVE here
		static float totalLatency = 0.0f; //CARE FOR OVERFLOW

		LARGE_INTEGER now;
		QueryPerformanceCounter(&now);

		LARGE_INTEGER proc_freq;
		::QueryPerformanceFrequency(&proc_freq);
		float frequency = (float)proc_freq.QuadPart;
		float latency = 1000*((now.QuadPart - mOldTick.QuadPart) / frequency);
	
		numberOfPings++;
		totalLatency += latency;
		index;
		this->mNet->SetLatency(totalLatency / numberOfPings);
	}
}
void MsgHandler::ReturnPing(int index)
{
	if(index >= 0 && index < this->mNet->GetNumPlayers())
	{
		char buffer[] = {(char)3};
		//this->mConn->SetWriteBuffer(buffer, 1, index - 1);
		this->mConn->Send(buffer, 1, index);

		this->mNet->GetBall(index)->ResetAliveTime();
	}
}
void MsgHandler::Close()
{
	this->mNet->Close();
}

void MsgHandler::ReceiveClientData(char* buf, int &offset, int index)
{
	if(index >= 0 && index < this->mNet->GetNumPlayers())
	{
		while(buf[offset] != SEGMENT_END)
		{
			int numKeys = (int)this->GetFromBufferC(buf, offset);
			char keys[5];
			for(int i = 0; i < numKeys; i++)
			{
				keys[i] = this->GetFromBufferC(buf, offset);
			}
			float duration = this->GetFromBufferF(buf, offset);
			D3DXVECTOR3 forward = this->GetFromBufferD(buf, offset);
			this->mNet->GetBall(index)->AddKeyInput(keys, numKeys, duration, forward, false);
		}
		offset += sizeof(SEGMENT_END);
		
		float clientExecTime = this->GetFromBufferF(buf, offset);
		this->mNet->GetBall(index)->SetClientExecTime(clientExecTime);
		this->mNet->GetBall(index)->ResetAliveTime();
	}
}
void MsgHandler::ReceiveServerData(char* buf, int &offset)
{
	this->mNet->SetIndex((int) this->GetFromBufferC(buf, offset));
	int numPlayers = (int) this->GetFromBufferC(buf, offset);
	this->mNet->SetNumPlayers(numPlayers);
	for(int i = 0; i < numPlayers; i++) 
	{
		int index = (int) this->GetFromBufferC(buf, offset);
		TEAM team = (TEAM)(int) this->GetFromBufferC(buf, offset);
		this->mNet->GetBall(index)->SetTeam(team);

		if (this->mNet->GetServerInfo().GetGameMode() == GAMEMODE::WARLOCK)
		{
			float hp = this->GetFromBufferF(buf, offset);
			this->mNet->GetBall(index)->SetHP(hp);
		}

		this->mNet->GetBall(index)->SetPos(this->GetFromBufferD(buf, offset));
		this->mNet->GetBall(index)->SetVel(this->GetFromBufferD(buf, offset));
	}
	
	if (this->mNet->GetServerInfo().GetGameMode() == GAMEMODE::CTF)
	{
		this->mNet->SetFlagPos(this->GetFromBufferD(buf, offset), 0);
		this->mNet->SetFlagPos(this->GetFromBufferD(buf, offset), 1);
	}
	

	this->mNet->GetBall(0)->SetClientExecTime(this->GetFromBufferF(buf, offset));
	this->mNet->GetBall(0)->ResetAliveTime();
}
void MsgHandler::ReceiveIdentification(char* buf, int &offset, int index)
{
	//Profile temp("kaka");
	//this->mLobby->AddPlayer(temp, index);
}
void MsgHandler::ReceivePlayerInfos(char* buf, int &offset)
{
	//Profile temp("kaka");
	//this->mLobby->AddPlayer(temp,0);
}
void MsgHandler::ReceiveTeamChange(char* buf, int &offset, int index)
{
	if(index >= 0 && index < this->mNet->GetNumPlayers())
	{
		TEAM team = (TEAM)(int)this->GetFromBufferC(buf, offset);
		this->mNet->GetBall(index)->SetTeam(team);
	}
}

void MsgHandler::PlayerReconnect(int index)
{ 
	if(index >= 0 && index < this->mNet->GetNumPlayers())
	{
		this->mNet->GetBall(index)->SetExecTime(0.0f); 
		this->mNet->GetBall(index)->ClearCommands();
	}
}

void MsgHandler::AddToBuffer(char* bufOut, int &offsetOut, float in)
{
	memmove(bufOut + offsetOut, &in, sizeof (in));
	offsetOut += sizeof (in);
}
void MsgHandler::AddToBuffer(char* bufOut, int &offsetOut, char in)
{
	memmove(bufOut + offsetOut, &in, sizeof (in));
	offsetOut += sizeof (in);
}
void MsgHandler::AddToBuffer(char* bufOut, int &offsetOut, D3DXVECTOR3 in)
{
	this->AddToBuffer(bufOut, offsetOut, in.x);
	this->AddToBuffer(bufOut, offsetOut, in.y);
	this->AddToBuffer(bufOut, offsetOut, in.z);
}
float MsgHandler::GetFromBufferF(char* buf, int &offsetOut)
{
	float out = 0.0f;
	memmove(&out, &buf[offsetOut], sizeof (float));
	offsetOut += sizeof (float);
	return out;
}
char MsgHandler::GetFromBufferC(char* buf, int &offsetOut)
{
	char out = '0';
	memmove(&out, &buf[offsetOut], sizeof (char));
	offsetOut += sizeof (char);
	return out;
}
D3DXVECTOR3	MsgHandler::GetFromBufferD(char* buf, int &offsetOut)
{
	D3DXVECTOR3 out;
	out.x = this->GetFromBufferF(buf, offsetOut);
	out.y = this->GetFromBufferF(buf, offsetOut);
	out.z = this->GetFromBufferF(buf, offsetOut);
	return out;
}