#include "Element.h"
#include "..\stdafx.h"

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

	this->mImage = NULL;
	this->mEvent = NULL;
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
	
	this->mImage = NULL;
	this->mEvent = tempEvent;
}
/*Element::Element(Element& origObj)
{
	origObj.GetPosition(this->mX, this->mY, this->mZ);
	
	origObj.GetWidth(this->mWidth);
	origObj.GetHeight(this->mHeight);

	origObj.GetActivePos(this->mActiveX, this->mActiveY);

	origObj.GetActiveWidth(this->mActiveWidth);
	origObj.GetActiveHeight(this->mActiveHeight);
	
	origObj.GetTextureName(this->mTextureName);

	this->mImage = origObj.GetImage();
	origObj.GetEvent(this->mEvent);
}*/
Element::~Element()
{
	SAFE_DELETE(this->mEvent);
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

Event* Element::CheckCollision(float mouseX, float mouseY)
{
	if((mouseX < this->mActiveX || mouseX > (this->mActiveX+this->mActiveWidth)) || (mouseY < this->mActiveY || mouseY > (this->mActiveY+this->mActiveHeight))) // Outside
		return NULL;
	else
		return this->mEvent;
}