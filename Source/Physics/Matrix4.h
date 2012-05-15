#pragma once
/**
* Author: Jerry Rahmqvist
* Create Date: 07/05/2012
* 
* 
**/
#include "..\Physics\Vector4.h"
#include "..\Physics\Matrix3.h"
#include "..\GraphicsEngine\MaloWLib\Vector.h"
using namespace MaloW;

class Matrix4
{
private:
	float mData[16];
public:
	Matrix4(bool homogenousMatrix = false);
	Matrix4(float arr[]);
	Matrix4(D3DXMATRIX mat);
	~Matrix4();
	Matrix4 operator*(const Matrix4& m) const;
	Vector4 operator*(const Vector4& vec) const;
	Vector3 operator*(const Vector3& vec) const;
	Matrix4 operator*(const float& s) const;
	Matrix3 GetSubMatrix(int i, int j) const;
	float GetDeterminant() const;
	Matrix4 GetAdjoint() const;
	Matrix4 GetInverse() const;
	Matrix4 GetTranspose() const;
	D3DXMATRIX GetD3DXMatrix() const;
	void TransposeThis();
	void LoadIdentity();
	void SetScale(float xS, float yS, float zS);
	void SetScale(Vector3 scale);
	void SetRotationX(float angleX);
	void SetRotationY(float angleY);
	void SetRotationZ(float angleZ);
	void SetTranslate(float xT, float yT, float zT);
	void SetTranslate(Vector3 translate);
	void SetElement(float value, int elementNr);
};