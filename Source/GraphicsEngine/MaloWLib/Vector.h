#ifndef MALOWVECTOR
#define MALOWVECTOR

#include "stdafx.h"

namespace MaloW
{
	class Vector2
	{
	public:
		float x;
		float y;

		Vector2()
		{
			this->x = 0;
			this->y = 0;
		}

		Vector2(float _x, float _y)
		{
			this->x = _x;
			this->y = _y;
		}

		virtual ~Vector2() {}

		D3DXVECTOR2 GetD3DVec() 
		{
			return D3DXVECTOR2(this->x, this->y);
		}

		float GetLength()
		{
			return sqrt(pow(this->x, 2) + pow(this->y, 2));
		}

		void normalize()
		{
			float length = this->GetLength();
			this->x /= length;
			this->y /= length;
		}
		/*
		float GetAngle(Vector2& compObj)
		{
			
			float diffx = this->x - compObj.x;
			float diffy = this->y - compObj.y;

			
		}*/

	};


	class Vector3
	{
	public:
		float x;
		float y;
		float z;

		Vector3()
		{
			this->x = 0;
			this->y = 0;
			this->z = 0;
		}

		Vector3(float _x, float _y, float _z)
		{
			this->x = _x;
			this->y = _y;
			this->z = _z;
		}

		Vector3(D3DXVECTOR3 vec)
		{
			this->x = vec.x;
			this->y = vec.y;
			this->z = vec.z;
		}

		virtual ~Vector3() {}



		D3DXVECTOR3 GetD3DVec() 
		{
			return D3DXVECTOR3(this->x, this->y, this->z);
		}

		float GetLength()
		{
			return sqrt(pow(this->x, 2) + pow(this->y, 2) + pow(this->z, 2));
		}

		void normalize()
		{
			float length = this->GetLength();
			this->x /= length;
			this->y /= length;
			this->z /= length;
		}

		float GetDotProduct(Vector3& compObj)
		{
			float dot = this->x * compObj.x;
			dot += this->y * compObj.y;
			dot += this->z * compObj.z;
			return dot;
		}

		Vector3 GetCrossProduct(Vector3 vec)
		{
			Vector3 retVec;
			retVec.x = this->y * vec.z - vec.y * this->z;
			retVec.y = -(this->x * vec.z - vec.x * this->x);
			retVec.z = this->x * vec.y - vec.x * this->y;

			return retVec;
		}

		float GetAngle(Vector3& compObj)
		{
			return acos(this->GetDotProduct(compObj) / (this->GetLength() * compObj.GetLength()));
		}
		// new for physics
		
		Vector3 operator+(const Vector3& v) const
        {
            return Vector3(this->x+v.x, this->y+v.y, this->z+v.z);
        }
		Vector3 operator-(const Vector3& v) const
		{
			return Vector3(this->x-v.x, this->y-v.y, this->z-v.z);
		}
		Vector3 operator*(const float& scalar) const
		{
			return Vector3(this->x*scalar, this->y*scalar, this->z*scalar);
		}
		Vector3 operator/(const float& scalar) const
		{
			return Vector3(this->x/scalar, this->y/scalar, this->z/scalar);
		}
		void operator+=(const Vector3& v)
        {
            x += v.x;
            y += v.y;
            z += v.z;
        }
		void operator-=(const Vector3& v)
        {
            x -= v.x;
            y -= v.y;
            z -= v.z;
        }
		void operator*=(const float scalar)
        {
            x *= scalar;
            y *= scalar;
            z *= scalar;
        }
	};
};

#endif