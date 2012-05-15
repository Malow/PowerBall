#include "Matrix4.h"

Matrix4::Matrix4(bool homogeneousMatrix)
{
	if(homogeneousMatrix)
	{
		for(int i = 0;i<15;i++)
			this->mData[i] = 0.0f;
		this->mData[15] = 1.0f;
	}
	else
	{
		for(int i = 0;i<16;i++)
			this->mData[i] = 0.0f;
	}
}

Matrix4::Matrix4(float arr[])
{
	for(int i = 0;i<16;i++)
		this->mData[i] = arr[i];
}

Matrix4::Matrix4(D3DXMATRIX mat)
{
	this->mData[0] = mat._11;
	this->mData[1] = mat._12;
	this->mData[2] = mat._13;
	this->mData[3] = mat._14;
	this->mData[4] = mat._21;
	this->mData[5] = mat._22;
	this->mData[6] = mat._23;
	this->mData[7] = mat._24;
	this->mData[8] = mat._31;
	this->mData[9] = mat._32;
	this->mData[10] = mat._33;
	this->mData[11] = mat._34;
	this->mData[12] = mat._41;
	this->mData[13] = mat._42;
	this->mData[14] = mat._43;
	this->mData[15] = mat._44;

}

Matrix4::~Matrix4()
{

}

Matrix4 Matrix4::operator*(const Matrix4& m) const
{
	Matrix4 temp;
	for(int i = 0;i<4;i++)
	{
		for(int j = 0; j<4; j++)
			temp.mData[i*4 + j] = this->mData[i*4]*m.mData[j] + this->mData[i*4+1]*m.mData[j+4] + this->mData[i*4+2]*m.mData[j+8] + this->mData[i*4+3]*m.mData[j+12];
	}
	return temp;
}

Vector4 Matrix4::operator*(const Vector4& vec) const
{
	Vector4 temp;
	temp.x = this->mData[0]*vec.x + this->mData[1]*vec.y + this->mData[2]*vec.z + this->mData[3]*vec.w;
	temp.y = this->mData[4]*vec.x + this->mData[5]*vec.y + this->mData[6]*vec.z + this->mData[7]*vec.w;
	temp.z = this->mData[8]*vec.x + this->mData[9]*vec.y + this->mData[10]*vec.z + this->mData[11]*vec.w;
	temp.w = this->mData[12]*vec.x + this->mData[13]*vec.y + this->mData[14]*vec.z + this->mData[15]*vec.w;
	return temp;
}

Vector3 Matrix4::operator*(const Vector3& vec) const
{
	Vector3 temp;
	temp.x = this->mData[0]*vec.x + this->mData[1]*vec.y + this->mData[2]*vec.z + this->mData[3];
	temp.y = this->mData[4]*vec.x + this->mData[5]*vec.y + this->mData[6]*vec.z + this->mData[7];
	temp.z = this->mData[8]*vec.x + this->mData[9]*vec.y + this->mData[10]*vec.z + this->mData[11];
	return temp;
}

Matrix4 Matrix4::operator*(const float& s) const
{
	Matrix4 temp;
	temp = *this;
	for(int i = 0;i<16;i++)
		temp.mData[i] = temp.mData[i]*s;
	return temp;
}

Matrix3 Matrix4::GetSubMatrix(int row, int col) const
{
	Matrix3 temp;
	int indexCounted = 0;
	row = row - 1;
	col = col - 1;
	for(int i = 0;i<4;i++)
	{
		for(int j = 0;j<4;j++)
		{
			if(i != row  && j != col)
			{
				temp.SetElement(this->mData[i*4+j], indexCounted);
				indexCounted++;
			}
		}
	}
	return temp;
}

float Matrix4::GetDeterminant() const
{
	float detPart1 = this->mData[0]*this->GetSubMatrix(1,1).GetDeterminant() - this->mData[1]*this->GetSubMatrix(1,2).GetDeterminant();
	float detPart2 = this->mData[2]*this->GetSubMatrix(1,3).GetDeterminant() - this->mData[3]*this->GetSubMatrix(1,4).GetDeterminant();
	float det = detPart1 + detPart2;
	return det;
}

Matrix4 Matrix4::GetAdjoint() const
{
	Matrix4 adjoint;
	for(int i = 0;i<4;i++)
	{
		for(int j = 0;j<4;j++)
		{
			if( (i+j+2)%2 == 0)
				adjoint.mData[i*4 + j] = this->GetSubMatrix(j+1, i+1).GetDeterminant();
			else
				adjoint.mData[i*4 + j] = -this->GetSubMatrix(j+1, i+1).GetDeterminant();
		}
	}
	return adjoint;
}

Matrix4 Matrix4::GetInverse() const
{
	Matrix4 adjoint;
	adjoint = this->GetAdjoint();
	float det = this->GetDeterminant();
	Matrix4 inverse;
	inverse = adjoint*(1.0f/det);
	return inverse;
}

Matrix4 Matrix4::GetTranspose() const
{
	Matrix4 transpose = *this;
	transpose.TransposeThis();
	return transpose;
}

D3DXMATRIX Matrix4::GetD3DXMatrix() const
{
	// also transposes it
	D3DXMATRIX mat;
	mat._11 = this->mData[0];
	mat._12 = this->mData[4];
	mat._13 = this->mData[8];
	mat._14 = this->mData[12];
	mat._21 = this->mData[1];
	mat._22 = this->mData[5];
	mat._23 = this->mData[9];
	mat._24 = this->mData[13];
	mat._31 = this->mData[2];
	mat._32 = this->mData[6];
	mat._33 = this->mData[10];
	mat._34 = this->mData[14];
	mat._41 = this->mData[3];
	mat._42 = this->mData[7];
	mat._43 = this->mData[11];
	mat._44 = this->mData[15];
	return mat;
}

