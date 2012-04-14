#include "SimpleButton.h"

SimpleButton::SimpleButton() : Element()
{
	this->mActiveX = 0;
	this->mActiveY = 0;

	this->mActiveWidth = 0;
	this->mActiveHeight = 0;
	
	this->mPressed = false;
	this->mHovered = false;

	this->mTextureNamePressed = " ";
	this->mTextureNameHovered = " ";

	this->mHoveredImage = NULL;
	this->mPressedImage = NULL;

	this->mEvent = NULL;
}
SimpleButton::~SimpleButton()
{
	SAFE_DELETE(this->mEvent);
}
SimpleButton::SimpleButton(float x, float y, float z, string textureName, float width, float height, GUIEvent* tempEvent,
		string mTextureNamePressed, string mTextureNameHovered, float activeX, float activeY, float activeWidth, float activeHeight) 
	: Element(x, y, z, textureName, width, height)
{
	this->mActiveX = activeX;
	this->mActiveY = activeY;
	
	this->mActiveWidth = activeWidth;
	this->mActiveHeight = activeHeight;

	this->mPressed = false;
	this->mHovered = false;

	this->mTextureNamePressed = mTextureNamePressed;
	this->mTextureNameHovered = mTextureNameHovered;

	this->mHoveredImage = NULL;
	this->mPressedImage = NULL;

	this->mEvent = tempEvent;
}
GUIEvent* SimpleButton::CheckCollision(float mouseX, float mouseY, bool mousePressed, GraphicsEngine* ge)
{
	if((mouseX < this->mActiveX || mouseX > (this->mActiveX+this->mActiveWidth)) || (mouseY < this->mActiveY || mouseY > (this->mActiveY+this->mActiveHeight))) // Outside
	{
		this->mPressed = false;
		if(this->mPressedImage != NULL)
		{
			ge->DeleteImage(this->mPressedImage);
			this->mPressedImage = NULL;
		}
		this->mHovered = false;
		if(this->mHoveredImage != NULL)
		{
			ge->DeleteImage(this->mHoveredImage);
			this->mHoveredImage = NULL;
		}
		return NULL;
	}
	else
	{
		if(mousePressed)
		{
			if(!this->mPressed)
			{
				this->mPressed = true;
				if(this->mPressedImage == NULL)
				{
					this->mPressedImage = ge->CreateImage(this->GetPositionD3D(), this->GetDimension(), this->mTextureNamePressed); 
				}

				this->mHovered = false;
				if(this->mHoveredImage != NULL)
				{
					ge->DeleteImage(this->mHoveredImage);
					this->mHoveredImage = NULL;
				}
			}
		}
		else if(!mousePressed)
		{
			if(this->mPressed)
			{
				this->mHovered = true;
				if(this->mHoveredImage == NULL)
				{
					this->mHoveredImage = ge->CreateImage(this->GetPositionD3D(), this->GetDimension(), this->mTextureNameHovered);
				}

				this->mPressed = false;
				if(this->mPressedImage != NULL)
				{
					ge->DeleteImage(this->mPressedImage);
					this->mPressedImage = NULL;
				}
				return this->GetEvent();
			}
			else
			{
				this->mHovered = true;
				if(this->mHoveredImage == NULL)
				{
					this->mHoveredImage = ge->CreateImage(this->GetPositionD3D(), this->GetDimension(), this->mTextureNameHovered);
				}
			}
		}
	}
	return NULL;
}
bool SimpleButton::RemoveFromRenderer(GraphicsEngine* ge)
{
	if(this->mHoveredImage != NULL)
	{
		ge->DeleteImage(this->mHoveredImage);
		this->mHoveredImage = NULL;
	}
	if(this->mPressedImage != NULL)
	{
		ge->DeleteImage(this->mPressedImage);
		this->mPressedImage = NULL;
	}
	Element::RemoveFromRenderer(ge);
	return true;
}