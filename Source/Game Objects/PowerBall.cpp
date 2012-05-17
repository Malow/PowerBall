#include "PowerBall.h"
#include "Map.h"


/*
string convertInt(int number)
{
		if (number == 0)
			return "0";
		string temp="";
		string returnvalue="";
		while (number>0)
		{
			temp+=number%10+48;
			number/=10;
		}
    for (int i=0;i<temp.length();i++)
        returnvalue+=temp[temp.length()-i-1];
    return returnvalue;
}
*/
/*
std::string OurItoa(int n)
{
	std::stringstream stream;
	stream <<n;
	return stream.str();
}
*/
#define Y_LEVEL_BOUNDARY 10
PowerBall::PowerBall(const string meshFilePath, D3DXVECTOR3 position)
{
	this->mHealth		 = 100;
	this->mMesh			 = GetGraphicsEngine()->CreateStaticMesh(meshFilePath, position); 
	this->mRadius		 = 1.0f;
	this->mVelocity		 = Vector3(0,0,0);
	this->mPreviousVelocity = Vector3(0,0,0);
	this->mTempPosition = position;
	this->mSteering      = true;
	this->mNrOfSpells	 = 0;
	this->mMaxNrOfSpells = 5;
	this->mSpells		  = new Spell*[this->mMaxNrOfSpells];
	this->mRoundsWon	 = 0;
	this->mWinTimerActivated = false;
	this->mLastRotation.LoadIdentity();
	this->mWinTimer = 0.0f;
	this->mForward		 = Vector3(0,0,1);
	this->mDistanceCam	= 5;
	this->mMaxVelocity	 = 10.0f;
	this->mAcceleration	 = Vector3(0,-9.81f,0);
	this->mDamping		 = 0.70f;//0.9995f; //0.995
	this->mMass			 = 9;
	this->mSumAddedForce = Vector3(0,0,0);
	this->mRestitution   = 1.0f; //0.95f
	this->mForcePress	 = 180.0f;
	this->mInTheAir		 = true;	// we are dropped from air
	this->mFriction		 = 0.9f;	// this is in the opposite direction to velocity, if this is 0, then no friction (only damping will decrese the speed)
	this->mKnockoutMode = false;
	this->mStartPosition = position;
	this->mLivesLeft	 = 2;
	this->mRespawnTime	 = 5.0f;
	this->mRespawnTimeLeft	= this->mRespawnTime;
	this->mTimeInHotZone = 0.0f;
	this->mTeamColor = TEAM::NOTEAM;
	this->mSound		  = false;
	this->mCollisionWithWall = GetGraphicsEngine()->GetSoundEngine()->LoadSoundEffect("Media/Sounds/SoundEffects/ball_vs_wall.mp3");
	this->mCollisionWithBall = GetGraphicsEngine()->GetSoundEngine()->LoadSoundEffect("Media/Sounds/SoundEffects/ball_vs_ball.mp3");
	/*
	this->mMaxNrOfItems = 6;
	this->mNrOfItems = 0;
	*/
	this->mFlag = NULL;
	

	/*
	*	Now it is working with deltaTime, the value above are in seconds and movement
	*	are the ones that are used in directx api
	*	If your speed is vx = 1, that means that you are traveling one position in
	*	worldspace in the direction as e1 (that is x-axis) per second.
	*	if you are dropping the balls from a height of 30 in our game (knockout map)
	*	the formula for position is:
	*	s = s0 + v0*t + (acceleration/2)*t*t = s0 + v0*t + 4.9*t*t
	*
	*   s = the point we want
	*	s0 = the startpoint
	*	v0 = the startvelocity
	*	t = time
	*
	*	This gives us: 
	*	14.7 = 30 + 0*t + 4.9*t*t -> t = sqrt(15,3/4,9) = 1,78 sek

	*	Now put a breakpoint inside the 'if statement' when collition to platform is
	*	true. Now check your counter above, it will show something like 1900 milisek
	*	and that is approx the same as 1.78 sek because we use drag in form of mDamping
	*	above.
	*
	*	Damping can we see as a combination of Friction in form of wind or against platform
	*	Restitution is a messurement of how the collision make a object loose speed.
	*
	*/
}
PowerBall::~PowerBall()
{

	GetGraphicsEngine()->DeleteStaticMesh(this->mMesh);
	this->mFlag = NULL;
	if(GetGraphicsEngine()->GetEngineParameters().CamType ==  TRD)
		if(this == ((TRDCamera*)GetGraphicsEngine()->GetCamera())->getPowerBallToFollow())
			((TRDCamera*)GetGraphicsEngine()->GetCamera())->removeFollowBall();
	for (int i = 0; i < this->mNrOfSpells; i++) 
	{
		SAFE_DELETE(this->mSpells[i]);
    }
	SAFE_DELETE_ARRAY(this->mSpells);
}
Vector3 PowerBall::GetPositionXZ() const
{
	D3DXVECTOR3 temp = this->mMesh->GetPosition();
	return Vector3(temp.x, 0, temp.z);
}