void Matrix4::SetScale(float xS, float yS, float zS)
{
	// the scale
	this->mData[0] = xS;
	this->mData[5] = yS;
	this->mData[10] = zS;

	// rest
	this->mData[1] = 0.0f;
	this->mData[2] = 0.0f;
	this->mData[3] = 0.0f;
	this->mData[4] = 0.0f;
	this->mData[6] = 0.0f;
	this->mData[7] = 0.0f;
	this->mData[8] = 0.0f;
	this->mData[9] = 0.0f;
	this->mData[11] = 0.0f;
	this->mData[12] = 0.0f;
	this->mData[13] = 0.0f;
	this->mData[14] = 0.0f;
	this->mData[15] = 1.0f;
}

void Matrix4::LoadIdentity() 
{
	this->mData[0] = 1.0f;
	this->mData[1] = 0.0f;
	this->mData[2] = 0.0f;
	this->mData[3] = 0.0f;

	this->mData[4] = 0.0f;
	this->mData[5] = 1.0f;
	this->mData[6] = 0.0f;
	this->mData[7] = 0.0f;

	this->mData[8] = 0.0f;
	this->mData[9] = 0.0f;
	this->mData[10] = 1.0f;
	this->mData[11] = 0.0f;

	this->mData[12] = 0.0f;
	this->mData[13] = 0.0f;
	this->mData[14] = 0.0f;
	this->mData[15] = 0.0f;
}

void Matrix4::TransposeThis()
{
	Matrix4 temp;
	for(int i = 0;i<4;i++)
		for(int j = 0;j<4;j++)
			temp.mData[i*4+j] = this->mData[j*4+i];
	*this = temp;
}

void Matrix4::SetScale(Vector3 scale)
{
	this->SetScale(scale.x, scale.y, scale.z);
}

void Matrix4::SetRotationX(float angleX)
{
	// x-axis (keep it)
	this->mData[0] = 1.0f;
	this->mData[4] = 0.0f;
	this->mData[8] = 0.0f;

	// y-axis
	this->mData[1] = 0.0f;
	this->mData[5] = cos(angleX);
	this->mData[9] = sin(angleX);

	// z-axis
	this->mData[2] = 0.0f;
	this->mData[6] = -sin(angleX);
	this->mData[10] = cos(angleX);

	// rest
	this->mData[3] = 0.0f;
	this->mData[7] = 0.0f;
	this->mData[11] = 0.0f;
	this->mData[12] = 0.0f;
	this->mData[13] = 0.0f;
	this->mData[14] = 0.0f;
	this->mData[15] = 1.0f;
}

void Matrix4::SetRotationY(float angleY)
{
	// x-axis
	this->mData[0] = cos(angleY);
	this->mData[4] = 0.0f;
	this->mData[8] = -sin(angleY);

	// y-axis (keep it)
	this->mData[1] = 0.0f;
	this->mData[5] = 1.0f;
	this->mData[9] = 0.0f;


	// z-axis
	this->mData[2] = sin(angleY);
	this->mData[6] = 0.0f;
	this->mData[10] = cos(angleY);

	// rest
	this->mData[3] = 0.0f;
	this->mData[7] = 0.0f;
	this->mData[11] = 0.0f;
	this->mData[12] = 0.0f;
	this->mData[13] = 0.0f;
	this->mData[14] = 0.0f;
	this->mData[15] = 1.0f;
}

void Matrix4::SetRotationZ(float angleZ)
{
	// x-axis
	this->mData[0] = cos(angleZ);
	this->mData[4] = sin(angleZ);
	this->mData[8] = 0.0f;

	// y-axis
	this->mData[1] = -sin(angleZ);
	this->mData[5] = cos(angleZ);
	this->mData[9] = 0.0f;

	// z-axis (keep it)
	this->mData[2] = 0.0f;
	this->mData[6] = 0.0f;
	this->mData[10] = 1.0f;

	// rest
	this->mData[3] = 0.0f;
	this->mData[7] = 0.0f;
	this->mData[11] = 0.0f;
	this->mData[12] = 0.0f;
	this->mData[13] = 0.0f;
	this->mData[14] = 0.0f;
	this->mData[15] = 1.0f;
}

void Matrix4::SetTranslate(float xT, float yT, float zT)
{
	// the translation
	this->mData[3] = xT;
	this->mData[7] = yT;
	this->mData[11]= zT;

	// rest
	this->mData[0] = 1.0f;
	this->mData[1] = 0.0f;
	this->mData[2] = 0.0f;
	this->mData[4] = 0.0f;
	this->mData[5] = 1.0f;
	this->mData[6] = 0.0f;
	this->mData[8] = 0.0f;
	this->mData[9] = 0.0f;
	this->mData[10] = 1.0f;
	this->mData[12] = 0.0f;
	this->mData[13] = 0.0f;
	this->mData[14] = 0.0f;
	this->mData[15] = 1.0f;
}

void Matrix4::SetTranslate(Vector3 translate)
{
	this->SetTranslate(translate.x, translate.y, translate.z);
}

void Matrix4::SetElement(float value, int elementNr)
{
	this->mData[elementNr] = value;
}