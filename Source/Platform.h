#pragma once
#include <string>
using namespace std;

//platform class, perhaps make this one abstract and have all the different platform types inherit this class.
class Platform
{
private:
	//Object*	mMesh;
	float		mShrink;
	//float		mRotation;
	//float		mWobblynessiiness;

public:
	//constructors and destructors
	
	/*! Initializes the Platform and loads assigned mesh.*/
				Platform(const string meshFilePath);
				Platform(const string meshFilePath, float shrinkValue);
	virtual		~Platform();
	
	//Get-Functions

	/*! Returns the Y-value of the x,z position in the world (Returns -1 if outside the platform). */
	float		GetY(const float x, const float z) const;
	
	/*! Returns the bilinear interpolated Y-value of surrounding vertices of the x,z position in the world (Returns -1 if outside the platform). */
	float		GetYInterpolated(const float x, const float z) const;

	/*! Returns the value for platform shrinking, the platform will shrink by x units per second (Returns 0 if not shrinking). */
	float		GetShrinkValue() const { return this->mShrink; }


	//Set-Functions
	
	/*! Sets the shrinking value, the platform will shrink by x units per second. */
	void		SetShrinkValue(const float shrinkVal) { this->mShrink = shrinkVal; }


	/*! Updates the platform. For example: shrink/rotate/wobble. */
	void		Update(const float dt);

};