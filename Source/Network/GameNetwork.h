#pragma once

#include "..\stdafx.h"
#include "ServerConnection.h"
#include "..\Game Objects\PowerBall.h"
#include "MsgHandler.h"
#include "PlayerHistory.h"
#include "CommandHandler.h"
using namespace std;
#define PLAYER_CAP 10
#define FLOAT_EPSILON 0.01f //used for ignoring very tiny movements
#define INTERPOS_MIN .3f //if the position difference (vector length) is lesser than this -> sets local position to network position
#define INTERPOS_MAX 10.0f //if the position difference (vector length) is greater than this -> sets local position to network position
#define INTERPOS_MOD 0.75f // 1 = setting local position to latest network position, 0 = ignore network position.
#define SERVER_SEND_MS 25.0f //MAX 1 PACKET PER XX MILLISECONDS
#define CLIENT_SEND_MS 50.0f //MAX 1 PACKET PER XX MILLISECONDS

class GameNetwork
{
private:
	ServerInfo			mServer;
	ServerConnection*	mConn;
	D3DXVECTOR3*		mPos;
	D3DXVECTOR3*		mVel;
	D3DXVECTOR3*		mFlagPos;
	PlayerHistory		mPlayerHistories[PLAYER_CAP];
	int					mIndex;
	int					mNumPlayers;
	//queue<KeyInput*>	mKeyInputs[PLAYER_CAP];
	CommandHandler		mCommandHandlers[PLAYER_CAP];
	D3DXVECTOR3 		mStartPositions[PLAYER_CAP];
	D3DXVECTOR3			mForwardVectors[PLAYER_CAP];
	float				mExecTime[PLAYER_CAP];
	float				mLatency;
	bool				mIsRunning;

	/*! Updates the client side, (updates LAN - variables). */
	bool				ClientUpdate();

	/*! Updates the server side, (updates LAN - variables). */
	void				ServerUpdate();

public:
				GameNetwork();
	virtual		~GameNetwork();
	void		SetLatency(float latency){this->mLatency = latency;}
	float		GetLatency() const {return this->mLatency;}
	void		AddMovementPowerBall(PowerBall* PowerBall);
	D3DXVECTOR3 		CorrectPosition();

	void SetServerExecTime(const float time){this->mExecTime[0] = time;}
	void SetExecTime(const float time, const int index){this->mExecTime[index] = time;}
	float GetServerExecTime() const {return this->mExecTime[0];}
	float GetExecTime(const int index) const {return this->mExecTime[index];}
	
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
	
	/*! Returns the forward vector of the player with specified index. */
	D3DXVECTOR3 GetForwardVector(const int index) const { return this->mForwardVectors[index]; }

	/*! Sets the position of the player with specified index. */
	void		SetPos(const D3DXVECTOR3 pos, const int index);

	/*! Sets the startposition of the player with specified index. */
	void		SetStartPos(const D3DXVECTOR3 pos, const int index);
	
	/*! Sets the startpositions of the players (player index follows array order). */
	void		SetStartPosistions(const D3DXVECTOR3 pos[], const int size);

	/*! Sets the velocity of the player with specified index. */
	void		SetVel(const D3DXVECTOR3 vel, const int index);

	/*! Sets the forwardVectors of the player with specified index. */
	void		SetForwardVector(const D3DXVECTOR3 forward, const int index);

	/*! Sets the forwardVectors of the players (player index follows array order). */
	void		SetForwardVectors(const D3DXVECTOR3 forward[], const int size);

	/*! Sets the position of the flag with specified index. */
	void		SetFlagPos(const D3DXVECTOR3 pos, const int index);

	/*! Sets the number of players on the LAN. */
	void		SetNumPlayers(int n) { this->mNumPlayers = n;}

	/*! Sets the index of this player. */
	void		SetIndex(int n) { this->mIndex = n;}

	/*! For client: Sets the key to down/up which will be sent to server. */
	void		AddKeyInput(const int index, char keys[], const int numKeys, const float dt, D3DXVECTOR3 forward);

	/*! For client: Sets the key to down/up which will be sent to server. */
	void		ResetKeyInput(const int index, const char key);

	/*! Returns true if the client is pressing the specified key. */
	float 		IsKeyPressed(const char key, const int index) const;
	
	/*! Returns true if the client is pressing the specified key. */
	Command* 	GetNextCommand(const int index);
	void		PopCommand(const int index);

	/*! Calling Server/Client -update and updates the positions/rotations/velocities etc of the PowerBalls. */
	bool		UpdatePowerBall(PowerBall** PowerBalls, int &numPowerBalls, float dt);

	/*! Starts the game network. */
	void		Start(ServerInfo server);

	/*! Returns true if you is the host. */
	bool		IsServer() const {return this->mConn->IsServer();}

	/*! Closes the game LAN. */
	void		Close();
	
	/*! Returns servers active on the LAN. */
	vector<ServerInfo>		FindServers();

};