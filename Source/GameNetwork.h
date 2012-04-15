
#include "stdafx.h"
#include "ServerConnection.h"
#include "Ball.h"
using namespace std;
class GameNetwork
{
private:
	ServerConnection*	mConn;
	int					mPlayerCap;
	D3DXVECTOR3*		mPos;
	D3DXVECTOR3*		mVel;
	int					mIndex;
	int					mNumPlayers;

	void				ClientUpdate();
	void				ServerUpdate();
public:
				GameNetwork();
	virtual		~GameNetwork();
	D3DXVECTOR3 GetPos(const int index);
	void		SetPos(D3DXVECTOR3 pos);
	void		SetVel(D3DXVECTOR3 vel);
	void		Update(Ball** balls, int &numBalls);
	void		Start();
	void		SetIP(char ip[]);
	bool		IsServer() const {return this->mConn->IsServer();}
	int			GetIndex() const {return this->mIndex;}
	int			GetNumPlayers() const{return this->mNumPlayers;}

};