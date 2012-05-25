#include "PhysicsEngine.h"
#include "..\Game Objects\PowerBall.h"
#include "..\Game Objects\Map.h"
#include "GameTimerSimple.h"
#include "GraphicsEngine.h"
#include "..\SoundEffect.h"
#include "..\Network\GameNetwork.h"
#include "..\Network\ServerInfo.h"

PhysicsEngine::PhysicsEngine(GraphicsEngine* ge)
{
	this->mGe = ge;
	this->mNet = NULL;
	this->mSize = 0;
	this->mCapacity = CAPACITY;
	this->mPowerBalls = new PowerBall*[CAPACITY];
	for(int i = 0;i<CAPACITY;i++)
		this->mPowerBalls[i] = NULL;
	this->mMap = NULL;
	this->mGameTimer = new GameTimerSimple();
	this->mCollisionWithWall = GetGraphicsEngine()->GetSoundEngine()->LoadSoundEffect("Media/Sounds/SoundEffects/ball_vs_wallFIXED.flac");
	this->mCollisionWithBall = GetGraphicsEngine()->GetSoundEngine()->LoadSoundEffect("Media/Sounds/SoundEffects/ball_vs_ballFIXED.wav");
	this->mHud = mGe->CreateText("",D3DXVECTOR2(10,300),1.0f,"Media/Fonts/1");

}

PhysicsEngine::PhysicsEngine(GraphicsEngine* ge, GameNetwork* net, ServerInfo info)
{
	this->mServerInfo = info;
	this->mZoomOutPressed = false;
	this->mZoomInPressed = false;
	this->mGe = ge;
	this->mNet = net;
	this->mSize = 0;
	this->mCapacity = CAPACITY;
	this->mPowerBalls = new PowerBall*[CAPACITY];
	for(int i = 0;i<CAPACITY;i++)
		this->mPowerBalls[i] = NULL;
	this->mMap = NULL;
	this->mGameTimer = new GameTimerSimple();
	this->mCollisionWithWall = GetGraphicsEngine()->GetSoundEngine()->LoadSoundEffect("Media/Sounds/SoundEffects/ball_vs_wall.mp3");
	this->mCollisionWithBall = GetGraphicsEngine()->GetSoundEngine()->LoadSoundEffect("Media/Sounds/SoundEffects/ball_vs_ball.mp3");
	this->mHud = mGe->CreateText("",D3DXVECTOR2(10,300),1.0f,"Media/Fonts/1");
}
PhysicsEngine::~PhysicsEngine()
{
	delete []this->mPowerBalls;
	delete this->mGameTimer;
	this->mGe->DeleteText(this->mHud);
	
}

void PhysicsEngine::Initialize()
{
	this->mGameTimer->Initialize();
}

void PhysicsEngine::AddBody(PowerBall* body)
{
	
	if(this->mCapacity > this->mSize)
	{
		this->mPowerBalls[this->mSize] = body;
		this->mSize++;
	}
	else
	{
		PowerBall** temp = new PowerBall*[this->mCapacity * 2];
		for(int i = 0;i<this->mCapacity;i++)
		{
			temp[i] = this->mPowerBalls[i];
		}
		/* we don't delete the pointers itself just the double pointer array */
		delete []this->mPowerBalls;
		this->mPowerBalls = temp;
		this->mCapacity = this->mCapacity * 2;
		this->mPowerBalls[this->mSize] = body;
		this->mSize++;
	}
}

void PhysicsEngine::AddMap(Map* map)
{
	/* keep it simple */
	this->mMap = map;
}

bool PhysicsEngine::RemoveBody(PowerBall* body)
{
	int removeIndex = -1;
	for(int i = 0;i<this->mSize;i++)
	{
		if(this->mPowerBalls[i] == body)
		{
			removeIndex = i;
			break;
		}
	}
	if(removeIndex != -1)
	{
		if(removeIndex == (this->mSize - 1) )
			this->mPowerBalls[removeIndex] = NULL;
		else
		{
			this->mPowerBalls[removeIndex] = this->mPowerBalls[this->mSize - 1];
			this->mPowerBalls[this->mSize - 1] = NULL;
		}
		this->mSize--;
		return true;
	}
	return false;
}

