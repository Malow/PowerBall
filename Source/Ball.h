#pragma once
#include <string>
using namespace std;

class Ball
{
private:
	//Object*	mMesh;
	float		mRadius;
	float		mVelocity;
	float		mMaxVelocity;
	float		mAcceleration; //insert physics.
	//vector3	mForward;

public:
	//constructors and destructors

	/*! Initializes the Ball and loads assigned mesh.*/
				Ball(const string meshFilePath);
				Ball(const string meshFilePath, float radius, float maxVelocity, float acceleration);
	virtual		~Ball();
	

	//Get-Functions
	
	/*! Returns the balls radius. */
	float		GetRadius() const { return this->mRadius; } 

	/*! Returns the position of the ball in world space (retrieved from mMesh). */
	float		GetPos() const; //{ return this->mMesh->GetCenter/Pos(); }
	
	/*! Returns the balls current velocity. */
	float		GetVelocity() const { return this->mVelocity; } 

	/*! Returns the upper limit of the balls velocity. */
	float		GetMaxVelocity() const { return this->mMaxVelocity; }
	
	/*! Returns acceleration value. */
	float		GetAcceleration() const { return this->mAcceleration; }

	/*! Returns the balls forward vector (the direction the ball is moving). */
	//vector3	GetForwardVector() const { return this->mForward; }


	//Set-Functions
	
	/*! Returns the balls radius. */
	void		SetRadius(float radius) { this->mRadius = radius; } 

	/*! Sets the position of the ball in world space. */
	void		SetPos(); //vector3 pos  { this->mMesh->SetCenter/Pos(pos); }

	/*! Sets the position of the ball in world space. */
	void		SetPos(const float x, const float y, const float z);

	/*! Sets the balls current velocity. */
	void		SetVelocity(float vel) { this->mVelocity = vel; } 

	/*! Sets the upper limit of the balls velocity. */
	void		SetMaxVelocity(float maxVel) { this->mMaxVelocity = maxVel; }
	
	/*! Sets the acceleration value. */
	void		SetAcceleration(float acc) { this->mAcceleration = acc; }

	/*! Sets the balls forward vector (the direction the ball is moving). */
	//void		SetForwardVector(vector3 forward) { this->mForward = forward; }


	/*! Updates the ball. */
	void		Update(const float dt);
	
	/*! Returns false if the position of the ball is outside the game field. */
	bool		IsAlive() const;


};