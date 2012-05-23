#pragma once
/**
* Author: Jerry Rahmqvist
* Create Date: 19/05/2012
* 
* 
**/
#include "..\GraphicsEngine\MaloWLib\Vector.h"
#include "..\Physics\Matrix4.h"
using MaloW::Vector3;

class Quaternion
{
	public:
		float mX;
		float mY;
		float mZ;
		float mW;

	public:
		Quaternion();
		Quaternion(const float& x, const float& y, const float& z, const float& w);
		Quaternion(float rotationAngle, const Vector3 & aroundAxis);
		Quaternion(const Matrix4& matrix);
		~Quaternion();
		Quaternion operator*(const Quaternion& q) const;
		friend inline Quaternion operator*(const Quaternion &a, float s);
		friend inline Quaternion operator*(float s, const Quaternion &a);
		friend inline Quaternion operator+(const Quaternion &a, const Quaternion &b);
		Quaternion GetComplexConjugate() const;
		float GetNorm() const;
		float GetLength() const;
		void Normalize();
		Matrix4 GetMatrix() const;
		Quaternion Slerp(const Quaternion& a, const Quaternion& b, float t);

};
