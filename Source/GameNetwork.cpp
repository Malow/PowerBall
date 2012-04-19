
#include "GameNetwork.h"

GameNetwork::GameNetwork()
{
	this->mConn = new ServerConnection();
	this->mPos = new D3DXVECTOR3[PLAYER_CAP];
	this->mVel = new D3DXVECTOR3[PLAYER_CAP];
	this->mIndex = 0;
	this->mNumPlayers = 1;

	//replace with MAP->GETSTARTPOSITIONS() ?
	this->mStartPositions[0] = D3DXVECTOR3(3,14.7f,-5);
	this->mStartPositions[1] = D3DXVECTOR3(3,14.7f,5);
	this->mStartPositions[2] = D3DXVECTOR3(-3,14.7f,-5);
	this->mStartPositions[3] = D3DXVECTOR3(-3,14.7f,5);
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
void GameNetwork::SetVel(const D3DXVECTOR3 vel, const int index)
{
	this->mVel[index] = vel;
}
void GameNetwork::AddKeyInput(const char key, const bool down)
{
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

	for(int i = 0; i < 256; i++)
	{
		if(this->mKeyInputs[this->mIndex][i])
		{
			this->AddToBuffer(bufW, offset, (char)i);
		}
	}
	this->mConn->SetWriteBuffer(bufW, 256, 0);

	ret = this->mConn->Update();

	char buf[256] = {0};
	if(this->mConn->GetReadBuffer(buf, 256, 0))
	{
		offset = 0;
		this->mIndex = (int) this->GetFromBufferC(buf, offset);
		this->mNumPlayers = (int) this->GetFromBufferC(buf, offset);

		for(int i = 0; i < this->mNumPlayers; i++) 
		{
			int index = (int) this->GetFromBufferC(buf, offset);
			this->mPos[index] = this->GetFromBufferD(buf, offset);
			this->mVel[index] = this->GetFromBufferD(buf, offset);
		}
	}
	
	for(int a = 0; a < 256; a++)
		this->mKeyInputs[this->mIndex][a] = false;

	return ret;
}

void GameNetwork::ServerUpdate()
{
	for(int i = 1; i < this->mConn->GetNumConnections(); i++)
	{
		char buf[256];
		if(this->mConn->GetReadBuffer(buf, 256, i - 1))
		{
			for(int a = 0; a < 256; a++)
				this->mKeyInputs[i][a] = false;

			int offset = 0;
			for(int a = 0; a < 5; a++)
			{
				this->mKeyInputs[i][this->GetFromBufferC(buf, offset)] = true;
			}
		}
	}
	
	for(int a = 1; a < this->mNumPlayers; a++)
	{
		char bufW[256] = {0};
		int offset = 0;

		this->AddToBuffer(bufW, offset, (char)a);
		this->AddToBuffer(bufW, offset, (char)this->mNumPlayers);

		for(int i = 0; i < this->mNumPlayers; i++)
		{
			this->AddToBuffer(bufW, offset, (char)i);
			this->AddToBuffer(bufW, offset, this->mPos[i]);
			this->AddToBuffer(bufW, offset, this->mVel[i]);
		}

		this->mConn->SetWriteBuffer(bufW, 256, a-1);
	}

}
bool GameNetwork::Update(Ball**	balls, int &numBalls)
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
			}
		}
		if(this->mConn->GetNumConnections() > 1)
			this->ServerUpdate();
	}
	else ret = this->ClientUpdate();

	if(!this->IsServer())
	for(int i = 0; i < numBalls; i++)
	{
		balls[i]->SetVelocity(Vector3(this->mVel[i]));
		Vector3 direction = Vector3(this->mPos[i].x - balls[i]->GetPosition().x, this->mPos[i].y - balls[i]->GetPosition().y, this->mPos[i].z - balls[i]->GetPosition().z);
		balls[i]->Rotate(direction);
		balls[i]->SetPosition(this->mPos[i]);
	}

	return ret;
}
void GameNetwork::Start()
{
	for(int i = 0; i < PLAYER_CAP; i++)
	{
		for(int a = 0; a < 256; a++)
			this->mKeyInputs[i][a]=false;

		this->mPos[i] = this->mStartPositions[i];
		this->mVel[i] = D3DXVECTOR3(0,0,0);
	}
	mConn->InitializeConnection();
}
void GameNetwork::SetIP(char ip[])
{
	this->mConn->SetIP(ip);
}
void GameNetwork::Close()
{
	this->mConn->Close();
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