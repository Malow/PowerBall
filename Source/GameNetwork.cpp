
#include "GameNetwork.h"

GameNetwork::GameNetwork()
{
	this->mConn = new ServerConnection();
	this->mPlayerCap = 5;
	this->mPos = new D3DXVECTOR3[this->mPlayerCap];
	this->mVel = new D3DXVECTOR3[this->mPlayerCap];
	this->mIndex = 0;
}
GameNetwork::~GameNetwork()
{
	delete this->mConn;
}
D3DXVECTOR3 GameNetwork::GetPos(const int index)
{
	return this->mPos[index];
}
void GameNetwork::SetPos(D3DXVECTOR3 pos, int index)
{
	this->mPos[index] = pos;
}
void GameNetwork::SetVel(D3DXVECTOR3 vel, int index)
{
	this->mVel[index] = vel;
}
void GameNetwork::AddKeyInput(char key, bool down)
{
	this->mKeyInputs[this->mIndex][key] = down;
}
bool GameNetwork::IsKeyPressed(char key, int index)
{
	return this->mKeyInputs[index][key];
}
void GameNetwork::ClientUpdate()
{
	char bufW[256] = {0};

	float _x = this->mPos[this->mIndex].x;
	float _y = this->mPos[this->mIndex].y;
	float _z = this->mPos[this->mIndex].z;

	int offset = 0;

	memcpy(bufW+offset, &_x, sizeof (float));
	offset += sizeof (float);

	memcpy(bufW+offset, &_y, sizeof (float));
	offset += sizeof (float);

	memcpy(bufW+offset, &_z, sizeof (float));
	offset += sizeof (float);
	
	_x = this->mVel[this->mIndex].x;
	_y = this->mVel[this->mIndex].y;
	_z = this->mVel[this->mIndex].z;

	memcpy(bufW+offset, &_x, sizeof (float));
	offset += sizeof (float);

	memcpy(bufW+offset, &_y, sizeof (float));
	offset += sizeof (float);

	memcpy(bufW+offset, &_z, sizeof (float));
	offset += sizeof (float);

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


	this->mConn->Update();


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

			//float dist = D3DXVec3Length(&(D3DXVECTOR3(x, y, z) - this->mPos[index])); // lägg yill +1
			//if(dist < 15)
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
	//this->mConn->Update();
}

void GameNetwork::ServerUpdate()
{

	//this->mConn->Update();
	this->mNumPlayers = this->mConn->GetNumConnections();
	for(int i = 1; i < this->mConn->GetNumConnections(); i++)
	{

		char buf[256];
		if(this->mConn->GetReadBuffer(buf, 256, i - 1))
		{
			for(int a = 0; a < 256; a++)
				this->mKeyInputs[i][a] = false;
			float x = 0;
			float y = 0;
			float z = 0;
			int offset = 0;

			memcpy(&x, &buf, sizeof (float));
			offset += sizeof (float);

			memcpy(&y, &buf[offset], sizeof (float));
			offset += sizeof (float);

			memcpy(&z, &buf[offset], sizeof (float));
			offset += sizeof (float);

			//float dist = D3DXVec3Length(&(D3DXVECTOR3(x, y, z) - this->mPos[i])); 
			//if(dist < 15)
				//this->mPos[i] = D3DXVECTOR3(x, y, z);
		
			x = 0;
			y = 0;
			z = 0;

			memcpy(&x, &buf[offset], sizeof (float));
			offset += sizeof (float);

			memcpy(&y, &buf[offset], sizeof (float));
			offset += sizeof (float);

			memcpy(&z, &buf[offset], sizeof (float));
			offset += sizeof (float);
		
			//this->mVel[i] = D3DXVECTOR3(x, y, z);


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
		
	//this->mConn->Update();

}
void GameNetwork::Update(Ball**	balls, int &numBalls)
{

	if(this->IsServer())
	{
		if(this->mConn->GetNumConnections() > 1)
			this->ServerUpdate();
	}
	else this->ClientUpdate();
	//this->mConn->Update();


	if(!this->IsServer())
	for(int i = 0; i < numBalls; i++)
	{
		balls[i]->SetVelocity(Vector3(this->mVel[i]));
		Vector3 direction = Vector3(this->mPos[i].x - balls[i]->GetPosition().x, this->mPos[i].y - balls[i]->GetPosition().y, this->mPos[i].z - balls[i]->GetPosition().z);
		balls[i]->Rotate(direction);
		balls[i]->SetPosition(this->mPos[i]);
	}

}
void GameNetwork::Start()
{
	for(int i = 0; i < this->mPlayerCap; i++)
	{
		for(int a = 0; a < 256; a++)
			this->mKeyInputs[i][a]=false;
		this->mPos[i] = D3DXVECTOR3(0,14.7f,0);
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
