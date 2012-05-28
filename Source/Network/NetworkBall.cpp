
#include "NetworkBall.h"

NetworkBall::NetworkBall()
{
	this->mPos					= D3DXVECTOR3(0,30,0);
	this->mVel					= D3DXVECTOR3(0,0,0);
	this->mStartPos				= D3DXVECTOR3(0,30,0);
	this->mForwardVector		= D3DXVECTOR3(0,0,1);
	this->mStartForwardVector	= D3DXVECTOR3(0,0,1);
	this->mExecTime				= 0.0f;
	this->mTeam					= TEAM::NOTEAM;
	this->mHP					= 0.0f;
	this->mAliveTimer			= DROP_TIMER;
	this->mClientExecTime		= 0.0f;
	this->mIsPredictingCollision= false;
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
	Vector3 asd = PowerBall->GetTempPosition();
	this->mPlayerHistory->AddSnapshot(PowerBall->GetPosition(), D3DXVECTOR3(asd.x, asd.y, asd.z), this->mExecTime);
}
void NetworkBall::Reset()
{
	delete this->mPlayerHistory;
	delete this->mCommandHandler;
	this->mPlayerHistory		= new PlayerHistory();
	this->mCommandHandler		= new CommandHandler();
	this->ResetAliveTime();
	this->mPos					= this->mStartPos;
	this->mForwardVector		= this->mStartForwardVector;
	this->mVel					= D3DXVECTOR3(0,0,0);
	this->mExecTime				= 0.0f;
	//this->mTeam					= TEAM::NOTEAM;
	this->mHP					= 0.0f;
	this->mAliveTimer			= DROP_TIMER;
	this->mClientExecTime		= 0.0f;
	this->mIsPredictingCollision= false;
}

