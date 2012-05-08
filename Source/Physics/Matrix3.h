#pragma once
/**
* Author: Jerry Rahmqvist
* Create Date: 07/05/2012
* 
* 
**/
#include "..\GraphicsEngine\MaloWLib\Vector.h"
using namespace MaloW;

class Matrix3
{
private:
	float mData[9];
public:
	Matrix3(bool identity = false);
	Matrix3(float arr[]);
	Matrix3(D3DXMATRIX mat);
	~Matrix3();
	Matrix3 operator*(const Matrix3& m) const;
	Vector3 operator*(const Vector3& vec) const;
	Matrix3 operator*(const float& s) const;
	float GetDeterminant() const;
	float GetDeterminant(const Matrix3& m) const;
	Matrix3 GetAdjoint() const;
	Matrix3 GetInverse() const;
	void TransposeThis();
	void SetScale(float xS, float yS, float zS);
	void SetScale(Vector3 scale);
	void SetRotationX(float angleX);
	void SetRotationY(float angleY);
	void SetRotationZ(float angleZ);
	void SetElement(float value, int elementNr);
};