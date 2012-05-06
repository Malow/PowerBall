#include "Element.h"
#include "..\stdafx.h"

Element::Element()
{
	this->mX = 0;
	this->mY = 0;
	this->mZ = 0;

	this->mTextureName = "null";

	this->mWidth = 0;
	this->mHeight = 0;

	this->mImage = NULL;
}

Element::Element(float x, float y, float z, string textureName, float width, float height)
{
	this->mX = x;
	this->mY = y;
	this->mZ = z;
	
	this->mTextureName = textureName;

	this->mWidth = width;
	this->mHeight = height;
	
	this->mImage = NULL;
}
Element::~Element()
{

}
bool Element::AddToRenderer(GraphicsEngine* ge)
{
	this->mImage = ge->CreateImage(D3DXVECTOR2(this->mX, this->mY), D3DXVECTOR2(this->mWidth, this->mHeight), this->mTextureName);

	return true;
}
bool Element::RemoveFromRenderer(GraphicsEngine* ge)
{
	ge->DeleteImage(this->mImage);
	this->mImage = NULL;

	return true;
}