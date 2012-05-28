#pragma once

#include "..\stdafx.h"
#include "NetworkHandler.h"
#include "..\Game Objects\PowerBall.h"
#include "MsgHandler.h"
#include "PlayerHistory.h"
#include "CommandHandler.h"
#include "NetworkBall.h"
#include "OnlineHandler.h"
#include "..\Physics\PhysicsEngine.h"
using namespace std;
#define PLAYER_CAP 10
#define FLOAT_EPSILON 0.001f //used for ignoring very tiny movements
#define INTERPOS_MIN 0.01f //if the position difference (vector length) is lesser than this -> sets local position to network position
#define INTERPOS_MAX 5.0f //if the position difference (vector length) is greater than this -> sets local position to network position
#define INTERPOS_MOD 0.75f // 1 = setting local position to latest network position, 0 = ignore network position.
#define SERVER_SEND_MS 1.0f //MAX 1 PACKET PER XX MILLISECONDS
#define CLIENT_SEND_MS 1.0f //MAX 1 PACKET PER XX MILLISECONDS

class GameNetwork
{
private:
	ServerInfo			mServer;
	NetworkHandler*		mConn;
	D3DXVECTOR3*		mFlagPos;
	int					mIndex;
	int					mNumPlayers;
	//queue<KeyInput*>	mKeyInputs[PLAYER_CAP];
	//create a network-ball class instead of having multiple arrays here.
	NetworkBall**		mNetBalls;
	float				mLatency;
	bool				mIsRunning;
	OnlineHandler*		mOnlineHandler;
	bool				mOnline;
	int					mDropPlayerIndex;

	/*! Updates the client side, (updates LAN - variables). */
	bool				ClientUpdate();

	/*! Updates the server side, (updates LAN - variables). */
	void				ServerUpdate();
	
	void				DropPlayer(PowerBall** PowerBalls, int &numPowerBalls, PhysicsEngine* pe, int index);
public:
				GameNetwork();
	virtual		~GameNetwork();
	bool		IsRunning() const { return this->mIsRunning; }
	void		DropPlayer(int index) { if(index > 0 && index < this->mNumPlayers) mDropPlayerIndex = index; }
	void		SetLatency(float latency){this->mLatency = latency;}
	float		GetLatency() const {return this->mLatency;}
	ServerInfo	ConnectTo(string ip);
	//void		AddMovementPowerBall(PowerBall* PowerBall);
	D3DXVECTOR3 		CorrectPosition();
	void			ProcessAllMsgs() { this->mConn->ProcessMSGs(); }
	ServerInfo  GetServerInfo() const { return this->mServer; }

	NetworkBall*	GetBall(int index) const { return this->mNetBalls[index]; }

	/*! Returns the position of the flag with specified index. */
	D3DXVECTOR3 GetFlagPos(const int index) const { return this->mFlagPos[index]; }

	/*! Returns the index that you is on the LAN. */
	int			GetIndex() const {return this->mIndex;}

	/*! Returns the number of players on the LAN. */
	int			GetNumPlayers() const {return this->mNumPlayers;}
	
	/*! Sets the startpositions of the players (player index follows array order). */
	void		SetStartPosistions(const D3DXVECTOR3 pos[], const int size);

	/*! Sets the start foreard vector of the players (player index follows array order). */
	void		SetStartForwardVectors(const D3DXVECTOR3 forward[], const int size);

	/*! Sets the forwardVectors of the players (player index follows array order). */
	void		SetForwardVectors(const D3DXVECTOR3 forward[], const int size);

	/*! Sets the position of the flag with specified index. */
	void		SetFlagPos(const D3DXVECTOR3 pos, const int index);

	/*! Sets the number of players on the LAN. */
	void		SetNumPlayers(int n) { this->mNumPlayers = n;}

	/*! Sets the index of this player. */
	void		SetIndex(int n) { this->mIndex = n;}

	/*! Calling Server/Client -update and updates the positions/rotations/velocities etc of the PowerBalls. */
	bool		UpdatePowerBall(PowerBall** PowerBalls, int &numPowerBalls, PhysicsEngine* pe, float dt);

	/*! Starts the game network. */
	void		Start(ServerInfo server);

	/*! Starts the game network. */
	void		GoOnline(string accName, string accPass);

	/*! Returns true if you is the host. */
	bool		IsServer() const {return this->mConn->IsServer();}

	/*! Closes the game LAN. */
	void		Close();

	void		Reset();
	
	/*! Returns servers active on the LAN. */
	vector<ServerInfo>		FindServers();

};