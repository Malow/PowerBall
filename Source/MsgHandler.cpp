#include "MsgHandler.h"
#include "GameNetwork.h"
MsgHandler::~MsgHandler()
{
}
void MsgHandler::Set(GameNetwork* gn, ServerConnection*	conn)
{
	this->mConn = conn;
	this->mNet = gn;
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
		case 2:
			this->ReturnPing(index);
			break;
		case 3:
			this->ReceivePing(index);
			break;
		case 4:
		case 5:
			this->Close();
			break;
		case 6:
			this->ReceiveClientData(buffer, offset, index);
			break;
		case 7:
			this->ReceiveServerData(buffer, offset);
			break;
		}
	}
}

void MsgHandler::Ping(int index)
{
	char buffer[] = {(char)2};
	this->mConn->SetWriteBuffer(buffer, 1, index - 1);
	QueryPerformanceCounter(&mOldTick);
	this->mConn->Send(-1);
}
void MsgHandler::SendClose()
{
	char identifier = (char)5;
	char buf[BUFFER_SIZE];
	int offset = 0;
	this->AddToBuffer(buf, offset, identifier);
	for(int i = 0; i < this->mNet->GetNumPlayers() - 1; i++)
	{
		this->mConn->SetWriteBuffer(buf, offset, i);
		this->mConn->Send(i);
	}
	Sleep(500);
}
void MsgHandler::SendClientData()
{
	char identifier = (char)6;
	char bufW[BUFFER_SIZE] = {0};
	int offset = 0;
	this->AddToBuffer(bufW, offset, identifier);

	KeyInput* command = this->mNet->GetNextCommand(this->mNet->GetIndex());
	while(command != NULL)
	{
		this->AddToBuffer(bufW, offset, (char)command->numKeys);
		for(int i = 0; i < command->numKeys; i++)
		{
			this->AddToBuffer(bufW, offset, command->keys[i]);
		}
		this->AddToBuffer(bufW, offset, command->dt);

		this->mNet->PopCommand(this->mNet->GetIndex());
		command = this->mNet->GetNextCommand(this->mNet->GetIndex());
	}
	
	if(offset > 1)
	{
		this->AddToBuffer(bufW, offset, SEGMENT_END);
		this->AddToBuffer(bufW, offset, this->mNet->GetForwardVector(this->mNet->GetIndex()));
		this->mConn->SetWriteBuffer(bufW, offset, 0);
		this->mConn->Send(-1);
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
			this->AddToBuffer(bufW, offset, this->mNet->GetPos(i));
			this->AddToBuffer(bufW, offset, this->mNet->GetVel(i));
		}
		this->AddToBuffer(bufW, offset, this->mNet->GetFlagPos(0));
		this->AddToBuffer(bufW, offset, this->mNet->GetFlagPos(1));

		
		this->AddToBuffer(bufW, offset, this->mNet->GetExecTime(a));

		this->mConn->SetWriteBuffer(bufW, offset, a-1);
		this->mConn->Send(a-1);
	}
}


void MsgHandler::ReceivePing(int index)
{
	static int numberOfPings = 0;
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
void MsgHandler::ReturnPing(int index)
{
	char buffer[] = {(char)3};
	this->mConn->SetWriteBuffer(buffer, 1, index - 1);
	this->mConn->Send(index-1);
}
void MsgHandler::Close()
{
	this->mNet->Close();
}

void MsgHandler::ReceiveClientData(char* buf, int &offset, int index)
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
		this->mNet->AddKeyInput(index, keys, numKeys, duration);
	}
	offset += sizeof(SEGMENT_END);
	this->mNet->SetForwardVector(this->GetFromBufferD(buf, offset), index);
}
void MsgHandler::ReceiveServerData(char* buf, int &offset)
{
	this->mNet->SetIndex((int) this->GetFromBufferC(buf, offset));
	int numPlayers = (int) this->GetFromBufferC(buf, offset);
	this->mNet->SetNumPlayers(numPlayers);
	for(int i = 0; i < numPlayers; i++) 
	{
		int index = (int) this->GetFromBufferC(buf, offset);
		this->mNet->SetPos(this->GetFromBufferD(buf, offset), index);
		this->mNet->SetVel(this->GetFromBufferD(buf, offset), index);
	}
	this->mNet->SetFlagPos(this->GetFromBufferD(buf, offset), 0);
	this->mNet->SetFlagPos(this->GetFromBufferD(buf, offset), 1);
	

	this->mNet->SetServerExecTime(this->GetFromBufferF(buf, offset));
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