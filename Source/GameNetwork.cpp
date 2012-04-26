
#include "GameNetwork.h"

GameNetwork::GameNetwork()
{
	this->mConn = new ServerConnection();
	this->mPos = new D3DXVECTOR3[PLAYER_CAP];
	this->mVel = new D3DXVECTOR3[PLAYER_CAP];
	this->mFlagPos = new D3DXVECTOR3[2];
	this->mIndex = 0;
	this->mNumPlayers = 1;
}
GameNetwork::~GameNetwork()
{
	SAFE_DELETE(this->mConn);
	SAFE_DELETE_ARRAY(this->mPos);
	SAFE_DELETE_ARRAY(this->mVel);
}
void GameNetwork::SetPos(const D3DXVECTOR3 pos, const int index)
{
	this->mPos[index] = pos;
}
void GameNetwork::SetStartPos(const D3DXVECTOR3 pos, const int index)
{
	this->mStartPositions[index] = pos;
}
void GameNetwork::SetStartPosistions(const D3DXVECTOR3 pos[], const int size)
{
	for(int i = 0; i < size; i++)
	{
		this->mStartPositions[i] = pos[i];
	}
}
void GameNetwork::SetVel(const D3DXVECTOR3 vel, const int index)
{
	this->mVel[index] = vel;
}
void GameNetwork::SetFlagPos(const D3DXVECTOR3 pos, const int index)
{
	this->mFlagPos[index] = pos;
}
void GameNetwork::AddKeyInput(const char key, const bool down)
{
	if(this->mKeyInputs[this->mIndex][key] && down == false)
	{
		this->mKeyUps.push_back(key);
	}
	this->mKeyInputs[this->mIndex][key] = down;
}
bool GameNetwork::IsKeyPressed(const char key, const int index) const
{
	return this->mKeyInputs[index][key];
}
bool GameNetwork::ClientUpdate()
{
	bool ret = true;
	char bufW[256] = {0};
	int offset = 0;
	/*
		Valve limits client sends to 30 packets per second.
	*/
	for(int i = 0; i < 256; i++)
	{
		if(this->mKeyInputs[this->mIndex][i])
		{
			this->AddToBuffer(bufW, offset, (char)i);
		}
	}
	if(offset > 0 || (this->mKeyUps.size() > 0 && offset == 0))
		this->AddToBuffer(bufW, offset, ';');

	for(int i = 0; i < this->mKeyUps.size(); i++)
	{
		this->AddToBuffer(bufW, offset, this->mKeyUps[i]);
	}
	this->mKeyUps.clear();

	if(offset > 0)
	{
		this->AddToBuffer(bufW, offset, ';');
		this->mConn->SetWriteBuffer(bufW, offset, 0);
	}


	char buf[256] = {0};
	if(this->mConn->GetReadBuffer(buf, 256, 0))
	{
		offset = 0;
		char identifier = this->GetFromBufferC(buf, offset);
		switch(identifier)
		{
		case 'K':
			{
				this->mIndex = (int) this->GetFromBufferC(buf, offset);
				this->mNumPlayers = (int) this->GetFromBufferC(buf, offset);

				for(int i = 0; i < this->mNumPlayers; i++) 
				{
					int index = (int) this->GetFromBufferC(buf, offset);
					this->mPos[index] = this->GetFromBufferD(buf, offset);
					this->mVel[index] = this->GetFromBufferD(buf, offset);
				}
				this->mFlagPos[0] = this->GetFromBufferD(buf, offset);
				this->mFlagPos[1] = this->GetFromBufferD(buf, offset);
				break;
			}
		case 'F':
			{
				this->mFlagPos[0] = this->GetFromBufferD(buf, offset);
				this->mFlagPos[1] = this->GetFromBufferD(buf, offset);
				break;
			}
		case 'C':
			{
				ret = false;
				this->Close();
				break;
			}
		}
	}
	
	//for(int a = 0; a < 256; a++)
		//this->mKeyInputs[this->mIndex][a] = false;

	return ret;
}

