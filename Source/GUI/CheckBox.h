#pragma once
/*
* Author: Rikard Johansson
* Create Date: 09/05/2012
*
* A box that have 2 states. On or Off. Starting off.
*/
#include "Element.h"


class CheckBox : public Element
{
private:
	float mActiveX;
	float mActiveY;
	float mActiveWidth;
	float mActiveHeight;

	bool mPressed;
	bool mOn;

	string mCheckedTextureName;
	string mName;

	GUIEvent* mEvent;
	SoundEffect* mMouseClick;
	Image* mCheckedImage;
public:
	CheckBox();
	CheckBox(float x, float y, float z, string textureName, float width, float height, string CheckedTextureName, bool on, GUIEvent* tempEvent, string name);
	virtual ~CheckBox();

	GUIEvent* GetEvent(){ return this->mEvent; }

	/*! Adds the element to the renderer*/
	virtual bool AddToRenderer(GraphicsEngine* ge);
	/*! Removes the element from the renderer*/
	virtual bool RemoveFromRenderer(GraphicsEngine* ge);

	string GetName(){ return this->mName; }

	bool GetOn(){ return this->mOn; }

	GUIEvent* CheckCollision(float mouseX, float mouseY, bool mousePressed, GraphicsEngine* ge);

};