#include "Ball.h"

Ball::Ball(const string meshFilePath)
{
	//this->mMesh		= Engine.GetInstance()->CreateObject(meshFilePath);
	this->mRadius		= 1.0f;
	this->mVelocity		= Vector3(0,0,0);
	this->mMaxVelocity	= 1.0f;
	this->mAcceleration	= Vector3(0,0,0);
	//this->mForward	= vector3(0.0f, 0.0f, 1.0f);
}
Ball::Ball(const string meshFilePath, float radius, float maxVelocity, Vector3 acceleration)
{
	//this->mMesh		= Engine.GetInstance()->CreateObject(meshFilePath);
	this->mRadius		= radius;
	this->mVelocity		= Vector3(0,0,0);
	this->mMaxVelocity	= maxVelocity;
	this->mAcceleration	= acceleration;
	//this->mForward	= vector3(0.0f, 0.0f, 1.0f);
}
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
	//Engine.GetInstance()->DeleteObject(this->mMesh);
}
Vector3 Ball::GetPositionXZ() const
{
	D3DXVECTOR3 temp = this->mMesh->GetPosition();
	return Vector3(temp.x, 0, temp.z);
}
void Ball::Update(const float dt)
{
	D3DXVECTOR3 temp = this->GetMesh()->GetPosition();
	Vector3 oldPosition = Vector3(temp.x, temp.y, temp.z);
	Vector3 newPosition = oldPosition + mVelocity*dt;
	
	if(newPosition.y < 14.7f )
		newPosition.y = 14.7f;	//oldPosition.y;
	
	Vector3 direction = newPosition - oldPosition;
	temp = D3DXVECTOR3(newPosition.x, newPosition.y, newPosition.z);
	this->GetMesh()->SetPosition(temp);
	float angleRad = (direction.GetLength()/(2*PI*this->mRadius))*(180/PI);
	temp = D3DXVECTOR3(direction.x, direction.y, direction.z);
	this->mMesh->Rotate(temp*angleRad);

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
	//if(this->GetPos().y > 0)
		alive = true;
	return alive;
}

