
#include "GameNetwork.h"

GameNetwork::GameNetwork()
{
	this->mConn = new ServerConnection();
}
GameNetwork::~GameNetwork()
{
	delete this->mConn;
}
D3DXVECTOR3 GameNetwork::GetEnemyPos()
{
	return this->mEnemyPos;
}
void GameNetwork::SetPos(D3DXVECTOR3 pos)
{
	this->mPos = pos;
}
void GameNetwork::Update(Ball* ball)
{
	this->mPos = ball->GetPosition();
	char bufW[256] = {0};

	float _x = this->mPos.x;
	float _y = this->mPos.y;
	float _z = this->mPos.z;

	char mod = ';';
	int offset = 0;
	memcpy(bufW, &_x, sizeof (float));

	offset += sizeof (float);
	memcpy(bufW+offset, &mod, sizeof (mod));

	offset += sizeof (mod);
	memcpy(bufW+offset, &_y, sizeof (float));
		
		
	offset += sizeof (float);
	memcpy(bufW+offset, &mod, sizeof (mod));

	offset += sizeof (mod);
	memcpy(bufW+offset, &_z, sizeof (float));
		
	offset += sizeof (float);
	memcpy(bufW+offset, &mod, sizeof (mod));

	//for(int i = 0; i<  254; i++)
	//	bufW[i] = 'p';

	this->mConn->SetWriteBuffer(bufW, 256);


	this->mConn->Update();


	char buf[256];
	this->mConn->GetReadBuffer(buf, 256);
	float x = 0;
	float y = 0;
	float z = 0;
	
	memcpy(&x, &buf, sizeof (float));
	memcpy(&y, &buf[sizeof (float) + sizeof(char)], sizeof (float));
	memcpy(&z, &buf[2*(sizeof (float) + sizeof(char))], sizeof (float));

	float dist = D3DXVec3Length(&(D3DXVECTOR3(x, y, z) - this->mEnemyPos));
	if(dist < 15)
		this->mEnemyPos = D3DXVECTOR3(x, y, z);



}
void GameNetwork::Start()
{
	mConn->InitializeConnection();
	if(this->IsServer())
	{
		this->mPos = D3DXVECTOR3(0,14.7f,5);
		this->mEnemyPos = D3DXVECTOR3(0,14.7f,-5);
	}
	else
	{
		this->mPos = D3DXVECTOR3(0,14.7f,-5);
		this->mEnemyPos = D3DXVECTOR3(0,14.7f,5);
	}
}
void GameNetwork::SetIP(char ip[])
{
	this->mConn->SetIP(ip);
}
