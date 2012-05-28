#include "Quaternion.h"
#include "Matrix4.h"
#include <math.h>
Quaternion::Quaternion()
{
	this->mX = 0.0f;
	this->mY = 0.0f;
	this->mZ = 0.0f;
	this->mW = 0.0f;
}

Quaternion::Quaternion(const float& x, const float& y, const float& z, const float& w)
{
	this->mX = x;
	this->mY = y;
	this->mZ = z;
	this->mW = w;
}


/* this is constructing a quaternion from a rotation around an axis and with 'theta' as angle. */

Quaternion::Quaternion(float rotationAngle, const Vector3 & aroundAxis)
{
	const float halfTheta = rotationAngle * 0.5f;
	const float s = (float) sin(halfTheta);
	const float c = (float) cos(halfTheta);
	this->mX = aroundAxis.x * s;
	this->mY = aroundAxis.y * s;
	this->mZ = aroundAxis.z * s;
	this->mW = c;
}

/* this is constructing a quaternion from an rotation matrix  4x4.  Right handed. */
/* to do  check what to fix so this is working for both right handed and lefthanded. */
Quaternion::Quaternion(const Matrix4& matrix)
{
	const float trace = 1.0f + matrix.GetRowCol(1,1) + matrix.GetRowCol(2,2) + matrix.GetRowCol(3,3);

	if (trace > 0.00001f)
	{
		const float s = sqrt(trace) * 2;
		this->mX = (matrix.GetRowCol(3,2) - matrix.GetRowCol(2,3)) / s;
		this->mY = (matrix.GetRowCol(1,3) - matrix.GetRowCol(3,1)) / s;
		this->mZ = (matrix.GetRowCol(2,1) - matrix.GetRowCol(1,2)) / s;
		this->mW = s / 4;
	}
	else if (matrix.GetRowCol(1,1) > matrix.GetRowCol(2,2) && matrix.GetRowCol(1,1) > matrix.GetRowCol(3,3))
	{
		const float s = sqrt(1.0f + matrix.GetRowCol(1,1) - matrix.GetRowCol(2,2) - matrix.GetRowCol(3,3)) * 2;
		this->mX = s / 4;
		this->mY = (matrix.GetRowCol(2,1) + matrix.GetRowCol(1,2)) / s;
		this->mZ = (matrix.GetRowCol(1,3) + matrix.GetRowCol(3,1)) / s;
		this->mW = (matrix.GetRowCol(3,2) - matrix.GetRowCol(2,3)) / s;
	}
	else if (matrix.GetRowCol(1,1) > matrix.GetRowCol(2,2))
	{
		const float s = sqrt(1.0f + matrix.GetRowCol(2,2) - matrix.GetRowCol(1,1) - matrix.GetRowCol(3,3)) * 2;
		this->mX = (matrix.GetRowCol(2,1) + matrix.GetRowCol(1,2)) / s;
		this->mY = s / 4;
		this->mZ = (matrix.GetRowCol(3,2) + matrix.GetRowCol(2,3)) / s;
		this->mW = (matrix.GetRowCol(1,3) - matrix.GetRowCol(3,1)) / s;
		
	}
	else
	{
		const float s = sqrt(1.0f + matrix.GetRowCol(3,3) - matrix.GetRowCol(1,1) - matrix.GetRowCol(2,2)) * 2;
		this->mX = (matrix.GetRowCol(1,3) + matrix.GetRowCol(3,1)) / s;
		this->mY = (matrix.GetRowCol(3,2) + matrix.GetRowCol(2,3)) / s;
		this->mZ = s / 4;
		this->mW = (matrix.GetRowCol(2,1) - matrix.GetRowCol(1,2)) / s;
	}

}


Quaternion::~Quaternion()
{

}

Quaternion Quaternion::operator*(const Quaternion& q) const
{
	Quaternion temp;
	temp.mW = this->mW*q.mW - this->mX*q.mX - this->mY*q.mY - this->mZ*q.mZ;
	temp.mX = this->mW*q.mX + this->mX*q.mW + this->mY*q.mZ - this->mZ*q.mY;
	temp.mY = this->mW*q.mY + this->mY*q.mW + this->mZ*q.mX - this->mX*q.mZ;
	temp.mZ = this->mW*q.mZ + this->mZ*q.mW + this->mX*q.mY - this->mY*q.mX;
	return temp;
}

Quaternion Quaternion::GetComplexConjugate() const
{
	Quaternion temp = Quaternion(-this->mX, -this->mY, -this->mZ, this->mW);
	return temp;
}

float Quaternion::GetNorm() const
{
	return (this->mX * this->mX + this->mY * this->mY + this->mZ * this->mZ + this->mW * this->mW);
}

float Quaternion::GetLength() const
{
	return sqrt(this->mX * this->mX + this->mY * this->mY + this->mZ * this->mZ + this->mW * this->mW);
}
void Quaternion::Normalize()
{
	const float length = this->GetLength();
	if (length == 0)
	{
		this->mW = 1;
		this->mX = 0;
		this->mY = 0;
		this->mZ = 0;
	}
	else
	{
		float inv = 1.0f / length;
		this->mX = this->mX * inv;
		this->mY = this->mY * inv;
		this->mZ = this->mZ * inv;
		this->mW = this->mW * inv;
	}
}

Matrix4 Quaternion::GetMatrix() const
{
	float m1[16] = {this->mW, this->mZ, -this->mY, this->mX, -this->mZ, this->mW, this->mX, this->mY,  this->mY, -this->mX, this->mW, this->mZ, -this->mX, -this->mY, -this->mZ, this->mW };
	Matrix4 mat1(m1);
	float m2[16] = { this->mW, this->mZ, -this->mY, -this->mX, -this->mZ, this->mW, this->mX, -this->mY, this->mY, -this->mX, this->mW, -this->mZ, this->mX, this->mY, this->mZ, this->mW };
	Matrix4 mat2(m2);
	Matrix4 newM;
	newM = mat1*mat2;
	return newM;
}

Quaternion Quaternion::Slerp(const Quaternion &a, const Quaternion &b, float t) 
{
	float epsilon = 0.0001f;
	if( t < 0 || t > 1)
		return Quaternion();
				
	float flip = 1;

	float cosine = a.mW*b.mW + a.mX*b.mX + a.mY*b.mY + a.mZ*b.mZ;
		
	if (cosine<0) 
	{ 
		cosine = -cosine; 
		flip = -1; 
	} 
		
	if ((1-cosine)<epsilon) 
		return a * (1-t) + b * (t*flip); 
	
	float theta = (float)acos(cosine); 
	float sine = (float)sin(theta); 
	float beta = (float)sin((1-t)*theta) / sine; 
	float alpha = (float)sin(t*theta) / sine * flip; 
	
	return a * beta + b * alpha; 
} 

inline Quaternion operator*(const Quaternion &a, float s)
{
	return Quaternion(a.mX*s, a.mY*s, a.mZ*s, a.mW*s);
}

inline Quaternion operator*(float s, const Quaternion &a)
{
	return Quaternion(a.mX*s, a.mY*s, a.mZ*s, a.mW*s);
}

inline Quaternion operator+(const Quaternion &a, const Quaternion &b)
{
	return Quaternion(a.mX+b.mX, a.mY+b.mY, a.mZ+b.mZ, a.mW+b.mW);
}