void GameNetwork::ServerUpdate()
{
	//if(this->mServer.GetGameMode() == CTF)
		//this->SendCTFParams();
	for(int i = 1; i < this->mConn->GetNumConnections(); i++)
	{

		char buf[256];
		if(this->mConn->GetReadBuffer(buf, 256, i - 1))
		{
			for(int a = 0; a < 256; a++)
				this->mKeyInputs[i][a] = false;
			int offset = 0;
			while(buf[offset] != ';')
			{
				this->mKeyInputs[i][this->GetFromBufferC(buf, offset)] = true;
			}
			offset += sizeof(char);
			
			while(buf[offset] != ';')
			{
				this->mKeyInputs[i][this->GetFromBufferC(buf, offset)] = false;
			}
			offset += sizeof(char);
		}
	}
	/*
		Clients usually have only a limited amount of available bandwidth. 
		In the worst case, players with a modem connection can't receive more than 5 to 7 KB/sec. If the server tried to send them updates with a higher data rate, 
		packet loss would be unavoidable. Therefore, the client has to tell the server its incoming bandwidth capacity by setting the console variable rate (in bytes/second). 
		This is the most important network variable for clients and it has to be set correctly for an optimal gameplay experience. 
		The client can request a certain snapshot rate by changing cl_updaterate (default 20), but the server will never send more updates than simulated ticks or exceed the requested client rate limit. 
		Server admins can limit data rate values requested by clients with sv_minrate and sv_maxrate (both in bytes/second). 
		Also the snapshot rate can be restricted with sv_minupdaterate and sv_maxupdaterate (both in snapshots/second).


		Game data is compressed using delta compression to reduce network load. 
		That means the server doesn't send a full world snapshot each time, 
		but rather only changes (a delta snapshot) that happened since the last acknowledged update. With each packet sent between the client and server, 
		acknowledge numbers are attached to keep track of their data flow. 
		Usually full (non-delta) snapshots are only sent when a game starts or a client suffers from heavy packet loss for a couple of seconds. 
		Clients can request a full snapshot manually with the cl_fullupdate command.


		Responsiveness, or the time between user input and its visible feedback in the game world, are determined by lots of factors, 
		including the server/client CPU load, simulation tickrate, data rate and snapshot update settings, but mostly by the network packet traveling time. 
		The time between the client sending a user command, the server responding to it, and the client receiving the server's response is called the latency or ping (or round trip time). 
		Low latency is a significant advantage when playing a multiplayer online game. 
		Techniques like prediction and lag compensation try to minimize that advantage and allow a fair game for players with slower connections


		The lag compensation system keeps a history of all recent player positions for one second. 
		If a user command is executed, the server estimates at what time the command was created as follows:
			Command Execution Time = Current Server Time - Packet Latency - Client View Interpolation
	*/


	//Perhaps add some rules here, so doesnt send stuff that isnt necessary to update (like a player standing still)
	for(int a = 1; a < this->mNumPlayers; a++)
	{
		char bufW[256] = {0};
		int offset = 0;
		this->AddToBuffer(bufW, offset, 'K');

		this->AddToBuffer(bufW, offset, (char)a);
		this->AddToBuffer(bufW, offset, (char)this->mNumPlayers);

		for(int i = 0; i < this->mNumPlayers; i++)
		{
			this->AddToBuffer(bufW, offset, (char)i);
			this->AddToBuffer(bufW, offset, this->mPos[i]);
			this->AddToBuffer(bufW, offset, this->mVel[i]);
		}
		this->AddToBuffer(bufW, offset, this->mFlagPos[0]);
		this->AddToBuffer(bufW, offset, this->mFlagPos[1]);

		this->mConn->SetWriteBuffer(bufW, offset, a-1);
	}

}
int test = 0;

