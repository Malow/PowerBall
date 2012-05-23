#pragma once
#include "Element.h"

class TextButton : public Element
{
private:
	float mActiveX;
	float mActiveY;

	float mActiveWidth;
	float mActiveHeight;

	bool mPressed;
	bool mHovered;

	string mText;

	GUIEvent* mEvent;
	Text* mTextPointer;
public:
	TextButton();
	TextButton(float x, float y, float z, string textureName, float width, float height, string text, GUIEvent* tempEvent);
	virtual ~TextButton();

	/*! Adds the element to the renderer*/
	virtual bool AddToRenderer(GraphicsEngine* ge);

	/*! Removes the element from the renderer*/
	virtual bool RemoveFromRenderer(GraphicsEngine* ge);

	/*! Check if the mouse is inside the active area and returns an event if it is*/
	GUIEvent* CheckCollision(float mouseX, float mouseY, bool mousePressed, GraphicsEngine* ge);
		
	/*! Gets the event from the element*/
	GUIEvent* GetEvent(){ return this->mEvent; }
};