bool PhysicsEngine::RemoveMap(Map* map)
{
	if(this->mMap == map)
	{
		this->mMap = NULL;
		return true;
	}
	return false;
}
void PhysicsEngine::Simulate(bool clientBall)
{
	bool needToUpdate = this->mGameTimer->Update();
	
	if(!needToUpdate)
		return;
	
	this->mGameTimer->mAccumulator += this->mGameTimer->GetDeltaTime();
	float timeStep = this->mGameTimer->GetTimeStepDt();
	
	
	float oldTime = 0;
	float newTime = 0;
	float loopTime = 0.0f ;
	//MaloW::Debug(timeStep);
	while(this->mGameTimer->mAccumulator >= timeStep )
	{
		oldTime = this->mGameTimer->GetTime();
		this->InputKnockout(timeStep);		
		for(int i = 0;i<this->mSize;i++)
		{
			// euler update 
			//this->mPowerBalls[i]->Update(timeStep, false);
			// for use when RK4 is working 
			this->mPowerBalls[i]->UpdatePhysicsEuler(timeStep);
			this->mPowerBalls[i]->UpdateLogic(timeStep, clientBall);

			//this->mPowerBalls[i]->UpdatePhysics(this->mGameTimer->mT, timeStep);
				
		}						
		PowerBall* b1;
		PowerBall* b2;
		for(int i = 0; i < this->mSize; i++)
		{
			b1 = this->mPowerBalls[i];
			for(int j = i+1; j < this->mSize; j++)
			{
				b2 = this->mPowerBalls[j];
				if(this->CollisionWithSphereSimple(b1, b2) )
					this->CollisionSphereResponse(b1, b2);
			}
			Vector3 normalPlane;
			if(this->CollisionWithMapSimple(b1, this->mMap,normalPlane))
				this->CollisionMapResponse(b1, this->mMap, normalPlane, timeStep);	
		}
		for(int i = 0;i<this->mSize;i++)
		{
			this->mPowerBalls[i]->UpdatePost();
		}
		this->mGameTimer->mAccumulator -= timeStep;
		this->mGameTimer->mT += timeStep;

		newTime = this->mGameTimer->GetTime();
		loopTime = newTime - oldTime;
		MaloW::Debug(loopTime);
	}

	this->mMap->Update(this->mGameTimer->GetDeltaTime());
	
	this->mHud->SetText("Time: " + MaloW::convertNrToString(this->mGameTimer->mT));


	
}

void PhysicsEngine::SimulateServer()
{
	bool needToUpdate = this->mGameTimer->Update();
	
	if(!needToUpdate)
		return;
	
	this->mGameTimer->mAccumulator += this->mGameTimer->GetDeltaTime();
	float timeStep = this->mGameTimer->GetTimeStepDt();
	
	
	float oldTime = 0;
	float newTime = 0;
	float loopTime = 0.0f ;
	//MaloW::Debug(timeStep);
	while(this->mGameTimer->mAccumulator >= timeStep )
	{
			oldTime = this->mGameTimer->GetTime();
			for(int i = 0; i < this->mSize; i++)
			{
				if(i != this->mNet->GetIndex())
					this->HandleClientKeyInputs(i, timeStep);
				else
					this->InputKeysPressedSelf(timeStep, i);	
			}
			for(int i = 0; i < this->mSize; i++)
			{					
				bool clientBall = true;
				if(i == this->mNet->GetIndex())
					clientBall = false;
				this->mPowerBalls[i]->UpdatePhysicsEuler(timeStep);
				this->mPowerBalls[i]->UpdateLogic(timeStep, clientBall);
			}
			PowerBall* b1;
			PowerBall* b2;
			for(int i = 0; i < this->mSize; i++)
			{
				b1 = this->mPowerBalls[i];
				for(int j = i+1; j < this->mSize; j++)
				{
					b2 = this->mPowerBalls[j];
					if(this->CollisionWithSphereSimple(b1, b2) )
						this->CollisionSphereResponse(b1, b2);
				}
				Vector3 normalPlane;
				if(this->CollisionWithMapSimple(b1, this->mMap,normalPlane))
					this->CollisionMapResponse(b1, this->mMap, normalPlane, timeStep);	
			}
			for(int i = 0;i<this->mSize;i++)
				this->mPowerBalls[i]->UpdatePost();
			for(int i = 0; i < this->mSize; i++)
			{
				this->mNet->GetBall(i)->SetPos(this->mPowerBalls[i]->GetPosition());
				Vector3 vel = this->mPowerBalls[i]->GetVelocity();
				this->mNet->GetBall(i)->SetVel(::D3DXVECTOR3(vel.x, vel.y, vel.z));
			}
			this->mMap->Update(timeStep);
			this->mNet->UpdatePowerBall(this->mPowerBalls, this->mSize, timeStep);
			this->mGameTimer->mAccumulator -= timeStep;
			this->mGameTimer->mT += timeStep;

			newTime = this->mGameTimer->GetTime();
			loopTime = newTime - oldTime;
			MaloW::Debug(loopTime);
	}
}

