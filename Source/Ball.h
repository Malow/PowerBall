#pragma once

#include <string>
#include "Vector.h"
#include "GraphicsEngine.h"
#include "Platform.h"
using namespace MaloW;
using namespace std;


class Ball
{
private:
	
	Mesh*		mMesh;
	float		mRadius;
	float		mMass;
	float		mDamping;
	Vector3		mSumAddedForce;
	Vector3		mVelocity;
	float		mMaxVelocity;
	Vector3		mAcceleration;
	float		mRestitution;
	float		mForcePress;
	float		mInTheAir;
	float	    mFriction;
	ofstream file;
	Mesh*		mInventory;
	/*
	float		mMaxNrOfItems;
	float		mNrOfItems;
	*/
	int			mLivesLeft;
	D3DXVECTOR3	mStartPos;
	float		mRespawnTime;
	float		mRespawnTimeLeft;
	//vector3	mForward;

public:
	//constructors and destructors

	/*! Initializes the Ball and loads assigned mesh.*/
	Ball(const string meshFilePath, D3DXVECTOR3 position);
	virtual		~Ball();
	

	//Get-Functions
	
	/*! Returns the balls radius. */
	float GetRadius() const { return this->mRadius; } 

	/*! Returns the position of the ball in world space (retrieved from mMesh). */
	D3DXVECTOR3 GetPosition() const { return this->mMesh->GetPosition(); }

	/*! Returns the position of the ball in world space (retrieved from mMesh). */
	Vector3 GetPositionVector3() const;

	/*! Returns the position of the platform in XZ plane */
	Vector3 GetPositionXZ() const;
	
	/*! Returns the balls current velocity. */
	Vector3 GetVelocity() const { return this->mVelocity; } 

	/*! Returns the upper limit of the balls velocity. */
	float GetMaxVelocity() const { return this->mMaxVelocity; }
	
	/*! Returns acceleration value. */
	Vector3 GetAcceleration() const { return this->mAcceleration; }
	
	/*! Returns a pointer to the mesh of this ball. */
	Mesh* GetMesh() { return this->mMesh; }
	
	/*! Returns the current time until the ball respawns. */
	float GetRespawnTimeLeft() const { return this->mRespawnTimeLeft; }

	/*! Returns the intial time for the ball to respawn. */
	float GetIntialRespawnTime() const { return this->mRespawnTime; }
	
	/*! Returns the number of lives left for the ball. */
	int GetNumLivesLeft() const { return this->mLivesLeft; }

	/*! Returns the balls forward vector (the direction the ball is moving). */
	//vector3	GetForwardVector() const { return this->mForward; }

	/*! Adds a item to the ball*/
	void AddItem(Mesh* item){ this->mInventory = item; }

	/*! Reset inventory*/
	void ResetInventory(){ this->mInventory = NULL; }

	//Set-Functions
	
	/*! Returns the balls radius. */
	void SetRadius(float radius) { this->mRadius = radius; } 

	/*! Sets the position of the ball in world space. */
	void SetPosition(D3DXVECTOR3 position) { this->mMesh->SetPosition(position); }

	/*! Sets the position of the ball in world space. */
	void SetPosition(const float x, const float y, const float z) { this->mMesh->SetPosition(D3DXVECTOR3(x,y,z)); }

	/*! Sets the position of the ball in world space. */
	void SetPosition(const Vector3 position) { this->mMesh->SetPosition(D3DXVECTOR3(position.x,position.y,position.z)); }

	/*! Sets the balls current velocity. */
	void SetVelocity(Vector3 vel) { this->mVelocity = vel; } 

	/*! Sets the upper limit of the balls velocity. */
	void SetMaxVelocity(float maxVel) { this->mMaxVelocity = maxVel; }
	
	/*! Sets the acceleration value. */
	void SetAcceleration(Vector3 acc) { this->mAcceleration = acc; }

	/*! Sets if the ball is in the air. */
	void SetInTheAir(bool isInAir) { this->mInTheAir = isInAir; }

	/*! Sets the respawn time for the ball. */
	void SetRespawnTime(const float respawnTime) { this->mRespawnTime = respawnTime; }
	
	/*! Sets the number of lives for the ball. */
	void SetNumLives(const int numLives) { this->mLivesLeft = numLives; }

	/*! Sets the balls forward vector (the direction the ball is moving). */
	//void SetForwardVector(vector3 forward) { this->mForward = forward; }


	/*! Updates the ball. */
	void Update(const float dt, Platform* platform);
	
	/*! Returns false if the number of ball lives reaches 0. */
	bool IsAlive() const;

	/*! Adds a force to this ball. */
	void AddForce(const Vector3 &force) { this->mSumAddedForce += force * 10.0f * this->mForcePress; }

	/*! Simple Collision Detection between Sphere and Sphere. */
	bool collisionWithSphereSimple(Ball* b1);

	/*! Sphere To Sphere Collision Response. */
	void collisionSphereResponse(Ball* b1, float dt);

	/*! Simple Collision Detection between Sphere and Platform. */
	bool collisionWithPlatformSimple(Platform* p, Vector3 &normal);

	/*! Sphere To Platform Collision Response. */
	void collisionPlatformResponse(Platform* p, Vector3 normalPlane, float dt);

	/*! Rotates the ball-mesh. */
	void Rotate(Vector3 direction);

	/*! Returns true if ray intersect triangle and u, v and time is saved. */
	bool RayTriIntersect(Vector3 origin, Vector3 direction, Vector3 p0, Vector3 p1, Vector3 p2, float &u, float &v, float &t);
};
