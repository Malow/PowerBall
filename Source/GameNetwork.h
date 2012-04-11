
#include "stdafx.h"
#include "ServerConnection.h"
#include <string>
#include "Ball.h"
using namespace std;
class GameNetwork
{
private:
	ServerConnection*	mConn;
	D3DXVECTOR3			mEnemyPos;
	D3DXVECTOR3			mPos;
public:
				GameNetwork();
	virtual		~GameNetwork();
	D3DXVECTOR3 GetEnemyPos();
	void		SetPos(D3DXVECTOR3 pos);
	void		Update(Ball* ball);
	void		Start();
	void		SetIP(char ip[]);
	bool		IsServer() const {return this->mConn->IsServer();}

};