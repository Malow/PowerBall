#include "Ball.h"

Ball::Ball(const string meshFilePath)
{
	//this->mMesh		= Engine.GetInstance()->CreateObject(meshFilePath);
	this->mRadius		= 1.0f;
	this->mVelocity		= 0.0f;
	this->mMaxVelocity	= 1.0f;
	this->mAcceleration	= 0.0f;
	//this->mForward	= vector3(0.0f, 0.0f, 1.0f);
}
Ball::Ball(const string meshFilePath, float radius, float maxVelocity, float acceleration)
{
	//this->mMesh		= Engine.GetInstance()->CreateObject(meshFilePath);
	this->mRadius		= radius;
	this->mVelocity		= 0.0f;
	this->mMaxVelocity	= maxVelocity;
	this->mAcceleration	= acceleration;
	//this->mForward	= vector3(0.0f, 0.0f, 1.0f);
}
Ball::~Ball()
{
	//Engine.GetInstance()->DeleteObject(this->mMesh);
}

void Ball::Update(const float dt)
{
	//split up
	//this->mMesh->SetCenter/Pos(this->mMesh->GetCenter() + this->mVelocity*dt * this->mForward);
	//if ( platform->gety(ball.pos) == -1.0f)
		//ball is affected by gravity
	//KeyHandler->getinstance->if(wasd)->affect the forwardvector and velocity
}
bool Ball::IsAlive() const
{
	bool alive = false;
	//if(this->GetPos().y > 0)
		alive = true;
	return alive;
}