void PhysicsEngine::SimulateClient()
{
	bool needToUpdate = this->mGameTimer->Update();
	
	if(!needToUpdate)
		return;
	
	this->mGameTimer->mAccumulator += this->mGameTimer->GetDeltaTime();
	float timeStep = this->mGameTimer->GetTimeStepDt();
	
	
	float oldTime = 0;
	float newTime = 0;
	float loopTime = 0.0f ;
	//MaloW::Debug(timeStep);
	if(this->mGameTimer->mAccumulator >= timeStep)
		MaloW::Debug(0);
	else 
		MaloW::Debug(1);
	while(this->mGameTimer->mAccumulator >= timeStep )
	{
			oldTime = this->mGameTimer->GetTime();
			for(int i = 0; i < this->mSize; i++)
			{
				if(this->mNet->GetIndex() != i)
				{
					D3DXVECTOR3 rotVector = this->mNet->GetBall(i)->GetPos() - this->mPowerBalls[i]->GetPosition();
					
					this->mPowerBalls[i]->SetPosition(this->mNet->GetBall(i)->GetPos());
					this->mPowerBalls[i]->SetTempPosition(this->mNet->GetBall(i)->GetPos()); //ny
					this->mPowerBalls[i]->Rotate(rotVector);

					if(this->mNet->GetBall(i)->GetNumCommands() > 0)
					{
						this->mPowerBalls[i]->UseSpell((int)this->mNet->GetBall(i)->GetNextCommand()->GetInput(0));
						this->mNet->GetBall(i)->PopCommand();
					}
				
					for(int c = 0; c < this->mPowerBalls[i]->GetNrOfSpells(); c++)
						this->mPowerBalls[i]->GetSpells()[c]->UpdateSpecial(timeStep * 0.001f);
				}
			}
			if(this->mNet->GetIndex() < this->mSize)
			{
				int i = this->mNet->GetIndex();
				this->SendKeyInputs(i, timeStep);
				this->InputKeysPressedSelf(timeStep, i);
				// ny
				for(int b = 0; b < this->mSize; b++)
				{					
					bool clientBall = true;
					if(b == this->mNet->GetIndex())
						clientBall = false;
					this->mPowerBalls[b]->UpdatePhysicsEuler(timeStep);
					this->mPowerBalls[b]->UpdateLogic(timeStep, clientBall);
				}

				//end ny
				PowerBall* b1;
				PowerBall* b2;
				for(int c = 0; c < this->mSize; c++)
				{
					b1 = this->mPowerBalls[c];
					for(int j = c+1; j < this->mSize; j++)
					{
						b2 = this->mPowerBalls[j];
						if(this->CollisionWithSphereSimple(b1, b2))
							this->CollisionSphereResponse(b1,b2);
					}
					
					Vector3 normalPlane;
					if(this->CollisionWithMapSimple(b1, this->mMap,normalPlane))
						this->CollisionMapResponse(b1, this->mMap, normalPlane, timeStep);	
				}
				for(int b = 0;b<this->mSize; b++)
				{
					this->mPowerBalls[i]->UpdatePost();
				}
				// bort
				/*
				Vector3 normalPlane;
				if(this->mPowerBalls[i]->collisionWithPlatformSimple(this->mMap, normalPlane))
					this->mPowerBalls[i]->collisionPlatformResponse(this->mMap, normalPlane, timeStep);
				this->mPowerBalls[i]->UpdatePhysicsEuler(timeStep);
				this->mPowerBalls[i]->UpdateLogic(timeStep, false);
				this->mPowerBalls[i]->UpdatePost();				
				*/

				this->mNet->GetBall(i)->AddMovementPowerBall(this->mPowerBalls[i]);
			}
			this->mMap->Update(timeStep);
			this->mNet->UpdatePowerBall(this->mPowerBalls, this->mSize, timeStep);

			this->mGameTimer->mAccumulator -= timeStep;
			this->mGameTimer->mT += timeStep;

			newTime = this->mGameTimer->GetTime();
			loopTime = newTime - oldTime;
			MaloW::Debug(loopTime);
	}
		
}
int PhysicsEngine::Size() const
{
	return this->mSize;
}

void PhysicsEngine::InputKnockout(float timeStep)
{
	
	// move ball 1
	if(mGe->GetKeyListener()->IsPressed('W'))
		this->mPowerBalls[0]->AddForceForwardDirection(timeStep);	
	if(mGe->GetKeyListener()->IsPressed('S'))
		this->mPowerBalls[0]->AddForceOppositeForwardDirection(timeStep);
	if(mGe->GetKeyListener()->IsPressed('A'))
		this->mPowerBalls[0]->AddForceLeftOfForwardDirection(timeStep);	
	if(mGe->GetKeyListener()->IsPressed('D'))
		this->mPowerBalls[0]->AddForceRightOfForwardDirection(timeStep);	
	
	// move ball 2
	if(mGe->GetKeyListener()->IsPressed(VK_LEFT))
		this->mPowerBalls[1]->AddForceLeftOfForwardDirection(timeStep);	
	if(mGe->GetKeyListener()->IsPressed(VK_RIGHT))
		this->mPowerBalls[1]->AddForceRightOfForwardDirection(timeStep);	
	if(mGe->GetKeyListener()->IsPressed(VK_UP))
		this->mPowerBalls[1]->AddForceForwardDirection(timeStep);	
	if(mGe->GetKeyListener()->IsPressed(VK_DOWN))
		this->mPowerBalls[1]->AddForceOppositeForwardDirection(timeStep);

}

