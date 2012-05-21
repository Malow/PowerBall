#pragma once
/**
* Author: Jerry Rahmqvist  
* Co Author: Didrik Axelsson
* 
* Create Date: 09/05/2012
* 
* The PowerBall class that controls your ball.
*
**/

#include "..\Game Objects\GameObject.h"
#include "..\Game Objects\FlagCTF.h"
#include "..\Spells\Spell.h"
class Map;
#define Y_LEVEL_BOUNDARY 10
class PowerBall : public GameObject
{
	private:
			float		mHealth;
			float		mMass;
			float		mRestitution;
			float		mRadius;
			float		mDamping;
			Vector3		mNormalContact;
			bool		mHasContact;
			Vector3		mSumAddedForce;
			Vector3		mTempPosition;
			Vector3		mVelocity;
			Vector3		mPreviousVelocity;
			bool		mSteering;
			Spell**		mSpells;
			int			mNrOfSpells;
			int			mMaxNrOfSpells;
			Matrix4		mLastRotation;
			int			mRoundsWon;
			Vector3		mForward;
			Vector3		mStartForwardVector;
			float		mDistanceCam;
			float		mMaxVelocity;
			Vector3		mAcceleration;
			float		mForcePress;
			float		mInTheAir;
			float	    mFriction;
			bool		mKnockoutMode;
			bool		mWinTimerActivated;
			float		mWinTimer;
		
			FlagCTF*	mFlag;
			int			mLivesLeft;
			float		mRespawnTime;
			float		mRespawnTimeLeft;
			float		mTimeInHotZone;
			int			mTeamColor;
			/* SoundEffects */
			bool			mSound;
			SoundEffect*	mCollisionWithWall;
			SoundEffect*	mCollisionWithBall;

			bool	mWarlockMode;

			
	public:
			PowerBall(const string meshFilePath, D3DXVECTOR3 position);
			virtual ~PowerBall();
	
			
			/* Get - functions */
	
			/*! Returns the balls radius. */
			float GetRadius() const { return this->mRadius; } 

			/*! Returns the position of the platform in XZ plane */
			Vector3 GetPositionXZ() const;
	
			/*! Returns the temp position of the ball that is before its going through physics .*/
			Vector3 GetTempPosition() const { return this->mTempPosition; }

			/*! Returns the balls current velocity. */
			Vector3 GetVelocity() const { return this->mVelocity; } 

			/*! Returns the upper limit of the balls velocity. */
			float GetMaxVelocity() const { return this->mMaxVelocity; }
	
			/*! Returns acceleration value. */
			Vector3 GetAcceleration() const { return this->mAcceleration; }
	
			/*! Returns a pointer to the mesh of this ball. */
			StaticMesh* GetMesh() { return this->mMesh; }
	
			/*! Returns the current time until the ball respawns. */
			float GetRespawnTimeLeft() const { return this->mRespawnTimeLeft; }

			/*! Returns the intial time for the ball to respawn. */
			float GetIntialRespawnTime() const { return this->mRespawnTime; }
	
			/*! Returns the number of lives left for the ball. */
			int GetNumLivesLeft() const { return this->mLivesLeft; }

			/*! Returns the number of lives left for the ball as string. */
			string GetNumLivesLeftStr() const { return MaloW::convertNrToString(this->mLivesLeft); }

			/*! Returns the number of rounds this ball has won. */

			/*! Returns the time the ball has been alone in hot zone. */
			float GetTimeInHotZone() const { return this->mTimeInHotZone; }

			/*! Returns the balls forward vector (the direction the ball is moving). */
			Vector3	GetForwardVector() const { return this->mForward; }

			/*! Returns the balls distance from cam. */
			float GetDistanceToCam() const { return this->mDistanceCam; }

			/*! Returns true if steering is on. */
			bool GetSteering() const { return this->mSteering; }

			/*! Returns the nr of spell currently on the ball. */
			int GetNrOfSpells() const { return this->mNrOfSpells; }

			/*! Returns the max nr of spells for the ball. */
			int GetMaxNrOfSpells() const { return this->mMaxNrOfSpells; }

			/*! Returns forcepress value. */
			float GetForcePressed() const { return this->mForcePress; }