void PowerBall::SetToStartPosition()
{
	this->SetPosition(this->mStartPosition);
	this->SetTempPosition(this->mStartPosition);
	this->SetVelocity(Vector3(0,0,0));
}
void PowerBall::Update(const float dt, bool clientBall)
{

	/*
	*	Try to avoid changing the algoritm below its okey for now.
	*	If you are unhappy with settings for the ball try to 
	*	change in the constructor insted. There you
	*	can change most things like how much "engergy" you loose at impact 
	*	this->mRestitution, for ball is only ball to ball. In the platform
	*	class you can change the Restitution value for impact ball against
	*	platform
	*
	*	this->mDamping is used as velocity loose such that airDrag and so on. 
	*
	*	how much tangental velocity that is loost due to friction on impact
	*	only in use when ball is against platform
	*	this->mFriction
	*	
	*	This can be changed if we feel its not working. Give me some pointers
	*	so we can make this better :)
	*/
	
	float newdt = dt * 0.001f;
	for(int i = 0;i<this->mNrOfSpells;i++)
		this->mSpells[i]->UpdateSpecial(newdt);
	if(this->mWinTimerActivated)
		this->mWinTimer += newdt;
	D3DXVECTOR3 temp = this->GetMesh()->GetPosition();
	Vector3 oldPosition = Vector3(temp);
	Vector3 newPosition = oldPosition + mVelocity * newdt;

	if(newPosition.y < Y_LEVEL_BOUNDARY && !this->mKnockoutMode)
	{
		this->mVelocity = Vector3(0,-2,0);
		if(!clientBall)
		{
			((TRDCamera*)GetGraphicsEngine()->GetCamera())->setPowerBallToFollow(NULL);
			((TRDCamera*)GetGraphicsEngine()->GetCamera())->LookAt(this->GetPosition());
		}
	}
	/*
	if(newPosition.y < 14.7f && platform->IsOnPlatform(temp.x, temp.z))
		newPosition.y = 14.7f;	//oldPosition.y;
	*/
	Vector3 direction = newPosition - oldPosition;
	direction.y = 0;
	temp = D3DXVECTOR3(newPosition.x, newPosition.y, newPosition.z);
	this->mTempPosition = newPosition;
	//this->GetMesh()->SetPosition(temp);
	if(this->mFlag != NULL)
		this->mFlag->SetPosition(this->mMesh->GetPosition());
	temp = D3DXVECTOR3(direction.x, direction.y, direction.z);

	//this->Rotate(direction);
	
	Vector3 resAcc = this->mAcceleration;
	// F = ma <-> a = F/m 
	resAcc += (this->mSumAddedForce / this->mMass );
	//resAcc += this->mSumAddedForce;
	Vector3 oldVelocity = this->mVelocity;
	this->mPreviousVelocity = oldVelocity;
	Vector3 newVelocity = this->mVelocity + resAcc * newdt;
	//Vector3 controlledMovedVelocity = newVelocity;
	//controlledMovedVelocity.y = 0.0f;

	if(newVelocity.GetLength() > this->mMaxVelocity)
	{
		float length = newVelocity.GetLength();
		newVelocity.normalize();
		this->mVelocity = newVelocity * this->mMaxVelocity;
		this->mVelocity *= pow(this->mDamping, newdt);
		/*
		resAcc.x = 0;
		resAcc.z = 0;
		this->mVelocity += resAcc * newdt;
		//this->mVelocity.y *= pow(this->mDamping, newdt);
		this->mVelocity *= pow(this->mDamping, newdt);
		*/
	}
	else
	{
		this->mVelocity += resAcc * newdt;
		this->mVelocity *= pow(this->mDamping, newdt);
	}
	//controlledMovedVelocity = this->mVelocity;
	//controlledMovedVelocity.y = 0;

	/*
	if (this->mVelocity.GetLength() > this->mMaxVelocity )
		this->mVelocity = oldVelocity;
	*/	
	// so that the ball will lose some force due to friction for example
	//this->mVelocity *= pow(this->mDamping, newdt);

	// remove the forces that did push against this ball
	this->mSumAddedForce = Vector3(0,0,0);
	//*this->mPos = this->mMesh->GetPosition();
	//*this->mFor = this->mForward.GetD3DVec();
	
	
	if((this->mMesh->GetPosition().y < Y_LEVEL_BOUNDARY + 1) && !this->mKnockoutMode) 
	{
		
		if(this->mFlag != NULL)
		{
			this->mFlag->Reset();
			this->ResetFlag();
		}
		if(this->mLivesLeft == 1) //Dont respawn if you lost your last life
			this->mLivesLeft = 0;

		this->mRespawnTimeLeft -= newdt;
		if(this->mRespawnTimeLeft <= 0.0f)
		{
			if(!clientBall)
			{
				//setting the forward vector pointing towards the center of the map when respawning.
				//D3DXVECTOR3 dir(-this->mStartPosition.x, 0, -this->mStartPosition.z);
				//::D3DXVec3Normalize(&dir, &dir);
				

				this->mForward = this->mStartForwardVector;
				((TRDCamera*)GetGraphicsEngine()->GetCamera())->setPowerBallToFollow(this);
			}
			this->mLivesLeft--;
			this->mMesh->SetPosition(this->mStartPosition);
			this->SetTempPosition(this->mStartPosition);
			this->mVelocity = Vector3(0,0,0);
			this->mRespawnTimeLeft = this->mRespawnTime;
		}
	}
}