bool PhysicsEngine::CollisionWithSphereSimple(PowerBall* b, PowerBall* b1)
{
	Vector3 r = b->GetPositionVector3() - b1->GetPositionVector3();
	r = b->GetTempPosition() - b1->GetTempPosition();

	float distanceBalls = r.GetLength();
	float sumRadius = b->GetRadius() + b1->GetRadius();
	if(distanceBalls > sumRadius)
		return false;

	/*  we have collision but we need to move the balls so they are not inside each other
	*	solve this equation: ((pos1 - t*vel1) - (pos2 - t*vel2)).length = radie1 + radie2
	*
	*	this gives ut the following:
	*	d	 = distance = p1-p2
	*	rV	 = relative velocity = v2-v1
	*	sumR = sumRadius = r1 + r2 
	*
	*	t    = - rV.dot(d)/|rV|^2 +- sqrt( rV.dot(d)^2/|rV|^4 - (sumR^2 - |d|^2) / |rV|^2
	*
	*/
	Vector3 d = b->GetPositionVector3() - b1->GetPositionVector3();
	d = b->GetTempPosition() - b1->GetTempPosition();

	Vector3 rV = b1->GetVelocity() - b->GetVelocity();
	float sumR = b->GetRadius() + b1->GetRadius();
	float tempA = rV.GetDotProduct(d) / rV.GetLengthSquared();
	float tempB = tempA*tempA;
	float tempC = (d.GetLengthSquared() - sumR*sumR) / rV.GetLengthSquared();
	float tempSq = tempB - tempC;
	if( tempSq < 0) // no real solutions
		return false;
	else
	{
		float t1 = - tempA - sqrt(tempSq);
		float t2 = - tempA + sqrt(tempSq);
		Vector3 newPos1, newPos2;
		if(t1 >= 0)
		{
			/*
			newPos1 = this->GetPositionVector3() - this->mVelocity*t1;
			newPos2 = b1->GetPositionVector3() - b1->mVelocity*t1;
								
			this->SetPosition(newPos1);
			b1->SetPosition(newPos2);
			*/

			newPos1 = b->GetTempPosition() - b->GetVelocity()*t1;
			newPos2 = b1->GetTempPosition() - b1->GetVelocity()*t1;
			b->SetTempPosition(newPos1);
			b1->SetTempPosition(newPos2);
		}
		else if(t2 >= 0)
		{
			/*
			newPos1 = this->GetPositionVector3() - this->mVelocity*t2;
			newPos2 = b1->GetPositionVector3() - b1->mVelocity*t2;
			
			this->SetPosition(newPos1);
			b1->SetPosition(newPos2);
			*/
			newPos1 = b->GetTempPosition() - b->GetVelocity()*t2;
			newPos2 = b1->GetTempPosition() - b1->GetVelocity()*t2;
			b->SetTempPosition(newPos1);
			b1->SetTempPosition(newPos2);
		}
		else 
			return false;
	}
	return true;
}

void PhysicsEngine::CollisionSphereResponse(PowerBall* b, PowerBall* b1)
{
	// normal of the "collision plane"
	//Vector3 nColl = this->GetPositionVector3() - b1->GetPositionVector3();
	Vector3 nColl = b->GetTempPosition() - b1->GetTempPosition();
	// for easy projecting of vector, no div by |n|^2 in proj formula
	Vector3 tempa = nColl;
	nColl.normalize();

	// savning the important stuff for easy acc
	float m1 = b->GetMass();
	float m2 = b1->GetMass();
	float mSum = m1+m2;
	Vector3 v1 = b->GetVelocity();
	Vector3 v2 = b1->GetVelocity();

	// projecting the vector v1 on nColl
	float x1 = nColl.GetDotProduct(v1);		// factor in nColl dir
	Vector3 v1x = nColl*x1;					// projetion done
	Vector3 v1y = v1 - v1x;					// perpendicular vector 

	// projecting the vector v2 on nColl
	nColl = nColl*(-1);						// switching direction of "plane normal"
	float x2 = nColl.GetDotProduct(v2);		// factor in nColl dir
	Vector3 v2x = nColl*x2;					// projetion done
	Vector3 v2y = v2 - v2x;					// perpendicular vector 

	float e1 = b->GetRestitution();
	float e2 = b1->GetRestitution();
	float e = (e1 + e2)/2.0f;

	if(((v1x-v2x).GetLength() > 0.6f) && b->SoundEnabled())
		this->mCollisionWithBall->Play();
	

	/*
	this->mVelocity = Vector3( v1x*(m1-m2)/(mSum) + v2x*(2*m2)/(mSum) + v1y );
	b1->mVelocity = Vector3( v1x*(2*m1)/(mSum) + v2x*(m2-m1)/(mSum) + v2y );
	*/
	b->SetVelocity(Vector3( v1x*(m1-m2*e)/(mSum) + v2x*((1+e)*m2)/(mSum) + v1y ));
	b1->SetVelocity(Vector3( v1x*((1+e)*m1)/(mSum) + v2x*(m2-m1*e)/(mSum) + v2y ));

	
	/* calculating damage. */
	if(b->WarlockMode())
	{
		Vector3 ve1 = b->GetVelocity();
		Vector3 ve2 = b1->GetVelocity();
		float v1Max = b->GetMaxVelocity();
		float v2Max = b1->GetMaxVelocity();
		float v1 = ve1.GetLength();
		float v2 = ve2.GetLength();
		Vector3 relativeV = ve1 - ve2;
		if( relativeV.GetLength() > 0.6f)
		{
			float sumV = v1 + v2;
			float damage1 = v2/sumV;
			float damage2 = v1/sumV;
			float health1 = b->GetHealth();
			health1 -= damage1*10.0f*(v2/v2Max);
			b->SetHealth(health1);
			float health2 = b1->GetHealth();
			health2 -= damage2*10.0f*(v1/v1Max);
			b1->SetHealth(health2);
		}
	}
	
	/* informing the spells to the balls that it has been a collision */
	Spell** spells = b->GetSpells();
	for(int i = 0;i<b->GetNrOfSpells();i++)
		spells[i]->InformCollision();
	spells = b1->GetSpells();
	for(int i = 0;i<b1->GetNrOfSpells();i++)
		spells[i]->InformCollision();

}