			/*! Returns the balls mass. */
			float GetMass() const { return this->mMass; }

			/*! Returns the Restitution value a.k.a. 'e' value. */
			float GetRestitution() const { return this->mRestitution; }

			/*! Returns the spells for this ball. */
			Spell** GetSpells() const { return this->mSpells; }

			/*! Returns the nr of rounds this ball has won. */
			int GetRoundsWon() const { return this->mRoundsWon; }

			/*! Returns the nr of rounds this ball has won. */
			string GetRoundsWonStr() const { return MaloW::convertNrToString(this->mRoundsWon); }

			/*! Returns the time that has pased after your win timer was activated. */
			float GetWinTimer() const { return this->mWinTimer; }

			/*! Returns the normal of the contact surface. */
			Vector3 GetNormalContact() const { return this->mNormalContact; }

			/*! Returns a bool if the ball is in contact with any surface. */
			bool GetHasContact() const { return this->mHasContact; }

			/*! Returns the team color of the ball. */
			int GetTeamColor() const { return this->mTeamColor; }

			/*! Returns the start forward vector. */
			Vector3 GetStartForwardVector() const { return this->mStartForwardVector; }

			/*! Returns the health of the ball. */
			float GetHealth() const { return this->mHealth; }
			
			/*! Adds a item to the ball*/
			void AddFlag(FlagCTF* item){ this->mFlag = item; }

			/*! Reset inventory*/
			void ResetFlag(){ this->mFlag = NULL; }

			/*! Reset time in hot zone. */
			void ResetTime() { this->mTimeInHotZone = 0; }

			/*! Restet time for win timer. */
			void RestetWinTimer() { this->mWinTimer = 0.0f; this->mWinTimerActivated = false;};
	
			/*! Restet the parent rotation. */
			void ResetParent() { Matrix4 temp; temp.LoadIdentity(); this->mLastRotation = temp; }
			//Set-Functions
			
			/*! Sets the health of the ball. */
			void SetHealth(float health) { if( health <= 100 && health >= 0)
												this->mHealth = health; 
										 }
			void SetWarlockMode(bool value) { this->mWarlockMode = value; }
			/*! Sets the balls radius. */
			void SetRadius(float radius) { this->mRadius = radius; } 

			/*! Set position to StartPosition- */
			virtual void SetToStartPosition();

			/*! Sets the temp position of the ball that is before its going through physics .*/
			void SetTempPosition(Vector3 temp) { this->mTempPosition = temp; }

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

			/*! Sets the time the ball has been in hot zone. */
			void SetTimeInHotZone(float timeInHotZone) {this->mTimeInHotZone = timeInHotZone; }

			/*! Sets the balls distance from cam. */
			void SetDistanceToCam(float dist) { this->mDistanceCam = dist; }

			/*! Sets the steering behav. for the ball, true if steering is on. */
			void SetSteering(bool steering) { this->mSteering = steering; }

			/*! Sets max nr of spells for ball. */
			void SetMaxNrOfSpells(int maxSpells) { this->mMaxNrOfSpells = maxSpells; }

			/*! Sets the forcePressed value. */
			void SetForcePressed(float force) { this->mForcePress = force; }

			/*! Sets the balls mass. */
			void SetMass(float mass) { this->mMass = mass; }

			/*! Sets the Restitution value a.k.a. 'e' value. */
			void SetRestitution(float restitution) { this->mRestitution = restitution; }

			/*! Sets the knockout flag. True if knockoutmode */
			void SetKnockoutMode() { this->mKnockoutMode = true; }

			

			/*! Sets the balls forward vector (the direction the ball is moving). */
			//void SetForwardVector(vector3 forward) { this->mForward = forward; }

			/*! Sets the ball forward vector. */
			void SetForwardVector(D3DXVECTOR3 forward) { this->mForward = Vector3(forward); }
			void SetForwardVector(Vector3 forward) { this->mForward = forward; }

			/*! Sets sound of if false. */
			void SetSound(bool sound) { this->mSound = sound; }

			/*! Sets the balls normal to the contacts surface. */
			void SetNormalContact(Vector3 normal) { this->mNormalContact = normal; }

