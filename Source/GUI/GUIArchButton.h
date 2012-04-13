#pragma once

#include "Element.h"

class GUIArchButton : public Element
{
private:
	bool mPressed;
	bool mHovered;

	string mTextureNamePressed;
	string mTextureNameHovered;

	Image* mHoveredImage;
	Image* mPressedImage;

	GUIEvent* mEvent;
public:
	GUIArchButton();
	virtual ~GUIArchButton();
	GUIArchButton(float x, float y, float z, string textureName, float width, float height, GUIEvent* tempEvent, 
		string mTextureNamePressed, string mTextureNameHovered);

	void SetHovered(bool value){ this->mHovered = value; }
	bool GetHovered(){ return this->mHovered; }

	void SetPressed(bool value){ this->mPressed = value; }
	bool GetPressed(){ return this->mPressed; }

	/*! Sets the event of the element*/
	void SetEvent(GUIEvent* tempEvent){ this->mEvent = tempEvent; }
	/*! Gets the event from the element*/
	GUIEvent* GetEvent(){ return this->mEvent; }

	void ChangePicture(GraphicsEngine* ge);
	
	virtual bool RemoveFromRenderer(GraphicsEngine* ge);
};