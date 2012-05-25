#include "TextButton.h"

TextButton::TextButton() : Element()
{
	this->mActiveX = 0;
	this->mActiveY = 0;

	this->mActiveWidth = 0;
	this->mActiveHeight = 0;

	this->mText = "";

	this->mEvent = NULL;
	this->mTextPointer = NULL;
}
TextButton::TextButton(float x, float y, float z, string textureName, float width, float height, string text, GUIEvent* tempEvent) : Element(x, y, z, textureName, width, height)
{
	this->mActiveX = x;
	this->mActiveY = y;

	this->mActiveWidth = width;
	this->mActiveHeight = height;

	this->mText = text;

	this->mEvent = tempEvent;
	this->mTextPointer = NULL;
}
TextButton::~TextButton()
{

}

bool TextButton::AddToRenderer(GraphicsEngine* ge)
{
	Element::AddToRenderer(ge);
	D3DXVECTOR2 temp = this->GetPositionD3D();
	this->mTextPointer = ge->CreateText(this->mText, D3DXVECTOR2(temp.x + 5, temp.y+2), 1, "Media/Fonts/1");

	return true;
}
bool TextButton::RemoveFromRenderer(GraphicsEngine* ge)
{
	Element::RemoveFromRenderer(ge);
	ge->DeleteText(this->mTextPointer);

	return true;
}

GUIEvent* TextButton::CheckCollision(float mouseX, float mouseY, bool mousePressed, GraphicsEngine* ge)
{
	if((mouseX < this->mActiveX || mouseX > (this->mActiveX+this->mActiveWidth)) || (mouseY < this->mActiveY || mouseY > (this->mActiveY+this->mActiveHeight)))
	{
		return NULL;
	}
	else
	{
		if(!mousePressed)
		{
			if(this->mPressed)
			{
				this->mPressed = false;
				GameOptions::mouseClick->Play();
				return this->GetEvent();
			}
		}
	}
	return NULL;
}