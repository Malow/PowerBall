
#include "GameNetwork.h"

GameNetwork::GameNetwork()
{
	this->mConn			= new NetworkHandler();
	this->mOnlineHandler  = new OnlineHandler();
	this->mFlagPos		= new D3DXVECTOR3[2];
	this->mIndex		= 0;
	this->mNumPlayers	= 1;
	this->mIsRunning	= true;
	this->mLatency		= 50.0f;
	this->mNetBalls		= new NetworkBall*[PLAYER_CAP];
	this->mDropPlayerIndex = -1;
	for(int i = 0; i < PLAYER_CAP; i++)
	{
		this->mNetBalls[i] = new NetworkBall();
	}
	MsgHandler::GetInstance().Set(this, this->mConn);

	this->mOnline		= false;
	//this->GoOnline("Puserball", "Passerball");
}
GameNetwork::~GameNetwork()
{
	SAFE_DELETE(this->mConn);
	SAFE_DELETE(this->mOnlineHandler);
	for(int i = 0; i < PLAYER_CAP; i++)
	{
		SAFE_DELETE(this->mNetBalls[i]);
	}
	SAFE_DELETE_ARRAY(this->mNetBalls);
	SAFE_DELETE_ARRAY(this->mFlagPos);
}

void GameNetwork::SetForwardVectors(const D3DXVECTOR3 forward[], const int size)
{
	for(int i = 0; i < size; i++)
	{
		this->mNetBalls[i]->SetForwardVector(forward[i]);
	}
}


void GameNetwork::SetStartForwardVectors(const D3DXVECTOR3 forward[], const int size)
{
	for(int i = 0; i < size; i++)
	{
		this->mNetBalls[i]->SetStartForwardVector(forward[i]);
	}
}

void GameNetwork::SetStartPosistions(const D3DXVECTOR3 pos[], const int size)
{
	for(int i = 0; i < size; i++)
	{
		this->mNetBalls[i]->SetStartPos(pos[i]);
	}
}
void GameNetwork::SetFlagPos(const D3DXVECTOR3 pos, const int index)
{
	this->mFlagPos[index] = pos;
}
bool GameNetwork::ClientUpdate()
{
	bool ret = true;
	/*
		Valve limits client sends to 30 packets per second.
	*/
	MsgHandler::GetInstance().SendClientData();
	return ret;
}

void GameNetwork::ServerUpdate()
{
	/*
		Clients usually have only a limited amount of available bandwidth. 
		In the worst case, players with a modem connection can't receive more than 5 to 7 KB/sec. If the server tried to send them updates with a higher data rate, 
		packet loss would be unavoidable. Therefore, the client has to tell the server its incoming bandwidth capacity by setting the console variable rate (in bytes/second). 
		This is the most important network variable for clients and it has to be set correctly for an optimal gameplay experience. 
		The client can request a certain snapshot rate by changing cl_updaterate (default 20), but the server will never send more updates than simulated ticks or exceed the requested client rate limit. 
		Server admins can limit data rate values requested by clients with sv_minrate and sv_maxrate (both in bytes/second). 
		Also the snapshot rate can be restricted with sv_minupdaterate and sv_maxupdaterate (both in snapshots/second).


		Game data is compressed using delta compression to reduce network load. 
		That means the server doesn't send a full world snapshot each time, 
		but rather only changes (a delta snapshot) that happened since the last acknowledged update. With each packet sent between the client and server, 
		acknowledge numbers are attached to keep track of their data flow. 
		Usually full (non-delta) snapshots are only sent when a game starts or a client suffers from heavy packet loss for a couple of seconds. 
		Clients can request a full snapshot manually with the cl_fullupdate command.


		Responsiveness, or the time between user input and its visible feedback in the game world, are determined by lots of factors, 
		including the server/client CPU load, simulation tickrate, data rate and snapshot update settings, but mostly by the network packet traveling time. 
		The time between the client sending a user command, the server responding to it, and the client receiving the server's response is called the latency or ping (or round trip time). 
		Low latency is a significant advantage when playing a multiplayer online game. 
		Techniques like prediction and lag compensation try to minimize that advantage and allow a fair game for players with slower connections


		The lag compensation system keeps a history of all recent player positions for one second. 
		If a user command is executed, the server estimates at what time the command was created as follows:
			Command Execution Time = Current Server Time - Packet Latency - Client View Interpolation
	*/


	//Perhaps add some rules here, so doesnt send stuff that isnt necessary to update (like a player standing still)
	MsgHandler::GetInstance().SendServerData();
	
}
//temporary test variables
int test = 0;
//PowerBall* shadow;
//PowerBall* shadow2;
ofstream file2;
D3DXVECTOR3 interpolationVector(0,0,0);

