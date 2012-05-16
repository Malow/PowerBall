
#include "NetworkBall.h"

NetworkBall::NetworkBall()
{
	this->mPos					= D3DXVECTOR3(0,0,0);
	this->mVel					= D3DXVECTOR3(0,0,0);
	this->mStartPos				= D3DXVECTOR3(0,0,0);
	this->mForwardVector		= D3DXVECTOR3(0,0,0);
	this->mStartForwardVector	= D3DXVECTOR3(0,0,0);
	this->mExecTime				= 0.0f;
	this->mTeam					= TEAM::NOTEAM;
	this->mHP					= 0.0f;
	this->mPlayerHistory		= new PlayerHistory();
	this->mCommandHandler		= new CommandHandler();
}
NetworkBall::~NetworkBall()
{
	delete this->mPlayerHistory;
	delete this->mCommandHandler;
}

void NetworkBall::AddKeyInput(char keys[], const int numKeys, const float dt, D3DXVECTOR3 forward, bool countUp)
{
	
	if(countUp)
	{
		static char previousKeys[5] = {0};
		static D3DXVECTOR3 prevForward(0,0,0);
		bool same = true;
		if(prevForward != forward)
			same = false;
		for(int i = 0; i < numKeys; i++)
		{
			if(previousKeys[i] != keys[i])
				same = false;
		}
		if(!same || this->mCommandHandler->Empty())
		{
			this->mCommandHandler->Push(keys, numKeys, dt, forward);
		}
		else
		{
			Command* command = this->mCommandHandler->Back();
			command->ModifyDuration(dt);
		}
	
		for (int i = 0; i < 5; i++)
			previousKeys[i] = 0;
		for (int i = 0; i < numKeys; i++)
			previousKeys[i] = keys[i];

		prevForward = forward;
		this->mExecTime += dt;
	}
	else this->mCommandHandler->Push(keys, numKeys, dt, forward);
}
Command* NetworkBall::GetNextCommand() 
{
	return this->mCommandHandler->Front();
}
void NetworkBall::PopCommand()
{
	this->mCommandHandler->Pop();
}
void NetworkBall::AddMovementPowerBall(PowerBall* PowerBall)
{
	this->mPlayerHistory->AddSnapshot(PowerBall->GetPosition(), this->mExecTime);
}
/*
D3DXVECTOR3 NetworkBall::CorrectPosition(float serverExecTime)
{
	D3DXVECTOR3 mod(0.0f, 0.0f, 0.0f);
	if(serverExecTime > 0 && serverExecTime < this->mExecTime)
	{
		// this->mTime - mLatency;//this->totExectime;// ;//this->totExectime;//this->totExectime;// mLatency;// - 100;// latency(which isnt really latency, just the time since server used the keyinput) can get really low if packets stack up, fix this
		//LATENCY = (TIME PING PACKET RECV - TIME PING PACKET SENT)  + TIME OF THE SUM OF THE KEYCOMMANDS EXECUTED BY SERVER. (execution time)
		
		D3DXVECTOR3 historyPos = this->mPlayerHistory->GetPos(serverExecTime);
		//shadow2->SetPosition(historyPos );
		mod = this->mPos - historyPos;
		
		mod.y = 0;//Ignoring y, maybe remove (weird due to the way collision is implemented, y-value always changes)
		if(D3DXVec3Length(&mod) < 0.11f)
		{
			mod.x = 0;
			mod.z = 0;
		}
	}
	return mod;
}*/