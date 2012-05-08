#pragma once
/**
* Author: Jerry Rahmqvist
* Create Date: 07/05/2012
* 
* 
**/
class Vector4
{
	public:
		float x;
		float y;
		float z;
		float w;
		Vector4() { x = y = z = w = 0.0f; }
		Vector4(float a, float b, float c, float d) { this->x = a; this->y = b; this->z = c; this->w = d; }
		~Vector4(){ }

};