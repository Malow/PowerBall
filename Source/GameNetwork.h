
#include "stdafx.h"
#include "ServerConnection.h"
#include "Ball.h"
using namespace std;
#define PLAYER_CAP 10
#define INTERPOS_MIN 0.025f //if the position difference (vector length) is lesser than this -> sets local position to network position
#define INTERPOS_MAX 5.0f //if the position difference (vector length) is greater than this -> sets local position to network position
#define INTERPOS_MOD 0.85f // 1 = setting local position to latest network position, 0 = ignore network position.

enum GAMEMODE{
	NONE,
	CTF,
	DM,
	KOTH,
	KOTH2
};

class GameNetwork
{
private:
	ServerInfo			mServer;
	ServerConnection*	mConn;
	D3DXVECTOR3*		mPos;
	D3DXVECTOR3*		mVel;
	D3DXVECTOR3*		mFlagPos;
	int					mIndex;
	int					mNumPlayers;
	bool				mKeyInputs[PLAYER_CAP][256];
	D3DXVECTOR3 		mStartPositions[PLAYER_CAP];

	/*! Updates the client side, (updates LAN - variables). */
	bool				ClientUpdate();

	/*! Updates the server side, (updates LAN - variables). */
	void				ServerUpdate();

	/*! Adds a float to the char buffer and counts up the offset. */
	void		AddToBuffer(char* bufOut, int &offsetOut, float in);

	/*! Adds a char to the char buffer and counts up the offset. */
	void		AddToBuffer(char* bufOut, int &offsetOut, char in);

	/*! Adds a d3dxvector3 to the char buffer and counts up the offset. */
	void		AddToBuffer(char* bufOut, int &offsetOut, D3DXVECTOR3 in);
	
	/*! Retrieves a float from the char buffer and counts up the offset. */
	float		GetFromBufferF(char* buf, int &offsetOut);
	
	/*! Retrieves a char from the char buffer and counts up the offset. */
	char		GetFromBufferC(char* buf, int &offsetOut);
	
	/*! Retrieves a d3dxvector3 from the char buffer and counts up the offset. */
	D3DXVECTOR3	GetFromBufferD(char* buf, int &offsetOut);

	/*! Sends CTF parameters to the clients. */
	void		SendCTFParams();
public:
				GameNetwork();
	virtual		~GameNetwork();
	
	/*! Returns the start position on the map of the player with specified index. */
	D3DXVECTOR3 GetStartPos(const int index) const {return this->mStartPositions[index];}

	/*! Returns the position of the player with specified index. */
	D3DXVECTOR3 GetPos(const int index) const { return this->mPos[index]; }
	
	/*! Returns the velocity of the player with specified index. */
	D3DXVECTOR3 GetVel(const int index) const { return this->mVel[index]; }

	/*! Returns the position of the flag with specified index. */
	D3DXVECTOR3 GetFlagPos(const int index) const { return this->mFlagPos[index]; }

	/*! Returns the index that you is on the LAN. */
	int			GetIndex() const {return this->mIndex;}

	/*! Returns the number of players on the LAN. */
	int			GetNumPlayers() const {return this->mNumPlayers;}

	/*! Sets the position of the player with specified index. */
	void		SetPos(const D3DXVECTOR3 pos, const int index);

	/*! Sets the velocity of the player with specified index. */
	void		SetVel(const D3DXVECTOR3 vel, const int index);

	/*! Sets the position of the flag with specified index. */
	void		SetFlagPos(const D3DXVECTOR3 pos, const int index);

	/*! For client: Sets the key to down/up which will be sent to server. */
	void		AddKeyInput(const char key, const bool down);

	/*! Returns true if the client is pressing the specified key. */
	bool		IsKeyPressed(const char key, const int index) const;

	/*! Calling Server/Client -update and updates the positions/rotations/velocities etc of the balls. */
	bool		Update(Ball** balls, int &numBalls, float dt);

	/*! Starts the game network. */
	void		Start(ServerInfo server);

	/*! Returns true if you is the host. */
	bool		IsServer() const {return this->mConn->IsServer();}

	/*! Closes the game LAN. */
	void		Close();
	
	/*! Returns servers active on the LAN. */
	vector<ServerInfo>		FindServers();

};