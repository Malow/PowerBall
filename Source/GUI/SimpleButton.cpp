#include "SimpleButton.h"

SimpleButton::SimpleButton() : Element()
{
	this->mActiveX = 0;
	this->mActiveY = 0;

	this->mActiveWidth = 0;
	this->mActiveHeight = 0;
	
	this->mEvent = NULL;
}
SimpleButton::~SimpleButton()
{
	SAFE_DELETE(this->mEvent);
}
SimpleButton::SimpleButton(float x, float y, float z, string textureName, float width, float height, GUIEvent* tempEvent) 
	: Element(x, y, z, textureName, width, height)
{
	this->mActiveX = x;
	this->mActiveY = y;
	
	this->mActiveWidth = width;
	this->mActiveHeight = height;

	this->mEvent = tempEvent;
}
GUIEvent* SimpleButton::CheckCollision(float mouseX, float mouseY)
{
	if((mouseX < this->mActiveX || mouseX > (this->mActiveX+this->mActiveWidth)) || (mouseY < this->mActiveY || mouseY > (this->mActiveY+this->mActiveHeight))) // Outside
		return NULL;
	else
		return this->GetEvent();
}