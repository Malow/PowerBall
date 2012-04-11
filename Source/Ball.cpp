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
	temp = D3DXVECTOR3(newPosition.x, newPosition.y, newPosition.z);
	this->GetMesh()->SetPosition(temp);
	float angleRad = (direction.GetLength()/(2*PI*this->mRadius))*(180/PI);
	temp = D3DXVECTOR3(direction.x, direction.y, direction.z);
	//this->mMesh->Rotate(temp*angleRad);

	Vector3 around = direction.GetCrossProduct(Vector3(0,1,0));
	around.normalize();
	D3DXVECTOR3 aroundD3D = D3DXVECTOR3(around.x, around.y, around.z);
	angleRad = (direction.GetLength()/(2*PI*this->mRadius))*2*PI;
	this->mMesh->RotateAxis(aroundD3D, angleRad);
	
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

	if(pos.y > 0)
		alive = true;

	return alive;
}