//Ball* shadow;
bool GameNetwork::Update(Ball**	balls, int &numBalls, float dt)
{
	bool ret = true;
	if(this->IsServer())
	{	
		if(this->mNumPlayers != this->mConn->GetNumConnections())
		{
			this->mNumPlayers = this->mConn->GetNumConnections();
			for(int i = 0; i < this->mNumPlayers - 1; i++)
			{
				this->mPos[i] = this->mStartPositions[i];
				balls[i]->SetPosition(this->mStartPositions[i]);
				balls[i]->SetNumLives(2);
			}
		}
		if(this->mConn->GetNumConnections() > 1)
			this->ServerUpdate();
		//update ball (diff = RTT)
	}
	else ret = this->ClientUpdate();

	if(!this->IsServer())
	for(int i = 0; i < numBalls; i++)
	{
		/*if(i != this->mIndex)
		{
			balls[i]->SetVelocity(Vector3(this->mVel[i]));
			Vector3 direction = Vector3(this->mPos[i].x - balls[i]->GetPosition().x, 0, this->mPos[i].z - balls[i]->GetPosition().z);
			balls[i]->Rotate(direction);
			balls[i]->SetPosition(this->mPos[i]);
		}
		else
		{*/
			D3DXVECTOR3 direction = D3DXVECTOR3(this->mPos[i].x - balls[i]->GetPosition().x, this->mPos[i].y - balls[i]->GetPosition().y, this->mPos[i].z - balls[i]->GetPosition().z);
			float length = D3DXVec3Length(&direction);
			if(length < INTERPOS_MIN || length > INTERPOS_MAX) // the difference is so little or too great so just replace the local pos with network pos.
			{
				
				balls[i]->SetVelocity(Vector3(this->mVel[i]));
				//Vector3 direction = Vector3(this->mPos[i].x - balls[i]->GetPosition().x, 0, this->mPos[i].z - balls[i]->GetPosition().z);
				balls[i]->Rotate(direction);
				balls[i]->SetPosition(this->mPos[i]);
			}
			else
			{
				balls[i]->SetVelocity(Vector3(this->mVel[i])*INTERVEL_MOD  + balls[i]->GetVelocity() * (1.0f-INTERVEL_MOD));


				//balls[i]->SetVelocity(balls[i]->GetVelocity() + direction * 0.25);


				D3DXVECTOR3 modifier = direction * dt * 0.001f * INTERPOS_MOD;
				balls[i]->Rotate(modifier);
				balls[i]->SetPosition(balls[i]->GetPosition() + modifier);
				
			}
		//}
	}
	/*if(!this->IsServer())
	{
		if(test == 0)
		{
			shadow = new Ball("Media/Ball.obj", D3DXVECTOR3(0,30.0f,-15));
			test++;
		}
		shadow->SetPosition(this->mPos[1]);
	}*/
	return ret;
}
void GameNetwork::Start(ServerInfo server)
{
	this->mServer = server;
	for(int i = 0; i < PLAYER_CAP; i++)
	{
		for(int a = 0; a < 256; a++)
			this->mKeyInputs[i][a]=false;

		this->mPos[i] = this->mStartPositions[i];
		this->mVel[i] = D3DXVECTOR3(0,0,0);
	}
	
	if(this->mServer.GetIP() == "")
	{
		mConn->Host(server);
	}
	else 
	{
		mConn->Connect(server);
	}
	
}
void GameNetwork::Close()
{
	if(this->IsServer())
	{
		char buf[BUFFER_SIZE];
		int offset = 0;
		this->AddToBuffer(buf, offset, 'C');
		for(int i = 0; i < this->mNumPlayers - 1; i++)
		{
			this->mConn->SetWriteBuffer(buf, BUFFER_SIZE, i);
		}
		Sleep(500);
	}
	this->mConn->Close();
}
vector<ServerInfo> GameNetwork::FindServers()
{
	return this->mConn->FindServers();
}
void GameNetwork::AddToBuffer(char* bufOut, int &offsetOut, float in)
{
	memmove(bufOut + offsetOut, &in, sizeof (in));
	offsetOut += sizeof (in);
}
void GameNetwork::AddToBuffer(char* bufOut, int &offsetOut, char in)
{
	memmove(bufOut + offsetOut, &in, sizeof (in));
	offsetOut += sizeof (in);
}
void GameNetwork::AddToBuffer(char* bufOut, int &offsetOut, D3DXVECTOR3 in)
{
	this->AddToBuffer(bufOut, offsetOut, in.x);
	this->AddToBuffer(bufOut, offsetOut, in.y);
	this->AddToBuffer(bufOut, offsetOut, in.z);
}
float GameNetwork::GetFromBufferF(char* buf, int &offsetOut)
{
	float out = 0.0f;
	memmove(&out, &buf[offsetOut], sizeof (float));
	offsetOut += sizeof (float);
	return out;
}
char GameNetwork::GetFromBufferC(char* buf, int &offsetOut)
{
	char out = '0';
	memmove(&out, &buf[offsetOut], sizeof (char));
	offsetOut += sizeof (char);
	return out;
}
D3DXVECTOR3	GameNetwork::GetFromBufferD(char* buf, int &offsetOut)
{
	D3DXVECTOR3 out;
	out.x = this->GetFromBufferF(buf, offsetOut);
	out.y = this->GetFromBufferF(buf, offsetOut);
	out.z = this->GetFromBufferF(buf, offsetOut);
	return out;
}
void GameNetwork::SendCTFParams()
{
	/*char bufW[256] = {0};
	int offset = 0;
	this->AddToBuffer(bufW, offset, 'F');
	this->AddToBuffer(bufW, offset, this->mFlagPos[0]);
	this->AddToBuffer(bufW, offset, this->mFlagPos[1]);

	for(int a = 1; a < this->mNumPlayers; a++)
	{
		this->mConn->SetWriteBuffer(bufW, 256, a-1);
	}*/
}