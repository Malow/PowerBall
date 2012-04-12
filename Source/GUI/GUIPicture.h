#pragma once

#include "Element.h"

class GUIPicture : public Element
{
private:
	bool mPressed;
	bool mHovered;

	string mTextureNamePressed;
	string mTextureNameHovered;

	Image* mHoveredImage;
	Image* mPressedImage;
public:
	GUIPicture();
	virtual ~GUIPicture();
	GUIPicture(float x, float y, float z, string textureName, float width, float height, GUIEvent* tempEvent, 
		string mTextureNamePressed, string mTextureNameHovered);

	void SetHovered(bool value){ this->mHovered = value; }
	bool GetHovered(){ return this->mHovered; }

	void SetPressed(bool value){ this->mPressed = value; }
	bool GetPressed(){ return this->mPressed; }

	void ChangePicture(GraphicsEngine* ge);

	virtual bool RemoveFromRenderer(GraphicsEngine* ge);
};