bool PhysicsEngine::CollisionWithMapSimple(PowerBall* b, Map* map, Vector3 &normalPlane)
{
	
	MaloW::Array<MeshStrip*>* temp = map->GetMesh()->GetStrips();
	//int sizeMstrip = temp->size();
	int sizeVertexS0 = temp->get(0)->getNrOfVerts();
	Vertex* verts;
	//Vector3 origin = this->GetPositionVector3();
	Vector3 origin = b->GetTempPosition();
	Vector3 dir = b->GetVelocity();
	Vector3 dirN = dir/dir.GetLength();
	verts = temp->get(0)->getVerts();
	/*
	for(int i = 0;i<sizeMstrip;i++)
	{
		
	}
	*/
	Vector3 p0,p1,p2, normal, v1,v2;
	float smalestTime = -1;
	bool firstHit = false;
	float u, v,t;
	float lengthProjN = 0;
	Vector3 p0Store, p1Store,p2Store, normalStore;
	Vector3 pos = Vector3(map->GetMesh()->GetPosition());
	Vector3 posS = b->GetTempPosition();//this->GetPositionVector3();
	Vector3 rayDirection;
	Vector3 scalingMesh = map->GetMesh()->GetScaling();

	D3DXMATRIX quat;
	D3DXMatrixRotationQuaternion(&quat, &map->GetMesh()->GetRotation()); 
	Matrix4 rotate(quat);
	rotate.TransposeThis();

	Matrix4 scaling;
	scaling.SetScale(scalingMesh);

	Matrix4 translate;
	translate.SetTranslate(pos);
	
	Matrix4 world = translate*rotate*scaling;
	
	for(int i =0; i< sizeVertexS0; i+=3)
	{
		
		/*
		p0 = Vector3(verts[i].pos).GetComponentMultiplication(scalingMesh) + pos;
		p1 = Vector3(verts[i+1].pos).GetComponentMultiplication(scalingMesh) +pos;
		p2 = Vector3(verts[i+2].pos).GetComponentMultiplication(scalingMesh) + pos;
		*/
		p0 = world*Vector3(verts[i].pos);
		p1 = world*Vector3(verts[i+1].pos);
		p2 = world*Vector3(verts[i+2].pos);

		v1 = p1-p0;
		v2 = p2-p0;
		rayDirection = v1.GetCrossProduct(v2);
		rayDirection.normalize();
		float tempLength;
		Vector3 ny;
		Vector3 projN;
		if(RayTriIntersect(origin , rayDirection, p0, p1, p2, u, v, t) )
		{
			normal = rayDirection;
			ny = origin - p0;
			projN = normal*ny.GetDotProduct(normal);
			tempLength = projN.GetLength();
			if(!firstHit)
			{
				firstHit = true;
				smalestTime = t;
				lengthProjN = tempLength;
				p0Store = p0;
				p1Store = p1;
				p2Store = p2;
				normalStore = normal;
			}
			else
			{
				if( tempLength < lengthProjN )
				{
					smalestTime = t;
					lengthProjN = tempLength;
					p0Store = p0;
					p1Store = p1;
					p2Store = p2;
					normalStore = normal;
				}
			}			
		}
		// check agains all edges
		Vector3 lineDirection;
		float scalarProj;
		Vector3 projOnLine;
		Vector3 normalToLine;
		// edge 1:
		ny = origin - p0;
		lineDirection = p1 - p0;
		scalarProj = (ny.GetDotProduct(lineDirection)/lineDirection.GetLengthSquared());
		projOnLine = lineDirection * scalarProj;
		if( (scalarProj >= 0.0f) && (scalarProj <= 1) )
		{
			normalToLine = ny - projOnLine;
			tempLength = normalToLine.GetLength();
			if(!firstHit)
			{
				firstHit = true;
				lengthProjN = tempLength;
				p0Store = p0;
				p1Store = p1;
				p2Store = p2;
				normalStore = normalToLine;
				normalStore.normalize();
			}
			else
			{
				if( tempLength < lengthProjN )
				{
					lengthProjN = tempLength;
					p0Store = p0;
					p1Store = p1;
					p2Store = p2;
					normalStore = normalToLine;
					normalStore.normalize();
				}
			}	

		}
		// edge 2:
		ny = origin - p1;
		lineDirection = p2 - p1;
		scalarProj = (ny.GetDotProduct(lineDirection)/lineDirection.GetLengthSquared());
		projOnLine = lineDirection * scalarProj;
		if( (scalarProj >= 0.0f) && (scalarProj <= 1) )
		{
			normalToLine = ny - projOnLine;
			tempLength = normalToLine.GetLength();
			if(!firstHit)
			{
				firstHit = true;
				lengthProjN = tempLength;
				p0Store = p0;
				p1Store = p1;
				p2Store = p2;
				normalStore = normalToLine;
				normalStore.normalize();
			}
			else
			{
				if( tempLength < lengthProjN )
				{
					lengthProjN = tempLength;
					p0Store = p0;
					p1Store = p1;
					p2Store = p2;
					normalStore = normalToLine;
					normalStore.normalize();
				}
			}	

		}
		// edge 3:
		ny = origin - p2;
		lineDirection = p0 - p2;
		scalarProj = (ny.GetDotProduct(lineDirection)/lineDirection.GetLengthSquared());
		projOnLine = lineDirection * scalarProj;
		if( (scalarProj >= 0.0f) && (scalarProj <= 1) )
		{
			normalToLine = ny - projOnLine;
			tempLength = normalToLine.GetLength();
			if(!firstHit)
			{
				firstHit = true;
				lengthProjN = tempLength;
				p0Store = p0;
				p1Store = p1;
				p2Store = p2;
				normalStore = normalToLine;
				normalStore.normalize();
			}
			else
			{
				if( tempLength < lengthProjN )
				{
					lengthProjN = tempLength;
					p0Store = p0;
					p1Store = p1;
					p2Store = p2;
					normalStore = normal;
				}
			}	

		}
	}
	if(firstHit)
	{
		// for checking if the ball are in the air not turned on at the moment, 
		float eps = 0.5f; //0.001
		if( (lengthProjN < (b->GetRadius() + eps)) && (lengthProjN > (b->GetRadius() - eps)) )
		{
			b->SetNormalContact(normalStore);
			b->SetHasContact(true);
		}
		else 
		{
			b->SetNormalContact(normalStore);
			b->SetHasContact(false);
		}

		if( lengthProjN <= b->GetRadius())
		{
			Vector3 velNorm = b->GetVelocity();
			velNorm.normalize();
			
			if(normalStore.GetDotProduct(velNorm) >=0)
				return false;
	
			float diff = abs(lengthProjN- b->GetRadius());
			
			//Vector3 newPo = origin -dirN*diff;
			//Vector3 projVel = normalStore * this->mVelocity.GetDotProduct(normalStore);
			Vector3 newPo = origin + normalStore*diff;
			/*
			if( projVel.GetDotProduct(normalStore) < 0.0f)
			{
				newPo = origin - normalStore*diff;
				return false;
			}
			else
				newPo = origin + normalStore*diff;
			*/
			
			//this->SetPosition(newPo);
			b->SetTempPosition(newPo);
			normalPlane = normalStore;
			//this->mNormalContact = normalPlane;
			//this->mHasContact  = true;
			return true;
		}
		else
		{
			normalPlane = Vector3(0,0,0);
			//this->mNormalContact = normalPlane;
			//this->mHasContact  = false;
			return false;
		}
		
	}
	normalPlane = Vector3(0,0,0);
	b->SetNormalContact(normalPlane);
	//this->mHasContact  = false;
	return false;
}