Snapshot NetworkBall::GetThePowerBallPositionFromThePast(float latency)
{
	float timeDifference = latency;//this->mClientExecTime - this->mExecTime;
	if(timeDifference <= 0.0f)
	{
		int asd = 0;
	}
	return this->mPlayerHistory->GetSnapshot(timeDifference);
}
void NetworkBall::SyncPosWithServer(PowerBall* ball, NetworkBall* serverBall)
{
	if(ball->GetPosition().y > Y_LEVEL_BOUNDARY)
	{
		D3DXVECTOR3 interpolationVector = this->CorrectPosition(serverBall);
		float length = D3DXVec3Length(&interpolationVector);
		if(length > INTERPOS_MIN && length < INTERPOS_MAX)//FLOAT_EPSILON)
		{
			ball->SetPosition(ball->GetPosition() + interpolationVector * INTERPOS_MOD);
			ball->SetTempPosition(ball->GetTempPosition() + interpolationVector * INTERPOS_MOD);
		//	ball->GetMesh()->SetPosition(ball->GetPosition() + interpolationVector * INTERPOS_MOD );
			ball->Rotate(interpolationVector * INTERPOS_MOD); 
			this->mPlayerHistory->MoveHistory(interpolationVector * INTERPOS_MOD); //a bit inefficient, add an offset vector in player_history that u add to GetPos()
			interpolationVector -= interpolationVector * INTERPOS_MOD;
		}
		else if(length >= INTERPOS_MAX)
		{
			ball->SetPosition(this->GetPos());
			ball->SetTempPosition(this->GetPos());
		//	ball->GetMesh()->SetPosition(this->GetPos());
			this->GetPlayerHistory()->Reset(this->GetPos()); //a bit inefficient, add an offset vector in player_history that u add to GetPos()
				
			ball->Rotate(interpolationVector); 
			interpolationVector = ::D3DXVECTOR3(0,0,0);
		}
		else if (length <= INTERPOS_MIN && length > FLOAT_EPSILON)
		{
			ball->SetPosition(ball->GetPosition() + interpolationVector);
			ball->SetTempPosition(ball->GetTempPosition() + interpolationVector);
		//	ball->GetMesh()->SetPosition(ball->GetPosition() + interpolationVector);
			ball->Rotate(interpolationVector); 
			//this->GetPlayerHistory()->MoveHistory(interpolationVector);
			this->GetPlayerHistory()->Reset(this->GetPos());
			interpolationVector = ::D3DXVECTOR3(0,0,0);
		}
	}
}
void NetworkBall::StartCollisionPrediction(PowerBall* ball)
{
	this->mPlayerHistory->Reset(ball->GetPosition());
	this->mExecTime					= 0;
	this->mIsPredictingCollision	= true;
	this->AddMovementPowerBall(ball);
}
void NetworkBall::PredictCollision(PowerBall* ball, float diff, float serverExecTimeDifference)
{
	this->mExecTime += diff;
	this->AddMovementPowerBall(ball);
	D3DXVECTOR3 historyPos = this->GetPlayerHistory()->GetPos(this->mExecTime - serverExecTimeDifference);

	D3DXVECTOR3 interpolationVector = this->GetPos() - historyPos;
	float length = D3DXVec3Length(&interpolationVector);
	if(length > INTERPOS_MIN && length < INTERPOS_MAX)//FLOAT_EPSILON)
	{
		ball->SetPosition(ball->GetPosition() + interpolationVector * INTERPOS_MOD);
		ball->SetTempPosition(ball->GetTempPosition() + interpolationVector * INTERPOS_MOD);
	//	ball->GetMesh()->SetPosition(ball->GetPosition() + interpolationVector * INTERPOS_MOD );
		ball->Rotate(interpolationVector * INTERPOS_MOD); 
		this->mPlayerHistory->MoveHistory(interpolationVector * INTERPOS_MOD); //a bit inefficient, add an offset vector in player_history that u add to GetPos()
		interpolationVector -= interpolationVector * INTERPOS_MOD;
	}
	else if(length >= INTERPOS_MAX)
	{
		ball->SetPosition(this->GetPos());
		ball->SetTempPosition(this->GetPos());
	//	ball->GetMesh()->SetPosition(this->GetPos());
		this->GetPlayerHistory()->Reset(this->GetPos()); //a bit inefficient, add an offset vector in player_history that u add to GetPos()
				
		ball->Rotate(interpolationVector); 
		interpolationVector = ::D3DXVECTOR3(0,0,0);
		this->mIsPredictingCollision	= false;
	}
	else if (length <= INTERPOS_MIN && length > FLOAT_EPSILON)
	{
		ball->SetPosition(ball->GetPosition() + interpolationVector);
		ball->SetTempPosition(ball->GetTempPosition() + interpolationVector);
	//	ball->GetMesh()->SetPosition(ball->GetPosition() + interpolationVector);
		ball->Rotate(interpolationVector); 
		//this->GetPlayerHistory()->MoveHistory(interpolationVector);
		this->GetPlayerHistory()->Reset(this->GetPos());
		interpolationVector = ::D3DXVECTOR3(0,0,0);

		//this->mIsPredictingCollision	= false;
	}
	//maybe check here if the serverball and the collision predicted ball is close to eachother then stop predicting.
	D3DXVECTOR3 vec = ball->GetPosition() - this->mPos;
	if(D3DXVec3Length(&vec) < 0.2f && this->mExecTime > 500)
	{
		this->mIsPredictingCollision	= false;
		ball->SetPosition(this->mPos);
		ball->SetTempPosition(this->mPos);
		ball->Rotate(vec);
	}

}

D3DXVECTOR3 NetworkBall::CorrectPosition(NetworkBall* serverBall)
{
	D3DXVECTOR3 mod(0.0f, 0.0f, 0.0f);
	if(serverBall->GetClientExecTime() > 0 && serverBall->GetClientExecTime() < this->GetExecTime())
	{
		// this->mTime - mLatency;//this->totExectime;// ;//this->totExectime;//this->totExectime;// mLatency;// - 100;// latency(which isnt really latency, just the time since server used the keyinput) can get really low if packets stack up, fix this
		//LATENCY = (TIME PING PACKET RECV - TIME PING PACKET SENT)  + TIME OF THE SUM OF THE KEYCOMMANDS EXECUTED BY SERVER. (execution time)
		
		D3DXVECTOR3 historyPos = this->GetPlayerHistory()->GetPos(serverBall->GetClientExecTime());
		//shadow2->SetPosition(historyPos );
		mod = this->GetPos() - historyPos;
		
		mod.y = 0;//Ignoring y, maybe remove (weird due to the way collision is implemented, y-value always changes)
		if(D3DXVec3Length(&mod) < 0.11f)
		{
			mod.x = 0;
			mod.z = 0;
		}
	}
	else if( this->GetExecTime() >  serverBall->GetClientExecTime() + 1)
	{
		this->SetExecTime(serverBall->GetClientExecTime());
	}
	return mod;
}