#include "Element.h"

Element::Element()
{
	this->mX = 0;
	this->mY = 0;
	this->mZ = 0;

	this->mActiveWidth = 0;
	this->mActiveHeight = 0;

	this->mTextureName = "null";

	this->mWidth = 0;
	this->mHeight = 0;

	this->mImage = 0;
}

Element::Element(float x, float y, float z, string textureName, float width, float height, float activeX, float activeY, float activeWidth, float activeHeight, Event* tempEvent)
{
	this->mX = x;
	this->mY = y;
	this->mZ = z;

	this->mActiveX = activeX;
	this->mActiveY = activeY;

	this->mActiveWidth = activeWidth;
	this->mActiveHeight = activeHeight;

	this->mTextureName = textureName;

	this->mWidth = width;
	this->mHeight = height;

	this->mImage = 0;
	this->mEvent = tempEvent;
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

Event* Element::CheckCollision(float mouseX, float mouseY)
{
	if((mouseX < this->mActiveX || mouseX > (this->mActiveX+this->mActiveWidth)) || (mouseY < this->mActiveY || mouseY > (this->mActiveY+this->mActiveHeight))) // Outside
		return new NoEvent();
	else
		return this->mEvent;
}