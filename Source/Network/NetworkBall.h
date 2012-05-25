#pragma once

#include "..\stdafx.h"
#include "NetworkHandler.h"
#include "..\Game Objects\PowerBall.h"
#include "MsgHandler.h"
#include "PlayerHistory.h"
#include "CommandHandler.h"
using namespace std;
#define DROP_TIMER 5000.0f
#define FLOAT_EPSILON 0.001f //used for ignoring very tiny movements
#define INTERPOS_MIN 0.01f //if the position difference (vector length) is lesser than this -> sets local position to network position
#define INTERPOS_MAX 5.0f //if the position difference (vector length) is greater than this -> sets local position to network position
#define INTERPOS_MOD 0.75f // 1 = setting local position to latest network position, 0 = ignore network position.
#define SERVER_SEND_MS 1.0f //MAX 1 PACKET PER XX MILLISECONDS
#define CLIENT_SEND_MS 1.0f //MAX 1 PACKET PER XX MILLISECONDS
class NetworkBall
{
private:
	D3DXVECTOR3			mPos;
	D3DXVECTOR3			mStartPos;
	D3DXVECTOR3			mForwardVector;
	D3DXVECTOR3			mStartForwardVector;
	D3DXVECTOR3			mVel;
	PlayerHistory*		mPlayerHistory;
	CommandHandler*		mCommandHandler;
	float				mHP;
	TEAM				mTeam;
	float				mExecTime;
	float				mAliveTimer;
	
	//Client variables
	bool				mIsPredictingCollision;
	//Server variables
	float				mClientExecTime;

	D3DXVECTOR3 CorrectPosition(NetworkBall* serverBall);
public:
				NetworkBall();
	virtual		~NetworkBall();
	
	void		SyncPosWithServer(PowerBall* ball, NetworkBall* serverBall);
	void		AddMovementPowerBall(PowerBall* PowerBall);

	int			GetNumCommands() const { return this->mCommandHandler->GetNumCommands(); }
	void		ModifyAliveTime(float mod) { this->mAliveTimer += mod; }
	void		SetAliveTime(float time) { this->mAliveTimer = time; }
	float		GetAliveTime() const {return this->mAliveTimer;}
	void		ResetAliveTime() { this->mAliveTimer = DROP_TIMER; }
	void		SetTeam(TEAM team){this->mTeam = team;}
	TEAM		GetTeam() const {return this->mTeam;}

	void		SetHP(float hp) { this->mHP = hp; }
	float		GetHP() const { return this->mHP; }

	void		SetExecTime(const float time){this->mExecTime = time;}
	float		GetExecTime() const { return this->mExecTime; }

	void		StartCollisionPrediction(PowerBall* ball);
	void		PredictCollision(PowerBall* ball, float diff, float serverExecTimeDifference);
	bool		IsPredictingCollision() { return this->mIsPredictingCollision; }
	
	void		SetClientExecTime(const float time){this->mClientExecTime = time;}
	float		GetClientExecTime() const { return this->mClientExecTime; }
	void		ModifyClientExecTime(float mod) {this->mClientExecTime += mod;}
	void		Reset();
	void		ClearCommands() { this->mCommandHandler->Clear(); }
	PlayerHistory* GetPlayerHistory() { return this->mPlayerHistory; }
	//CommandHandler& GetCommandHandler() { return this->

	/*! Returns the start position on the map of the player with specified index. */
	D3DXVECTOR3 GetStartPos() const {return this->mStartPos;}

	/*! Returns the start forward vectors for the ball with the specified index. */
	D3DXVECTOR3 GetStartForwardVector() const {return this->mStartForwardVector;}

	/*! Returns the position of the player with specified index. */
	D3DXVECTOR3 GetPos() const { return this->mPos; }
	
	/*! Returns the velocity of the player with specified index. */
	D3DXVECTOR3 GetVel() const { return this->mVel; }
	
	/*! Returns the forward vector of the player with specified index. */
	D3DXVECTOR3 GetForwardVector() const { return this->mForwardVector; }

	/*! Sets the position of the player with specified index. */
	void		SetPos(const D3DXVECTOR3 pos) { this->mPos = pos; }

	/*! Sets the startposition of the player with specified index. */
	void		SetStartPos(const D3DXVECTOR3 pos) { this->mStartPos = pos; }
	
	/*! Sets the velocity of the player with specified index. */
	void		SetVel(const D3DXVECTOR3 vel) { this->mVel = vel; }

	/*! Sets the forwardVectors of the player with specified index. */
	void		SetForwardVector(const D3DXVECTOR3 forward) { this->mForwardVector = forward; }

	/*! Sets the start forward vector  of the player with specified index. */
	void		SetStartForwardVector(const D3DXVECTOR3 forward) { this->mStartForwardVector = forward; }

	/*! For client: Sets the key to down/up which will be sent to server. */
	void		AddKeyInput(char keys[], const int numKeys, const float dt, D3DXVECTOR3 forward, bool countUp = true);

	/*! For client: Sets the key to down/up which will be sent to server. */
	void		ResetKeyInput(const char key);
	
	/*! Returns true if the client is pressing the specified key. */
	Command* 	GetNextCommand();
	void		PopCommand();

	Snapshot	GetThePowerBallPositionFromThePast(float latency);

};