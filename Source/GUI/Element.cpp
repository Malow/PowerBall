#include "Element.h"

Element::Element()
{
	this->mX = 0;
	this->mY = 0;
	this->mZ = 0;

	this->mTextureName = "null";

	this->mWidth = 0;
	this->mHeight = 0;

	this->mImage = 0;
}

Element::Element(float x, float y, float z, string textureName, int width, int height)
{
	this->mX = x;
	this->mY = y;
	this->mZ = z;

	this->mTextureName = textureName;

	this->mWidth = width;
	this->mHeight = height;

	this->mImage = 0;
}
Element::~Element()
{
	if(this->mImage)
		delete [] this->mImage;
}

bool Element::AddToRenderer(GraphicsEngine* ge)
{
	this->mImage = ge->CreateImage(D3DXVECTOR2(this->mX, this->mY), D3DXVECTOR2(this->mWidth, this->mHeight), this->mTextureName);

	return true;
}
bool Element::RemoveFromRenderer(GraphicsEngine* ge)
{
	ge->DeleteImage(this->mImage);
	this->mImage = 0;
	return true;
}