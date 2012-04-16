
#include "stdafx.h"
#include "ServerConnection.h"
#include "Ball.h"
using namespace std;
#define PLAYER_CAP 10
class GameNetwork
{
private:
	ServerConnection*	mConn;
	D3DXVECTOR3*		mPos;
	D3DXVECTOR3*		mVel;
	int					mIndex;
	int					mNumPlayers;
	bool				mKeyInputs[PLAYER_CAP][256];
	D3DXVECTOR3 		mStartPositions[PLAYER_CAP];

	/*! Updates the client side, (updates LAN - variables). */
	bool				ClientUpdate();

	/*! Updates the server side, (updates LAN - variables). */
	void				ServerUpdate();
public:
				GameNetwork();
	virtual		~GameNetwork();
	
	/*! Returns the start position on the map of the player with specified index. */
	D3DXVECTOR3 GetStartPos(const int index) const {return this->mStartPositions[index];}

	/*! Returns the position of the player with specified index. */
	D3DXVECTOR3 GetPos(const int index) const { return this->mPos[index]; }
	
	/*! Returns the velocity of the player with specified index. */
	D3DXVECTOR3 GetVel(const int index) const { return this->mVel[index]; }

	/*! Returns the index that you is on the LAN. */
	int			GetIndex() const {return this->mIndex;}

	/*! Returns the number of players on the LAN. */
	int			GetNumPlayers() const {return this->mNumPlayers;}

	/*! Sets the position of the player with specified index. */
	void		SetPos(const D3DXVECTOR3 pos, const int index);

	/*! Sets the velocity of the player with specified index. */
	void		SetVel(const D3DXVECTOR3 vel, const int index);
	
	/*! Sets the IP that the client will connect to. */
	void		SetIP(char ip[]);

	/*! For client: Sets the key to down/up which will be sent to server. */
	void		AddKeyInput(const char key, const bool down);

	/*! Returns true if the client is pressing the specified key. */
	bool		IsKeyPressed(const char key, const int index) const;

	/*! Calling Server/Client -update and updates the positions/rotations/velocities etc of the balls. */
	bool		Update(Ball** balls, int &numBalls);

	/*! Starts the game network. */
	void		Start();

	/*! Returns true if you is the host. */
	bool		IsServer() const {return this->mConn->IsServer();}

	/*! Closes the game LAN. */
	void		Close();

};