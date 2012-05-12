#include "GUIArchButton.h"

GUIArchButton::GUIArchButton() : Element()
{
	this->mPressed = false;
	this->mHovered = false;

	this->mTextureNamePressed = "";
	this->mTextureNameHovered = "";

	this->mPressedImage = NULL;
	this->mHoveredImage = NULL;

	this->mEvent = NULL;
}
GUIArchButton::~GUIArchButton()
{
	SAFE_DELETE(this->mEvent);
}
GUIArchButton::GUIArchButton(float x, float y, float z, string textureName, float width, float height, GUIEvent* tempEvent, 
	string mTextureNamePressed, string mTextureNameHovered) : Element(x, y, z, textureName, width, height)
{
	this->mPressed = false;
	this->mHovered = false;

	this->mTextureNamePressed = mTextureNamePressed;
	this->mTextureNameHovered = mTextureNameHovered;
	
	this->mPressedImage = NULL;
	this->mHoveredImage = NULL;
	
	this->mEvent = tempEvent;
}

void GUIArchButton::ChangePicture(GraphicsEngine* ge)
{
	if(this->mPressed == true && this->mPressedImage == NULL)
	{
		float tempX, tempY, tempZ;
		this->GetPosition(tempX, tempY, tempZ);
		float width, height;
		this->GetWidth(width);
		this->GetHeight(height);
		if(this->mTextureNamePressed != "")
			this->mPressedImage = ge->CreateImage(D3DXVECTOR2(tempX, tempY), D3DXVECTOR2(width, height), this->mTextureNamePressed);
	}
	else if(this->mHovered == true && this->mHoveredImage == NULL)
	{
		float tempX, tempY, tempZ;
		this->GetPosition(tempX, tempY, tempZ);
		float width, height;
		this->GetWidth(width);
		this->GetHeight(height);
		if(this->mTextureNameHovered != "")
			this->mHoveredImage = ge->CreateImage(D3DXVECTOR2(tempX, tempY), D3DXVECTOR2(width, height), this->mTextureNameHovered);
	}
	if(this->mHovered == false && this->mHoveredImage != NULL)
	{
		ge->DeleteImage(this->mHoveredImage);
		this->mHoveredImage = NULL;
	}
	if(this->mPressed == false && this->mPressedImage != NULL)
	{
		ge->DeleteImage(this->mPressedImage);
		this->mPressedImage = NULL;
	}
}
bool GUIArchButton::RemoveFromRenderer(GraphicsEngine* ge)
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