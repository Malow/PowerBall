#include "Ball.h"

Ball::Ball(const string meshFilePath, D3DXVECTOR3 position)
{
	this->mMesh			 = GetGraphicsEngine()->CreateMesh(meshFilePath, position); 
	this->mRadius		 = 1.0f;
	this->mVelocity		 = Vector3(0,0,0);
	this->mMaxVelocity	 = 3.0f;
	this->mAcceleration	 = Vector3(0,-9.81f,0);
	this->mDamping		 = 0.999f; //0.995
	this->mMass			 = 9;
	this->mSumAddedForce = Vector3(0,0,0);
}
Ball::~Ball()
{
	GetGraphicsEngine()->DeleteMesh(this->mMesh);
}
Vector3 Ball::GetPositionXZ() const
{
	D3DXVECTOR3 temp = this->mMesh->GetPosition();
	return Vector3(temp.x, 0, temp.z);
}
void Ball::Update(const float dt, Platform* platform)
{
	D3DXVECTOR3 temp = this->GetMesh()->GetPosition();
	Vector3 oldPosition = Vector3(temp.x, temp.y, temp.z);
	Vector3 newPosition = oldPosition + mVelocity*dt;
	
	if(newPosition.y < 14.7f && platform->IsOnPlatform(temp.x, temp.z))
		newPosition.y = 14.7f;	//oldPosition.y;
	
	Vector3 direction = newPosition - oldPosition;
	direction.y = 0;
	temp = D3DXVECTOR3(newPosition.x, newPosition.y, newPosition.z);
	this->GetMesh()->SetPosition(temp);
	temp = D3DXVECTOR3(direction.x, direction.y, direction.z);

	this->Rotate(direction);
	
	Vector3 resAcc = this->mAcceleration*dt;
	// F = ma <-> a = F/m 
	resAcc += (this->mSumAddedForce / this->mMass );
	Vector3 oldVelocity = this->mVelocity;
	this->mVelocity += resAcc*dt;
	if (this->mVelocity.GetLength() > this->mMaxVelocity )
		this->mVelocity = oldVelocity;

	// so that the ball will lose some force due to friction for example
	this->mVelocity *= this->mDamping;

	// remove the forces that did push against this ball
	this->mSumAddedForce = Vector3(0,0,0);

	}
bool Ball::IsAlive() const
{
	bool alive = false;
	D3DXVECTOR3 pos = this->mMesh->GetPosition();

	if(pos.y > -25)
		alive = true;

	return alive;
}
Vector3 Ball::GetPositionVector3() const
{
	D3DXVECTOR3 pos = this->mMesh->GetPosition();
	Vector3 position = Vector3(pos.x, pos.y, pos.z);
	return position;
}
bool Ball::collisionWithSphereSimple(Ball* b1)
{
	Vector3 r = this->GetPositionVector3() - b1->GetPositionVector3();
	float distanceBalls = r.GetLength();
	float sumRadius = this->GetRadius() + b1->GetRadius();
	if(distanceBalls > sumRadius)
		return false;
	return true;
}

void Ball::collisionSphereResponse(Ball* b1, float dt)
{
	// normal of the "collision plane"
	Vector3 nColl = this->GetPositionVector3() - b1->GetPositionVector3();
	// for easy projecting of vector, no div by |n|^2 in proj formula
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
	this->mVelocity = Vector3( v1x*(m1-m2)/(mSum) + v2x*(2*m2)/(mSum) + v1y );
	b1->mVelocity = Vector3( v1x*(2*m1)/(mSum) + v2x*(m2-m1)/(mSum) + v2y );
}

bool Ball::collisionWithPlatformSimple(Platform* p)
{
	/* start here
	float distance = this->GetPositionVector3().GetLength;
	if ( this->mRadius
	return true;
	*/
	//p->GetNormalClosestToThisPosition(this->GetPositionVector3());
	Vector3 pos = this->GetPositionVector3();
	if(pos.y >= 14.7f || !p->IsOnPlatform(pos.x, pos.z) )
		return false;

	/*  we have collision but we need to move the ball so they are not inside each other
	*	solve this equation: ((pos1 - t*vel1) = 14.7
	*
	*	this gives ut the following:
	*	t = (pos1.y - 14.7)/v.y
	*
	*/
	/*
	Vector3 pos1 = this->GetPositionVector3();
	Vector3 vec1 = this->GetVelocity();
	float time = (pos1.y - 14.7f)/vec1.y;
	Vector3 newPos = pos1 - vec1*time;
	this->SetPosition(newPos);
	this->SetPosition(pos1.x, 14.7f, pos1.y);
	*/
	return true;
}

void Ball::collisionPlatformResponse(Platform* p, float dt)
{
	/* simple response
	Vector3 normal = Vector3(0,1,0);
	Vector3 vn = normal*(this->mVelocity.GetDotProduct(normal));
	Vector3 vp = this->mVelocity - vn;
	Vector3 vAfter = vp - vn;
	this->mVelocity = vAfter;
	*/
	// normal of the "collision plane"
	
	Vector3 nColl = Vector3(0,-1,0); //this->GetPositionVector3() - p->GetPositionVector3();
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
	
	this->mVelocity = Vector3( v1x*(m1-m2)/(mSum) + v2x*(2*m2)/(mSum) + v1y );
	// use this line if the platform also has a speed.
	//b1->mVelocity = Vector3( v1x*(2*m1)/(mSum) + v2x*(m2-m1)/(mSum) + v2y );
	return;
}

void Ball::Rotate(Vector3 direction)
{
	float angleRad = (direction.GetLength()/(2*PI*this->mRadius))*(180/PI);

	Vector3 around = direction.GetCrossProduct(Vector3(0,1,0));
	around.normalize();
	D3DXVECTOR3 aroundD3D = D3DXVECTOR3(around.x, around.y, around.z);
	angleRad = (direction.GetLength()/(2*PI*this->mRadius))*2*PI;
	this->mMesh->RotateAxis(aroundD3D, angleRad);
}