void PhysicsEngine::CollisionMapResponse(PowerBall* b, Map* map, Vector3 normalPlane, float dt)
{
	
	/* simple response
	Vector3 normal = Vector3(0,1,0);
	Vector3 vn = normal*(this->mVelocity.GetDotProduct(normal));
	Vector3 vp = this->mVelocity - vn;
	Vector3 vAfter = vp - vn;
	this->mVelocity = vAfter;
	*/
	// normal of the "collision plane"
	
	//Vector3 nColl = Vector3(0,-1,0); //this->GetPositionVector3() - p->GetPositionVector3();
	Vector3 nColl = normalPlane;
	// for easy projecting of vector, no div by |n|^2 in proj formula
	nColl.normalize();

	// savning the important stuff for easy acc
	float m1 = b->GetMass();
	float m2 = map->GetMass();
	float mSum = m1+m2;
	Vector3 v1 = b->GetVelocity();
	Vector3 v2 = Vector3(0,0,0);//the platform is not moving b1->mVelocity;

	// projecting the vector v1 on nColl
	float x1 = nColl.GetDotProduct(v1);		// factor in nColl dir
	Vector3 v1x = nColl*x1;					// projetion done
	Vector3 v1y = v1 - v1x;					// perpendicular vector 

	// projecting the vector v2 on nColl
	nColl = nColl*(-1);						// switching direction of "plane normal"
	float x2 = nColl.GetDotProduct(v2);		// factor in nColl dir
	Vector3 v2x = nColl*x2;					// projetion done
	Vector3 v2y = v2 - v2x;					// perpendicular vector 
	
	
	if((v1x.GetLength() >1.5f) && b->SoundEnabled())
		this->mCollisionWithWall->Play();
	
	//float e = this->mRestitution;
	float e = map->GetRestitution();
	float newdt = dt*0.001f;
	//v1y -= v1y*pow(this->mFriction, 2)*newdt;
	if( b->GetPreviousVelocity().GetLength() < b->GetVelocity().GetLength() )
	{
		if( (v1y.GetLength() < 2.0f) && (v1y.GetLength() > 0.0f) )
			v1y -= v1y*b->GetFriction()*newdt;
		if( v1y.GetLength() < 0.1f && (v1y.GetLength() > 0.0f) )
			v1y *= b->GetFriction()*newdt;
	}	
	
	

	b->SetVelocity(Vector3( v1x*(m1-e*m2)/(mSum) + v2x*((1+e)*m2)/(mSum) + v1y));
	//this->mAcceleration = Vector3(0,0,0);
	//this->mSumAddedForce += this->mAcceleration*(-1);
	// use this line if the platform also has a speed.
	//b1->mVelocity = Vector3( v1x*(2*m1)/(mSum) + v2x*(m2-m1)/(mSum) + v2y );
	return;
}