void PowerBall::UpdateBallParentMode(Map* map)
{
		
		Vector3 scalingMesh = Vector3(1,1,1);
		Vector3 pos = map->GetMesh()->GetPosition();
		D3DXMATRIX quat;
		D3DXMatrixRotationQuaternion(&quat, &map->GetMesh()->GetRotation()); 
		
		Matrix4 rotate(quat);
		rotate.TransposeThis();

		Matrix4 scaling;
		scaling.SetScale(scalingMesh);

		Matrix4 translate;
		translate.SetTranslate(pos);
	
				
		Matrix4 world = translate*rotate*scaling;
		Vector3 posBall = this->mMesh->GetPosition();
		
		/* if we move the platform we can also use world, but this way
		   is simpler and we use the Transpose to get the rotation matrix inverse. */

		Matrix4 newRotation = rotate * this->mLastRotation.GetTranspose(); 
		Vector3 newPosBall = newRotation * (posBall - pos);
		newPosBall += pos;
		this->SetPosition(newPosBall.GetD3DVec());
		this->SetTempPosition(newPosBall.GetD3DVec());
		this->mLastRotation = rotate;

}

void PowerBall::UpdatePost()
{
	Vector3 oldPosition = Vector3(this->mMesh->GetPosition());
	Vector3 newPosition = this->mTempPosition;
	Vector3 direction = newPosition - oldPosition;
	direction.y = 0;
	D3DXVECTOR3 temp = D3DXVECTOR3(newPosition.x, newPosition.y, newPosition.z);
	this->GetMesh()->SetPosition(temp);
	temp = D3DXVECTOR3(direction.x, direction.y, direction.z);
	this->Rotate(direction);	

}

bool PowerBall::IsAlive() const
{
	
	if(!this->mKnockoutMode)
	{
		bool alive = false;
		if(this->mLivesLeft > 0 )
			alive = true;
		return alive;
	}
	else
	{
		bool alive = true;
		if(this->mMesh->GetPosition().y < Y_LEVEL_BOUNDARY + 1)
			alive = false;
		return alive;
	}
}

