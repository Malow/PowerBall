#include "Matrix3.h"

Matrix3::Matrix3(bool identity)
{
	if(identity)
	{
		this->mData[0] = 1.0f;
		this->mData[1] = 0.0f;
		this->mData[2] = 0.0f;
		this->mData[3] = 0.0f;
		this->mData[4] = 1.0f;
		this->mData[5] = 0.0f;
		this->mData[6] = 0.0f;
		this->mData[7] = 0.0f;
		this->mData[8] = 1.0f;
	}
	else
	{
		for(int i = 0;i<9;i++)
			this->mData[i] = 0.0f;
	}
	
}

Matrix3::Matrix3(float arr[])
{
	for(int i = 0;i<9;i++)
		this->mData[i] = arr[i];
}

Matrix3::Matrix3(D3DXMATRIX mat)
{
	this->mData[0] = mat._11;
	this->mData[1] = mat._12;
	this->mData[2] = mat._13;
	this->mData[3] = mat._21;
	this->mData[4] = mat._22;
	this->mData[5] = mat._23;
	this->mData[6] = mat._31;
	this->mData[7] = mat._32;
	this->mData[8] = mat._33;
	
}

Matrix3::~Matrix3()
{

}

Matrix3 Matrix3::operator*(const Matrix3& m) const
{
	Matrix3 temp;
	for(int i = 0;i<3;i++)
	{
		for(int j = 0; j<3; j++)
			temp.mData[i*3 + j] = this->mData[i*3]*m.mData[j] + this->mData[i*3+1]*m.mData[j+3] + this->mData[i*3+2]*m.mData[j+6];
	}
	return temp;
}

Vector3 Matrix3::operator*(const Vector3& vec) const
{
	Vector3 temp;
	temp.x = this->mData[0]*vec.x + this->mData[1]*vec.y + this->mData[2]*vec.z;
	temp.y = this->mData[3]*vec.x + this->mData[4]*vec.y + this->mData[5]*vec.z;
	temp.z = this->mData[6]*vec.x + this->mData[7]*vec.y + this->mData[8]*vec.z;
	return temp;
}

Matrix3 Matrix3::operator*(const float& s) const
{
	Matrix3 temp;
	temp = *this;
	for(int i = 0;i<9;i++)
		temp.mData[i] = temp.mData[i]*s;
	return temp;
}

float Matrix3::GetDeterminant() const
{
	float detP = this->mData[0]*this->mData[4]*this->mData[8] + this->mData[1]*this->mData[5]*this->mData[6] + this->mData[2]*this->mData[3]*this->mData[7];
	float detN = this->mData[6]*this->mData[4]*this->mData[2] + this->mData[7]*this->mData[5]*this->mData[0] + this->mData[8]*this->mData[3]*this->mData[1];
	float det = detP-detN;
	return det;
}

float Matrix3::GetDeterminant(const Matrix3& m) const
{
	float detP = m.mData[0]*m.mData[4]*m.mData[8] + m.mData[1]*m.mData[5]*m.mData[6] + m.mData[2]*m.mData[3]*m.mData[7];
	float detN = m.mData[6]*m.mData[4]*m.mData[2] + m.mData[7]*m.mData[5]*m.mData[0] + m.mData[8]*m.mData[3]*m.mData[1];
	float det = detP-detN;
	return det;
}

Matrix3 Matrix3::GetAdjoint() const
{
	Matrix3 adjoint;
	adjoint.mData[0] = this->mData[4]*this->mData[8] - this->mData[7]*this->mData[5];
	adjoint.mData[1] = -(this->mData[1]*this->mData[8] - this->mData[7]*this->mData[2]);
	adjoint.mData[2] = this->mData[1]*this->mData[5] - this->mData[4]*this->mData[2];
	adjoint.mData[3] = -(this->mData[3]*this->mData[8] - this->mData[6]*this->mData[5]);
	adjoint.mData[4] = this->mData[0]*this->mData[8] - this->mData[6]*this->mData[2];
	adjoint.mData[5] = -(this->mData[0]*this->mData[5] - this->mData[3]*this->mData[2]);
	adjoint.mData[6] = this->mData[3]*this->mData[7] - this->mData[6]*this->mData[4];
	adjoint.mData[7] = -(this->mData[0]*this->mData[7] - this->mData[6]*this->mData[1]);
	adjoint.mData[8] = this->mData[0]*this->mData[4] - this->mData[3]*this->mData[1];
	return adjoint;
}

Matrix3 Matrix3::GetInverse() const
{
	Matrix3 adjoint;
	adjoint = this->GetAdjoint();
	Matrix3 inverse;
	inverse = adjoint*(1.0f/this->GetDeterminant());
	return inverse;
}

void Matrix3::SetScale(float xS, float yS, float zS)
{
	// the scale
	this->mData[0] = xS;
	this->mData[4] = yS;
	this->mData[8] = zS;

	// rest
	this->mData[1] = 0.0f;
	this->mData[2] = 0.0f;
	this->mData[3] = 0.0f;
	this->mData[5] = 0.0f;
	this->mData[6] = 0.0f;
	this->mData[7] = 0.0f;
	
}

void Matrix3::TransposeThis()
{
	Matrix3 temp;
	for(int i = 0;i<3;i++)
		for(int j = 0;j<3;j++)
			temp.mData[i*3+j] = this->mData[j*3+i];
	*this = temp;
}

void Matrix3::SetScale(Vector3 scale)
{
	this->SetScale(scale.x, scale.y, scale.z);
}

void Matrix3::SetRotationX(float angleX)
{
	// x-axis (keep it)
	this->mData[0] = 1.0f;
	this->mData[3] = 0.0f;
	this->mData[6] = 0.0f;

	// y-axis
	this->mData[1] = 0.0f;
	this->mData[4] = cos(angleX);
	this->mData[7] = sin(angleX);

	// z-axis
	this->mData[2] = 0.0f;
	this->mData[5] = -sin(angleX);
	this->mData[8] = cos(angleX);

}

void Matrix3::SetRotationY(float angleY)
{
	// x-axis
	this->mData[0] = cos(angleY);
	this->mData[3] = 0.0f;
	this->mData[6] = -sin(angleY);

	// y-axis (keep it)
	this->mData[1] = 0.0f;
	this->mData[4] = 1.0f;
	this->mData[7] = 0.0f;


	// z-axis
	this->mData[2] = sin(angleY);
	this->mData[5] = 0.0f;
	this->mData[8] = cos(angleY);

}

void Matrix3::SetRotationZ(float angleZ)
{
	// x-axis
	this->mData[0] = cos(angleZ);
	this->mData[3] = sin(angleZ);
	this->mData[6] = 0.0f;

	// y-axis
	this->mData[1] = -sin(angleZ);
	this->mData[4] = cos(angleZ);
	this->mData[7] = 0.0f;

	// z-axis (keep it)
	this->mData[2] = 0.0f;
	this->mData[5] = 0.0f;
	this->mData[8] = 1.0f;

}

void Matrix3::SetElement(float value, int elementNr)
{
	this->mData[elementNr] = value;
}