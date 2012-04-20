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
	Mesh*	mMesh;
	float	mShrink;
	float	mRadius;
	float   mScaledRadius;
	float	mMass;
	float   mRestitution;

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
	Mesh* GetMesh() { return this->mMesh; }
	
	/*! Returns the position of the platform in XZ plane */
	Vector3 GetPositionXZ() const;

	/*! Returns the mass of the platform. */
	float GetMass() const { return this->mMass; }

	/*! Returns the scaled radius */
	float GetScaledRadius() const { return this->mScaledRadius; }

	/*! Returns the Restitution- */
	float GetRestitution() const { return this->mRestitution; }
	//Set-Functions
	
	/*! Sets the shrinking value, the platform will shrink by x units per second. */
	void SetShrinkValue(const float shrinkVal) { this->mShrink = shrinkVal; }


	/*! Updates the platform. For example: shrink/rotate/wobble. */
	void Update(const float dt);
	
	/*! Returns true if the XZ position is on the platform. */
	bool IsOnPlatform(const float x, const float z) const;


};