bool PowerBall::AddSpell(Spell* spell)
{
	if(this->mNrOfSpells < this->mMaxNrOfSpells)
	{
		spell->SetPowerBall(this);
		this->mSpells[this->mNrOfSpells++] = spell;
		return true;
	}
	return false;
}

void PowerBall::ZoomOut()
{
	if(this->mDistanceCam < 9)
		this->mDistanceCam += 1.0f;
}

void PowerBall::ZoomIn()
{
	if(this->mDistanceCam > 2)
		this->mDistanceCam -= 1.0f;
}

bool PowerBall::collisionWithSphereSimple(PowerBall* b1)
{
	Vector3 r = this->GetPositionVector3() - b1->GetPositionVector3();
	r = this->GetTempPosition() - b1->GetTempPosition();

	float distanceBalls = r.GetLength();
	float sumRadius = this->GetRadius() + b1->GetRadius();
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
	Vector3 d = this->GetPositionVector3() - b1->GetPositionVector3();
	d = this->GetTempPosition() - b1->GetTempPosition();

	Vector3 rV = b1->mVelocity - this->mVelocity;
	float sumR = this->mRadius + b1->mRadius;
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

			newPos1 = this->GetTempPosition() - this->mVelocity*t1;
			newPos2 = b1->GetTempPosition() - b1->mVelocity*t1;
			this->SetTempPosition(newPos1);
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
			newPos1 = this->GetTempPosition() - this->mVelocity*t2;
			newPos2 = b1->GetTempPosition() - b1->mVelocity*t2;
			this->SetTempPosition(newPos1);
			b1->SetTempPosition(newPos2);
		}
		else 
			return false;
	}
	return true;
}

void PowerBall::collisionSphereResponse(PowerBall* b1)
{
	// normal of the "collision plane"
	//Vector3 nColl = this->GetPositionVector3() - b1->GetPositionVector3();
	Vector3 nColl = this->GetTempPosition() - b1->GetTempPosition();
	// for easy projecting of vector, no div by |n|^2 in proj formula
	Vector3 tempa = nColl;
	nColl.normalize();

	// savning the important stuff for easy acc
	float m1 = this->mMass;
	float m2 = b1->mMass;
	float mSum = m1+m2;
	Vector3 v1 = this->mVelocity;
	Vector3 v2 = b1->mVelocity;

	// projecting the vector v1 on nColl
	float x1 = nColl.GetDotProduct(v1);		// factor in nColl dir
	Vector3 v1x = nColl*x1;					// projetion done
	Vector3 v1y = v1 - v1x;					// perpendicular vector 

	// projecting the vector v2 on nColl
	nColl = nColl*(-1);						// switching direction of "plane normal"
	float x2 = nColl.GetDotProduct(v2);		// factor in nColl dir
	Vector3 v2x = nColl*x2;					// projetion done
	Vector3 v2y = v2 - v2x;					// perpendicular vector 

	float e1 = this->mRestitution;
	float e2 = b1->mRestitution;
	float e = (e1 + e2)/2.0f;

	if(((v1x-v2x).GetLength() > 0.6f) && this->mSound)
		this->mCollisionWithBall->Play();
	

	/*
	this->mVelocity = Vector3( v1x*(m1-m2)/(mSum) + v2x*(2*m2)/(mSum) + v1y );
	b1->mVelocity = Vector3( v1x*(2*m1)/(mSum) + v2x*(m2-m1)/(mSum) + v2y );
	*/
	this->mVelocity = Vector3( v1x*(m1-m2*e)/(mSum) + v2x*((1+e)*m2)/(mSum) + v1y );
	b1->mVelocity = Vector3( v1x*((1+e)*m1)/(mSum) + v2x*(m2-m1*e)/(mSum) + v2y );

	/* informing the spells to the balls that it has been a collision */
	for(int i = 0;i<this->mNrOfSpells;i++)
		this->mSpells[i]->InformCollision();
	for(int i = 0;i<b1->mNrOfSpells;i++)
		b1->mSpells[i]->InformCollision();

}

