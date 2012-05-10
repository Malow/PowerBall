#pragma once

/**
* Author: Jerry Rahmqvist
* Co Author: Didrik Axelsson
* Create Date: 09/05/2012
* 
* The Map class that is your map when your start a game.
*
**/

#include "..\Game Objects\GameObject.h"

class Map : GameObject
{
	private:
			float		mMass;
			float		mRestitution;
			float		mShrink;
			float		mRadius;
			float		mScaledRadius;
			Vector3		mHotZonePosition;
			float		mHotZoneRadius;
			float		mMaxTimeInHotZone;
			float		mTargetAngleX;
			float		mTargetAngleZ;
			bool		mIsRotating;
			float		mAngleX;
			float		mAngleZ;
			float		mMaxAngleX;
			float		mMaxAngleZ;
			StaticMesh*	mMeshHotZone;
			float		mAngleY;
	public:
			Map(const string meshFilePath, D3DXVECTOR3 position);
			virtual ~Map();
				
			
			/* Get-Functions */

			
			/*! Returns the value for platform shrinking, the platform will shrink by x units per second (Returns 0 if not shrinking). */
			float GetShrinkValue() const { return this->mShrink; }

			/*! Returns a pointer to the mesh of this platform. */
			StaticMesh* GetMesh() { return this->mMesh; }

			/*! Returns a pointer to the mesh of the hotzone. */
			StaticMesh* GetMeshHotZone() { return this->mMeshHotZone; }
	
			/*! Returns the position of the platform in XZ plane */
			Vector3 GetPositionXZ() const;

			/*! Returns the mass of the platform. */
			float GetMass() const { return this->mMass; }

			/*! Returns the scaled radius */
			float GetScaledRadius() const { return this->mScaledRadius; }

			/*! Returns the Restitution- */
			float GetRestitution() const { return this->mRestitution; }

			/*! Returns the Hot Zone Position for the platform. */
			Vector3 GetHotZonePosition() const { return this->mHotZonePosition; }

			/*! Returns the Hot Zone Radius for the platform. */
			float GetHotZoneRadius() const { return this->mHotZoneRadius; }

			/*! Returns the Maximum time the ball needs to be in hot zone. */
			float GetMaxTimeInHotZone() const { return this->mMaxTimeInHotZone; }

			/*! Returns the scale of the platform. */
			Vector3 GetScale() const { return Vector3(this->mMesh->GetScaling()); }

			/*! Returns the target angle for the x-axis. */
			float GetTargetAngleX() const { return this->mTargetAngleX; }

			/*! Returns the target angle for the z-axis. */
			float GetTargetAngleZ() const { return this->mTargetAngleZ; }

			/*! Returns the current angle for the x-axis. */
			float GetAngleX() const { return this->mAngleX; }

			/*! Returns the current angle for the z-axis. */
			float GetAngleZ() const { return this->mAngleZ; }

			/*! Returns the max angle for the x-axis. */
			float GetMaxAngleX() const { return this->mMaxAngleX; }

			/*! Returns the max angle for the z-axis. */
			float GetMaxAngleZ() const { return this->mMaxAngleZ; }

			/*! Returns if the platform is going to rotate or not (around the x and z axis). */
			bool GetRotate() const { return this->mIsRotating; }


			//Set-Functions
	
			/*! Sets the shrinking value, the platform will shrink by x units per second. */
			void SetShrinkValue(const float shrinkVal) { this->mShrink = shrinkVal; }

			/*! Sets the Hot Zone Position */
			void SetHotZonePosition(const Vector3 hotZonePos) { this->mHotZonePosition = hotZonePos; }

			/*! Sets the Hot Zone Radius. */
			void SetHotZoneRadius(float radius) { this->mHotZoneRadius = radius; }

			/*! Sets the Maximum time for hot zone. */
			void SetMaxTimeInHotZone(float maxTime) { this->mMaxTimeInHotZone = maxTime; }

			/*! Sets the Mesh of the Hotzone. */
			void SetMeshHotZone(StaticMesh* mesh) { this->mMeshHotZone = mesh; }

			/*! Updates the platform. For example: shrink/rotate/wobble. */
			void Update(const float dt);
	
			/*! Sets the scaling of the platform. */
			void SetScale(Vector3 scale) { this->mMesh->Scale(scale.GetD3DVec()); }

			/*! Sets the target angle in x-axis- */
			void SetTargetAngleX(float angleX) { this->mTargetAngleX = angleX; }

			/*! Sets the target angle in Z-axis- */
			void SetTargetAngleZ(float angleZ) { this->mTargetAngleZ = angleZ; }

			/*! Sets the max angle for the x-axis. */
			void SetMaxAngleX(float angle) { this->mMaxAngleX = angle; }

			/*! Sets the max angle for the z-axis. */
			void SetMaxAngleZ(float angle) { this->mMaxAngleZ = angle; }

			/*! Sets if the platform is going to rotate or not (around the x and z axis). */
			void SetRotate(bool rotate) { this->mIsRotating = rotate; }

			/*! Sets the restitution a.k.a. e value for the platform. */
			void SetRestition(float restitution) { this->mRestitution = restitution; }

			/*! Sets the position to the start position. */
			virtual void SetToStartPosition();

			/*! Reset the platforms scale. */
			void Reset();

			/*! Rotates the mesh around the x-axis. */
			void RotateX(float dt);

			/*! Rotates the mesh around the z-axis. */
			void RotateZ(float dt);

			/*! Returns true if the target angle is reached for x-axis. */
			bool IsTargetAngleReachedX() const;

			/*! Returns true if the target angle is reached for z-axis. */
			bool IsTargetAngleReachedZ() const;

			/*! Returns true if the XZ position is on the platform. */
			bool IsOnPlatform(const float x, const float z) const;

			/*! Returns true if ball is in Hot Zone. */
			bool IsInHotZone(Vector3 positionOfBall, float radiusBall);


};