bool PhysicsEngine::RayTriIntersect(Vector3 origin, Vector3 direction, Vector3 p0, Vector3 p1, Vector3 p2, float &u, float &v, float &t)
{
	Vector3 e1, e2, q, s, r;
	u = v = t = 0;
	float a,f, eps = 0.00001f;
	e1 = p1 - p0;
	e2 = p2 - p0;
	//q  = direction.GetCrossProduct(e2);
	q  = e2.GetCrossProduct(direction);
	a = e1.GetDotProduct(q);
	if( a > - eps && a < eps)
		return false;
	f = 1/a;
	s = origin - p0;
	u = f*(s.GetDotProduct(q));
	if( u < 0.0f)
		return false;
	//r = s.GetCrossProduct(e1);
	r = e1.GetCrossProduct(s);
	v = f*(direction.GetDotProduct(r));
	if( v < 0.0f || u+v > 1.0f)
		return false;
	t = f*(e2.GetDotProduct(q));
	return true;
}

void PhysicsEngine::ResetTimers()
{
	this->mGameTimer->ResetTimers();
}

void PhysicsEngine::HandleClientKeyInputs(const int clientIndex, float diff)
{
	//keep reading client inputs until the sum of all DT has exceeded server DT (->not allowed to move any more)
	Command* command = this->mNet->GetBall(clientIndex)->GetNextCommand();
	float duration = 0.0f;
	if(command != NULL)
	{
		duration = command->GetDuration();
		while(duration <=  diff && command != NULL)
		{
			this->mPowerBalls[clientIndex]->SetForwardVector(command->GetForward());
			for(int c = 0; c < command->GetNumInputs(); c++)
			{
				this->ClientKeyPress(command->GetDuration(), clientIndex, command->GetInput(c));
			}
			this->mNet->GetBall(clientIndex)->SetExecTime(this->mNet->GetBall(clientIndex)->GetExecTime() + command->GetDuration());
			this->mNet->GetBall(clientIndex)->PopCommand();


			command = this->mNet->GetBall(clientIndex)->GetNextCommand();
			if(command != NULL)
				duration += command->GetDuration();
								
		}
		if(duration > diff && command != NULL)
		{
			this->mPowerBalls[clientIndex]->SetForwardVector(command->GetForward());
			duration -= command->GetDuration();
									
			for(int c = 0; c < command->GetNumInputs(); c++)
			{
				//ADD A CHECK HERE SO THAT THE SAME KEY CANT APPEAR MORE THAN ONCE IN THE ARRAY (COULD CHEAT THE SYSTEM THIS WAY)
				
				this->ClientKeyPress((diff - duration), clientIndex, command->GetInput(c));
			}

			command->ModifyDuration(-(diff - duration));
								
			this->mNet->GetBall(clientIndex)->SetExecTime(this->mNet->GetBall(clientIndex)->GetExecTime() + (diff - duration));
		}
	}
}

void PhysicsEngine::ClientKeyPress(float diff, const int index, char key)
{

	if(key == 'A')
		mPowerBalls[index]->AddForceLeftOfForwardDirection(diff);
	if(key == 'D')
		mPowerBalls[index]->AddForceRightOfForwardDirection(diff);
	/* wTF */
	if(key == 'W')
		mPowerBalls[index]->AddForceForwardDirection(diff);
	
	
	if(key == 'S')
		mPowerBalls[index]->AddForceOppositeForwardDirection(diff);
	
	if(key == 'Q')
		mPowerBalls[index]->RotateForwardLeft(diff);
	if(key == 'E')
		mPowerBalls[index]->RotateForwardRight(diff);
	
	if(this->mServerInfo.GetGameMode() == GAMEMODE::WARLOCK)
	{
		if(key == '1')
		{
			if(mPowerBalls[index]->GetSpells()[0]->ReadyToBeCast())
				MsgHandler::GetInstance().SendCastSpell(index, 1);
			mPowerBalls[index]->UseSpell(1);
		}
		if(key == '2')
		{
			if(mPowerBalls[index]->GetSpells()[1]->ReadyToBeCast())
				MsgHandler::GetInstance().SendCastSpell(index, 2);
			mPowerBalls[index]->UseSpell(2);
		}
		if(key == '3')
		{
			if(mPowerBalls[index]->GetSpells()[2]->ReadyToBeCast())
				MsgHandler::GetInstance().SendCastSpell(index, 3);
			mPowerBalls[index]->UseSpell(3);
		}
		if(key == '4')
		{
			if(mPowerBalls[index]->GetSpells()[3]->ReadyToBeCast())
				MsgHandler::GetInstance().SendCastSpell(index, 4);
			mPowerBalls[index]->UseSpell(4);
		}
		if(key == VK_SPACE)
		{
			if(mPowerBalls[index]->GetSpells()[4]->ReadyToBeCast())
				MsgHandler::GetInstance().SendCastSpell(index, 5);
			mPowerBalls[index]->UseSpell(5);
		}
	}

	
}

