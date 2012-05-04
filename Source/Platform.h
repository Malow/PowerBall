#pragma once
#include <string>
#include "Vector.h"
#include "GraphicsEngine.h"
using namespace MaloW;
using namespace std;

//platform class, perhaps make this one abstract and have all the different platform types inherit this class.


class Platform
{
private:
	StaticMesh*	mMesh;
	float		mShrink;
	float		mRadius;
	float		mScaledRadius;
	float		mMass;
	float		mRestitution;
	Vector3		mHotZonePosition;
	float		mHotZoneRadius;
	float		mMaxTimeInHotZone;

public:
	//constructors and destructors
	
	/*! Initializes the Platform and loads assigned mesh.*/
	Platform(const string meshFilePath, D3DXVECTOR3 position);
	virtual	~Platform();
	
	//Get-Functions

	/*! Returns the Y-value of the x,z position in the world (Returns -1 if outside the platform). */
	float GetY(const float x, const float z) const;
	
	/*! Returns the bilinear interpolated Y-value of surrounding vertices of the x,z position in the world (Returns -1 if outside the platform). */
	float GetYInterpolated(const float x, const float z) const;
	
	/*! Returns the value for platform shrinking, the platform will shrink by x units per second (Returns 0 if not shrinking). */
	float GetShrinkValue() const { return this->mShrink; }

	/*! Returns a pointer to the mesh of this platform. */
	StaticMesh* GetMesh() { return this->mMesh; }
	
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
	//Set-Functions
	
	/*! Sets the shrinking value, the platform will shrink by x units per second. */
	void SetShrinkValue(const float shrinkVal) { this->mShrink = shrinkVal; }

	/*! Sets the Hot Zone Position */
	void SetHotZonePosition(const Vector3 hotZonePos) { this->mHotZonePosition = hotZonePos; }

	/*! Sets the Hot Zone Radius. */
	void SetHotZoneRadius(float radius) { this->mHotZoneRadius = radius; }

	/*! Sets the Maximum time for hot zone. */
	void SetMaxTimeInHotZone(float maxTime) { this->mMaxTimeInHotZone = maxTime; }

	/*! Updates the platform. For example: shrink/rotate/wobble. */
	void Update(const float dt);
	
	/*! Sets the scaling of the platform. */
	void SetScale(Vector3 scale) { this->mMesh->Scale(scale.GetD3DVec()); }

	/*! Reset the platforms scale. */
	void Reset();

	/*! Returns true if the XZ position is on the platform. */
	bool IsOnPlatform(const float x, const float z) const;

	/*! Returns true if ball is in Hot Zone. */
	bool IsInHotZone(Vector3 positionOfBall, float radiusBall);


};