bool PowerBall::collisionWithPlatformSimple(Map* p, Vector3 &normalPlane)
{
	
	MaloW::Array<MeshStrip*>* temp = p->GetMesh()->GetStrips();
	//int sizeMstrip = temp->size();
	int sizeVertexS0 = temp->get(0)->getNrOfVerts();
	Vertex* verts;
	//Vector3 origin = this->GetPositionVector3();
	Vector3 origin = this->GetTempPosition();
	Vector3 dir = this->mVelocity;
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
	Vector3 pos = Vector3(p->GetMesh()->GetPosition());
	Vector3 posS = this->GetTempPosition();//this->GetPositionVector3();
	Vector3 rayDirection;
	Vector3 scalingMesh = p->GetMesh()->GetScaling();

	D3DXMATRIX quat;
	D3DXMatrixRotationQuaternion(&quat, &p->GetMesh()->GetRotation()); 
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
		if( (lengthProjN < (this->mRadius + eps)) && (lengthProjN > (this->mRadius - eps)) )
		{
			this->mNormalContact = normalStore;
			this->mHasContact = true;
		}
		else 
		{
			this->mNormalContact = normalStore;
			this->mHasContact = false;
		}

		if( lengthProjN <= this->mRadius)
		{
			Vector3 velNorm = this->mVelocity;
			velNorm.normalize();
			
			if(normalStore.GetDotProduct(velNorm) >=0)
				return false;
	
			float diff = abs(lengthProjN-this->mRadius);
			
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
			this->SetTempPosition(newPo);
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
	this->mNormalContact = normalPlane;
	//this->mHasContact  = false;
	return false;
}

void PowerBall::collisionPlatformResponse(Map* p, Vector3 normalPlane, float dt)
{
	/* HÄR ÄR LJUDET!!!!!!!!!!!!!!!!!!!!!!!!!!
	if(this->mVelocity.GetLength() > this->mMaxVelocity/5)
		this->mCollisionWithWall->Play();*/

	


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
	float m1 = this->mMass;
	float m2 = p->GetMass();
	float mSum = m1+m2;
	Vector3 v1 = this->mVelocity;
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
	
	
	if((v1x.GetLength() >1.5f) && this->mSound)
		this->mCollisionWithWall->Play();
	
	//float e = this->mRestitution;
	float e = p->GetRestitution();
	float newdt = dt*0.001f;
	//v1y -= v1y*pow(this->mFriction, 2)*newdt;
	if( this->mPreviousVelocity.GetLength() < this->mVelocity.GetLength() )
	{
		if( (v1y.GetLength() < 2.0f) && (v1y.GetLength() > 0.0f) )
			v1y -= v1y*this->mFriction*newdt;
		if( v1y.GetLength() < 0.1f && (v1y.GetLength() > 0.0f) )
			v1y *= this->mFriction*newdt;
	}	
	


	this->mVelocity = Vector3( v1x*(m1-e*m2)/(mSum) + v2x*((1+e)*m2)/(mSum) + v1y);
	//this->mAcceleration = Vector3(0,0,0);
	//this->mSumAddedForce += this->mAcceleration*(-1);
	// use this line if the platform also has a speed.
	//b1->mVelocity = Vector3( v1x*(2*m1)/(mSum) + v2x*(m2-m1)/(mSum) + v2y );
	return;
}


void PowerBall::Rotate(Vector3 direction)
{
	direction.y = 0;
	float angleRad = (direction.GetLength()/(2*PI*this->mRadius))*(180/PI);
	Vector3 around = Vector3(0,1,0).GetCrossProduct(direction);
	around.normalize();
	D3DXVECTOR3 aroundD3D = D3DXVECTOR3(around.x, around.y, around.z);
	angleRad = (direction.GetLength()/(2*PI*this->mRadius))*2*PI;

	this->mMesh->RotateAxis(aroundD3D, angleRad);
}


bool PowerBall::RayTriIntersect(Vector3 origin, Vector3 direction, Vector3 p0, Vector3 p1, Vector3 p2, float &u, float &v, float &t)
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

void PowerBall::SetTeamColor(int team)
{
	 this->mTeamColor = team; 
	 if(team == TEAM::BLUETEAM)
		 this->mMesh->SetSpecialColor(COLOR::BLUE_COLOR);
	 else if(team == TEAM::REDTEAM)
		 this->mMesh->SetSpecialColor(COLOR::RED_COLOR);
	 else
	 {
		 
	 }
}