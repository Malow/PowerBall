#pragma once

#include "..\stdafx.h"
#include "ServerConnection.h"
#include "..\Game Objects\PowerBall.h"
#include "MsgHandler.h"
#include "PlayerHistory.h"
#include "CommandHandler.h"
using namespace std;

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

public:
				NetworkBall();
	virtual		~NetworkBall();

	void		AddMovementPowerBall(PowerBall* PowerBall);
	//D3DXVECTOR3 CorrectPosition(float serverExecTime);


	void		SetTeam(TEAM team){this->mTeam = team;}
	TEAM		GetTeam() const {return this->mTeam;}

	void		SetHP(float hp) { this->mHP = hp; }
	float		GetHP() const { return this->mHP; }

	void		SetExecTime(const float time){this->mExecTime = time;}
	float		GetExecTime() const { return this->mExecTime; }

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

};