void PhysicsEngine::InputKeysPressedSelf(float diff, int index)
{
	if(mGe->GetEngineParameters().CamType == TRD)
	{
		if(mGe->GetKeyListener()->IsPressed('A'))
			mPowerBalls[index]->AddForceLeftOfForwardDirection(diff);	
		if(mGe->GetKeyListener()->IsPressed('W'))
			mPowerBalls[index]->AddForceForwardDirection(diff);	
		if(mGe->GetKeyListener()->IsPressed('S'))
			mPowerBalls[index]->AddForceOppositeForwardDirection(diff);
		if(mGe->GetKeyListener()->IsPressed('Q'))
			mPowerBalls[index]->RotateForwardLeft(diff);
		if(mGe->GetKeyListener()->IsPressed('E'))
			mPowerBalls[index]->RotateForwardRight(diff);
		if(mGe->GetKeyListener()->IsPressed('D'))
			mPowerBalls[index]->AddForceRightOfForwardDirection(diff);	
		if(this->mServerInfo.GetGameMode() == GAMEMODE::WARLOCK)
		{
			if(mGe->GetKeyListener()->IsPressed('1'))
			{
				if(this->mNet->IsServer() && mPowerBalls[index]->GetSpells()[0]->ReadyToBeCast())
					MsgHandler::GetInstance().SendCastSpell(index, 1);
				mPowerBalls[index]->UseSpell(1);
			}
			if(mGe->GetKeyListener()->IsPressed('2'))
			{
				if(this->mNet->IsServer() && mPowerBalls[index]->GetSpells()[1]->ReadyToBeCast())
					MsgHandler::GetInstance().SendCastSpell(index, 2);
				mPowerBalls[index]->UseSpell(2);
			}
			if(mGe->GetKeyListener()->IsPressed('3'))
			{
				if(this->mNet->IsServer() && mPowerBalls[index]->GetSpells()[2]->ReadyToBeCast())
					MsgHandler::GetInstance().SendCastSpell(index, 3);
				mPowerBalls[index]->UseSpell(3);
			}
			if(mGe->GetKeyListener()->IsPressed('4'))
			{
				if(this->mNet->IsServer() && mPowerBalls[index]->GetSpells()[3]->ReadyToBeCast())
					MsgHandler::GetInstance().SendCastSpell(index, 4);
				mPowerBalls[index]->UseSpell(4);
			}
			if(mGe->GetKeyListener()->IsPressed(VK_SPACE))
			{
				if(this->mNet->IsServer() && mPowerBalls[index]->GetSpells()[4]->ReadyToBeCast())
					MsgHandler::GetInstance().SendCastSpell(index, 5);
				mPowerBalls[index]->UseSpell(5);
			}
		}
		if(mGe->GetKeyListener()->IsPressed('Z') && !this->mZoomOutPressed)
		{
			mPowerBalls[index]->ZoomOut();
			this->mZoomOutPressed = true;
		}
		else if(!mGe->GetKeyListener()->IsPressed('Z'))
			this->mZoomOutPressed = false;
		if(mGe->GetKeyListener()->IsPressed('C') && !this->mZoomInPressed)
		{
			mPowerBalls[index]->ZoomIn();
			this->mZoomInPressed = true;
		}
		else if(!mGe->GetKeyListener()->IsPressed('C'))
			this->mZoomInPressed = false;
	}
	else
	{

	}
}

void PhysicsEngine::SendKeyInputs(const int clientIndex, float diff)
{
	char keyDowns[5] = {0};
	char keysToCheck[12] = {'A', 'D', 'W', 'S', 'Q', 'E', VK_SPACE, '1', '2', '3', '4', VK_SPACE};
	int numKeys = 0;
	for(int i = 0; i < 12; i++)
	{
		if(mGe->GetKeyListener()->IsPressed(keysToCheck[i]) && numKeys < 5)
			keyDowns[numKeys++] = keysToCheck[i];
	}
				
	if(numKeys == 0)
		keyDowns[numKeys++] = '?'; //"idle"-key
	
	Vector3 temp = this->mPowerBalls[clientIndex]->GetForwardVector();
	this->mNet->GetBall(clientIndex)->AddKeyInput(keyDowns, numKeys, diff, D3DXVECTOR3(temp.x, temp.y, temp.z));
	
}