			/*! Sets the has contact, that is saying that the ball has contact with a surface. */
			void SetHasContact(bool contact) { this->mHasContact = contact; }

			/*! Sets the team of the ball. */
			void SetTeamColor(int team);

			/*! Sets the start forward vector. */
			void SetStartForwardVector(Vector3 forward) { this->mStartForwardVector = forward; }

			/*! Updates the ball. */
			void Update(const float dt, bool clientBall = false);

			/*! Updates the balls like if the ball belong to the platform. */
			void UpdateBallParentMode(Map* map);
	
			/*! Post update the ball with the new position given by physics. And apply rotation */
			void UpdatePost();

			/*! Returns false if the number of ball lives reaches 0. */
			bool IsAlive() const;

			/*! Adds a force to this ball. */
			void AddForce(const Vector3 &force) { this->mSumAddedForce += force * this->mForcePress; }

			/*! Adds a force to this ball in forward direction. */
			void AddForceForwardDirection(float dt) { 
														if(this->mHasContact && this->mSteering)
															this->AddForce(this->mForward * dt); 
													}

			/*! Adds a force to this ball in opposite to the forward vector. */
			void AddForceOppositeForwardDirection(float dt) {
																if(this->mHasContact && this->mSteering)
																	this->AddForce(this->mForward * (-dt)); 
															}

			/*! Adds a force to this ball in left direction of forward vector. */
			void AddForceLeftOfForwardDirection(float dt) { 
																if(this->mHasContact && this->mSteering)
																	this->AddForce(this->mForward.GetRoteted(-PI/2.0f) * dt); 
														  }
	
			/*! Adds a force to this ball in right direction of forward vector. */
			void AddForceRightOfForwardDirection(float dt) { 
																if(this->mHasContact && this->mSteering)
																	this->AddForce(this->mForward.GetRoteted(PI/2.0f) * dt); 
														   }

			/*! Adds a spell to this ball. Returns true if successful (not full spell array) */
			bool AddSpell(Spell* spell);

			/*! Adds a won round to this ball. */
			void AddWonRound() { this->mRoundsWon += 1; }

			/*! Activates the win timer. Will be updated in update for ball. */
			void ActivateWinTimer() { this->mWinTimerActivated = true; }

			void UseSpell(int spellNr) { 
											if(spellNr <= this->mNrOfSpells) 
												this->mSpells[spellNr-1]->Use();
									   }

			/*! Zoom out one click from ball. */
			void ZoomOut();

			/*! Zoom in one click from ball. */
			void ZoomIn();


			/*! Adds time to the hot zone time. */
			void AddTimeInHotZone(float dt) { this->mTimeInHotZone += dt; }

			/*! Simple Collision Detection between Sphere and Sphere. */
			bool collisionWithSphereSimple(PowerBall* b1);

			/*! Sphere To Sphere Collision Response. */
			void collisionSphereResponse(PowerBall* b1);

			/*! Simple Collision Detection between Sphere and Platform. */
			bool collisionWithPlatformSimple(Map* p, Vector3 &normal);

			/*! Sphere To Platform Collision Response. */
			void collisionPlatformResponse(Map* p, Vector3 normalPlane, float dt);

			/*! Rotates the ball-mesh. */
			void Rotate(Vector3 direction);

			/*! Rotets the forward vector counterclockwise when you looking down the y+ axis. */
			void RotateForwardLeft(float dt) { this->mForward.RotateY(-(PI/2) * dt * 0.001f); }

			/*! Rotets the forward vector clockwise when you looking down the y+ axis. */
			void RotateForwardRight(float dt) { this->mForward.RotateY((PI/2) * dt * 0.001f); }
	
			/*! Returns true if ray intersect triangle and u, v and time is saved. */
			bool RayTriIntersect(Vector3 origin, Vector3 direction, Vector3 p0, Vector3 p1, Vector3 p2, float &u, float &v, float &t);

			/*! Sets the transparency of the ball. Range is [0,1] */
			//void SetTransparency(float transparency) { this->mMesh->SetTransparency(transparency); }

			/*! Set ball to use invisibility effect or not. */
			void UseInvisibilityEffect(bool use) { this->mMesh->UseInvisibilityEffect(use); }
};

