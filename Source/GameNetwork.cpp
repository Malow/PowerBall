
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

	char b = '0';
	for(int i = 0; i < 256; i++)
	{
		if(this->mKeyInputs[this->mIndex][i])
		{
			b = i;
			memcpy(bufW+offset, &i, sizeof (char));
			offset += sizeof (char);
		}
	}

	this->mConn->SetWriteBuffer(bufW, 256, 0);


	ret = this->mConn->Update();


	char buf[256] = {0};
	if(this->mConn->GetReadBuffer(buf, 256, 0))
	{
		float x = 0;
		float y = 0;
		float z = 0;
		int index = 0;
		int numPlayers = 0;
		offset = 0;
		char b = '0';
		memcpy(&b, &buf, sizeof (char));
		offset += sizeof (char);
		this->mIndex = b;

		memcpy(&b, &buf[offset], sizeof (char));
		offset += sizeof (char);
		numPlayers = b;
		this->mNumPlayers = numPlayers;
		for(int i = 0; i < numPlayers; i++) // lägg till +1
		{
			memcpy(&b, &buf[offset], sizeof (char));
			offset += sizeof (char);
			index = b;

			memcpy(&x, &buf[offset], sizeof (float));
			offset += sizeof (float);

			memcpy(&y, &buf[offset], sizeof (float));
			offset += sizeof (float);

			memcpy(&z, &buf[offset], sizeof (float));
			offset += sizeof (float);

			this->mPos[index] = D3DXVECTOR3(x, y, z);

			x = 0;
			y = 0;
			z = 0;

			memcpy(&x, &buf[offset], sizeof (float));
			offset += sizeof (float);

			memcpy(&y, &buf[offset], sizeof (float));
			offset += sizeof (float);

			memcpy(&z, &buf[offset], sizeof (float));
			offset += sizeof (float);

			this->mVel[index] = D3DXVECTOR3(x, y, z);
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
			char key = '0';
			for(int a = 0; a < 5; a++)
			{
				memcpy(&key, &buf[offset], sizeof (char));
				offset += sizeof (char);

				this->mKeyInputs[i][key] = true;
			}
		}
	}
	
	int numPlayers = this->mConn->GetNumConnections();
	for(int a = 1; a < numPlayers; a++)
	{
		char bufW[256] = {0};
		int offset = 0;
		char b = a;
		memcpy(bufW, &b, sizeof (char));
		offset += sizeof (char);

		b = numPlayers;
		memcpy(bufW + offset, &b, sizeof (char));
		offset += sizeof (char);

		for(int i = 0; i < numPlayers; i++)
		{
			float _x = this->mPos[i].x;
			float _y = this->mPos[i].y;
			float _z = this->mPos[i].z;
			b = i;
			memcpy(bufW + offset, &b, sizeof (char));
			offset += sizeof (char);

			memcpy(bufW + offset, &_x, sizeof (float));
			offset += sizeof (float);

			memcpy(bufW+offset, &_y, sizeof (float));
			offset += sizeof (float);

			memcpy(bufW+offset, &_z, sizeof (float));
			offset += sizeof (float);

			_x = this->mVel[i].x;
			_y = this->mVel[i].y;
			_z = this->mVel[i].z;

			memcpy(bufW + offset, &_x, sizeof (float));
			offset += sizeof (float);

			memcpy(bufW+offset, &_y, sizeof (float));
			offset += sizeof (float);

			memcpy(bufW+offset, &_z, sizeof (float));
			offset += sizeof (float);
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