bool GameNetwork::UpdatePowerBall(PowerBall**	PowerBalls, int &numPowerBalls, PhysicsEngine* pe, float dt)
{
	static float counter = 0.0f;
	counter += dt;
	
	if(this->IsServer())
	{
		if(counter > SERVER_SEND_MS) 
		{
			counter = fmodf(counter, SERVER_SEND_MS);
		
			if(this->mNumPlayers != this->mConn->GetNumConnections())
			{
				this->mNumPlayers = this->mConn->GetNumConnections();
				/*
				Here: Do w/e u want to happen when a new player joins the game.
				for(int i = 0; i < this->mNumPlayers - 1; i++)
				{
					this->mNetBalls[i]->SetPos(this->mNetBalls[i]->GetStartPos());
					PowerBalls[i]->SetPosition(this->mNetBalls[i]->GetStartPos());
				}*/
			}
			if(this->mConn->GetNumConnections() > 1)
			{
				this->ServerUpdate();
				for(int i = 1; i < this->mNumPlayers; i++)
				{
					this->mNetBalls[i]->ModifyAliveTime(-dt);
					if(this->mNetBalls[i]->GetAliveTime() <= 0.0f)
					{
						this->DropPlayer(PowerBalls, numPowerBalls, pe, i);
						MsgHandler::GetInstance().SendDropPlayer(i);
						this->mConn->DropPlayer(i);
					}
				}
			}
		}
	}
	else //Is client
	{
		if(counter > CLIENT_SEND_MS)
		{
			counter = fmodf(counter, CLIENT_SEND_MS);
			this->ClientUpdate();
		}
		
		if(test == 0)
		{
			file2.open("testdata.txt", ios::out);
			//shadow = new PowerBall("Media/PowerBall.obj", D3DXVECTOR3(0,30.0f,-15));
			//shadow2 = new PowerBall("Media/PowerBall.obj", D3DXVECTOR3(0,30.0f,-15));
			test++;
		}
		//shadow->SetPosition(this->mPos[this->mIndex]);
		/*static float ping_counter = 0.0f;
		ping_counter++;
		if(ping_counter > 10000.0f)
		{
			//if no answer, then client dc
			ping_counter = 0.0f;
			MsgHandler::GetInstance().Ping(0);
		}*/
		//file2 << this->mLatency << endl;
		if(numPowerBalls > this->mIndex)
		{
			this->mNetBalls[this->mIndex]->SyncPosWithServer(PowerBalls[this->mIndex], this->mNetBalls[0]);

			//D3DXVECTOR3 mod = this->CorrectPosition();//(0,0,0);//
			/*
				Have a interpolation vector and add mod to it all the time and then in the update loop u slowly subtracts from it so it become a smooth movement and not as spiky as now.
				
			*/
			//interpolation, MAYBE MOVE TO PLAYERHISTORY
			/*interpolationVector += mod;
			float length = D3DXVec3Length(&interpolationVector);

			if(length < INTERPOS_MIN) // the difference is so little or too great so just replace the local pos with network pos.
			{
				if(length > FLOAT_EPSILON) //float epsilon
				{
					PowerBalls[this->mIndex]->SetPosition(PowerBalls[this->mIndex]->GetPosition() + interpolationVector);
					PowerBalls[this->mIndex]->Rotate(interpolationVector);
					this->mPlayerHistories[this->mIndex].MoveHistory(interpolationVector); //a bit inefficient, add an offset vector in player_history that u add to GetPos()
					interpolationVector = D3DXVECTOR3(0,0,0);

				}
			}
			else if (length > INTERPOS_MAX) //May be caused my lag or external movement of the PowerBall.
			{
				PowerBalls[this->mIndex]->SetPosition(this->mPos[this->mIndex]);
				this->mPlayerHistories[this->mIndex].Reset(this->mStartPositions[this->mIndex]);
				interpolationVector = D3DXVECTOR3(0,0,0);
			}
			else
			{
				D3DXVECTOR3 modifier = interpolationVector * dt * 0.001f * INTERPOS_MOD;
				interpolationVector -= modifier;
				PowerBalls[this->mIndex]->SetPosition(PowerBalls[this->mIndex]->GetPosition() + modifier);
				PowerBalls[this->mIndex]->Rotate(modifier);
				this->mPlayerHistories[this->mIndex].MoveHistory(modifier); //a bit inefficient, add an offset vector in player_history that u add to GetPos()
			}*/

			//NO iNTERPOLATION, MIGHT GET SPIKY. Smooth correction needs a bit more work.
			//MODIFY VELOCITY TOO?
			/*interpolationVector = mod;
			if(D3DXVec3Length(&interpolationVector) > FLOAT_EPSILON*10000)
			{
				//PowerBalls[this->mIndex]->AddForce(interpolationVector);
				
				PowerBalls[this->mIndex]->SetPosition(PowerBalls[this->mIndex]->GetPosition() + interpolationVector );
				PowerBalls[this->mIndex]->SetTempPosition(PowerBalls[this->mIndex]->GetTempPosition() + interpolationVector );
				PowerBalls[this->mIndex]->UpdatePost();
				PowerBalls[this->mIndex]->Update(dt);
				PowerBalls[this->mIndex]->Rotate(interpolationVector); //- : rotate other way :D
				this->mNetBalls[this->mIndex]->GetPlayerHistory()->MoveHistory(interpolationVector); //a bit inefficient, add an offset vector in player_history that u add to GetPos()
				interpolationVector = ::D3DXVECTOR3(0,0,0);
			}*/
			/**//*
			if(1 == 2)
			{
			if(PowerBalls[this->mIndex]->GetPosition().y > Y_LEVEL_BOUNDARY)
			{
				interpolationVector = mod;
				float length = D3DXVec3Length(&interpolationVector);
				if(length > INTERPOS_MIN && length < INTERPOS_MAX)//FLOAT_EPSILON)
				{
					PowerBalls[this->mIndex]->SetPosition(PowerBalls[this->mIndex]->GetPosition() + interpolationVector * INTERPOS_MOD);
					PowerBalls[this->mIndex]->SetTempPosition(PowerBalls[this->mIndex]->GetTempPosition() + interpolationVector * INTERPOS_MOD);
				//	PowerBalls[this->mIndex]->GetMesh()->SetPosition(PowerBalls[this->mIndex]->GetPosition() + interpolationVector * INTERPOS_MOD );
					PowerBalls[this->mIndex]->Rotate(interpolationVector * INTERPOS_MOD); 
					this->mNetBalls[this->mIndex]->GetPlayerHistory()->MoveHistory(interpolationVector * INTERPOS_MOD); //a bit inefficient, add an offset vector in player_history that u add to GetPos()
					interpolationVector -= interpolationVector * INTERPOS_MOD;
				}
				else if(length >= INTERPOS_MAX)
				{
					PowerBalls[this->mIndex]->SetPosition(this->mNetBalls[this->mIndex]->GetPos());
					PowerBalls[this->mIndex]->SetTempPosition(this->mNetBalls[this->mIndex]->GetPos());
				//	PowerBalls[this->mIndex]->GetMesh()->SetPosition(this->mNetBalls[this->mIndex]->GetPos());
					this->mNetBalls[this->mIndex]->GetPlayerHistory()->Reset(this->mNetBalls[this->mIndex]->GetPos()); //a bit inefficient, add an offset vector in player_history that u add to GetPos()
				
					PowerBalls[this->mIndex]->Rotate(interpolationVector); 
					interpolationVector = ::D3DXVECTOR3(0,0,0);
				}
				else if (length <= INTERPOS_MIN && length > FLOAT_EPSILON)
				{
					PowerBalls[this->mIndex]->SetPosition(PowerBalls[this->mIndex]->GetPosition() + interpolationVector);
					PowerBalls[this->mIndex]->SetTempPosition(PowerBalls[this->mIndex]->GetTempPosition() + interpolationVector);
				//	PowerBalls[this->mIndex]->GetMesh()->SetPosition(PowerBalls[this->mIndex]->GetPosition() + interpolationVector);
					PowerBalls[this->mIndex]->Rotate(interpolationVector); 
					//this->mNetBalls[this->mIndex]->GetPlayerHistory()->MoveHistory(interpolationVector);
					this->mNetBalls[this->mIndex]->GetPlayerHistory()->Reset(this->mNetBalls[this->mIndex]->GetPos());
					interpolationVector = ::D3DXVECTOR3(0,0,0);
				}
			}
			}
			if(PowerBalls[this->mIndex]->GetPosition().y > Y_LEVEL_BOUNDARY)
			{
				interpolationVector += mod;
				if(D3DXVec3Length(&interpolationVector) > INTERPOS_MIN && D3DXVec3Length(&interpolationVector) < INTERPOS_MAX)//FLOAT_EPSILON)
				{
					PowerBalls[this->mIndex]->SetPosition(PowerBalls[this->mIndex]->GetPosition() + interpolationVector * INTERPOS_MOD);
					PowerBalls[this->mIndex]->SetTempPosition(PowerBalls[this->mIndex]->GetTempPosition() + interpolationVector * INTERPOS_MOD);
					//PowerBalls[this->mIndex]->GetMesh()->SetPosition(PowerBalls[this->mIndex]->GetPosition() + interpolationVector * INTERPOS_MOD );
					//PowerBalls[this->mIndex]->Rotate(interpolationVector * INTERPOS_MOD); 
					this->mNetBalls[this->mIndex]->GetPlayerHistory()->MoveHistory(interpolationVector * INTERPOS_MOD); //a bit inefficient, add an offset vector in player_history that u add to GetPos()
					interpolationVector -= interpolationVector * INTERPOS_MOD;
				}
				else if(D3DXVec3Length(&interpolationVector) > 0.1f)
				{
					D3DXVECTOR3 rot =  this->mNetBalls[this->mIndex]->GetPos() - PowerBalls[this->mIndex]->GetPosition();
					PowerBalls[this->mIndex]->SetPosition(this->mNetBalls[this->mIndex]->GetPos());
					PowerBalls[this->mIndex]->SetTempPosition(this->mNetBalls[this->mIndex]->GetPos());
					//PowerBalls[this->mIndex]->GetMesh()->SetPosition(this->mNetBalls[this->mIndex]->GetPos());
					this->mNetBalls[this->mIndex]->GetPlayerHistory()->Reset(this->mNetBalls[this->mIndex]->GetPos()); //a bit inefficient, add an offset vector in player_history that u add to GetPos()
				
					//PowerBalls[this->mIndex]->Rotate(rot); 
					interpolationVector = ::D3DXVECTOR3(0,0,0);

				}
			}*/
		}
		
		for(int i = 0; i < numPowerBalls; i++)
		{
			if(this->mNetBalls[i]->IsPredictingCollision() && i != this->mIndex)
				this->mNetBalls[i]->PredictCollision(PowerBalls[i], dt, this->mNetBalls[this->mIndex]->GetExecTime() - this->mNetBalls[0]->GetClientExecTime());
			
		}
		if(numPowerBalls > this->mNumPlayers)
		{
			this->DropPlayer(PowerBalls, numPowerBalls, pe,  this->mDropPlayerIndex);
		}
		
		this->mNetBalls[0]->ModifyAliveTime(-dt);
		if(this->mNetBalls[0]->GetAliveTime() <= 0.0f)
		{
			this->mConn->Close();
			this->mIsRunning = false;
		}
	}
	return this->mIsRunning;
}
void GameNetwork::DropPlayer(PowerBall** PowerBalls, int &numPowerBalls, PhysicsEngine* pe, int index)
{
	if(index > 0 && index < this->mNumPlayers)
	{
		pe->RemoveBody(PowerBalls[index]);
		NetworkBall* temp = this->mNetBalls[index];
		this->mNetBalls[index] = this->mNetBalls[--this->mNumPlayers];
		::D3DXVECTOR3 startPos = this->mNetBalls[index]->GetStartPos();
		::D3DXVECTOR3 startForward = this->mNetBalls[index]->GetStartForwardVector();
		this->mNetBalls[index]->SetStartPos(temp->GetStartPos());
		this->mNetBalls[index]->SetStartForwardVector(temp->GetStartForwardVector());
		temp->SetStartPos(startPos);
		temp->SetStartForwardVector(startForward);
		temp->Reset();
		this->mNetBalls[this->mNumPlayers] = temp;
		delete PowerBalls[index];
		PowerBalls[index] = PowerBalls[--numPowerBalls];
	}
}
ServerInfo GameNetwork::ConnectTo(string ip)
{
	this->mServer = this->mConn->ConnectTo(ip);

	for(int i = 0; i < PLAYER_CAP; i++)
	{
		this->mNetBalls[i]->SetPos(this->mNetBalls[i]->GetStartPos());
		this->mNetBalls[i]->SetHP(((WARLOCKInfo*)this->mServer.GetGameModeInfo())->GetStartHealth());
	}
	this->mNetBalls[this->mIndex]->GetPlayerHistory()->Reset(this->mNetBalls[this->mIndex]->GetStartPos());

	return this->mServer;
}
void GameNetwork::Start(ServerInfo server)
{
	this->mServer = server;
	for(int i = 0; i < PLAYER_CAP; i++)
	{
		this->mNetBalls[i]->SetPos(this->mNetBalls[i]->GetStartPos());
		this->mNetBalls[i]->SetHP(((WARLOCKInfo*)server.GetGameModeInfo())->GetStartHealth());
	}
	this->mNetBalls[this->mIndex]->GetPlayerHistory()->Reset(this->mNetBalls[this->mIndex]->GetStartPos());

	if(!this->mOnline)
	{

		if(this->mServer.GetIP() == "")
		{
			mConn->Host(server);
		}
		else 
		{
			mConn->Connect(server);
		}
	}
	else
	{
		if(server.GetID() == -1)
		{
			char create[1024] = "CREATE GAME";
			int offset = 12;
			server.GetBuffer(create, offset);
			create[offset++] = 10;
			this->mOnlineHandler->Send(create, offset);
		}
		else
		{
			char join[55] = "JOIN GAME";
			int offset = 10;
			AddToBuf(join, offset, server.GetID());
			join[offset++] = 10;
			this->mOnlineHandler->Send(join, offset);
		}
	}
	
}
void GameNetwork::GoOnline(string accName, string accPass)
{
	this->mOnline = true;
	this->mOnlineHandler->Connect(accName, accPass);
}
void GameNetwork::Reset()
{
	for(int i = 0; i < PLAYER_CAP; i++)
	{
		this->mNetBalls[i]->Reset();
	}
}
void GameNetwork::Close()
{
	this->mIsRunning = false;
	if(this->IsServer())
	{
		MsgHandler::GetInstance().SendClose();
	}
	this->mConn->Close();
}
vector<ServerInfo> GameNetwork::FindServers()
{
	if(this->mOnline)
		return this->mOnlineHandler->FindServers();
	else return this->mConn->FindServers();
}