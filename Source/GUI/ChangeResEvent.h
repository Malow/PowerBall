#pragma once
/**
* Author: Rikard Johansson
* Create Date: 04/04/2012
* This is a event that returns that nothing is going to happen.
**/
#include "GUIEvent.h"

class ChangeResEvent : public GUIEvent
{
private:
	int mWidth;
	int mHeight;
public:
	ChangeResEvent();
	ChangeResEvent(int width, int height);

	int GetWidth(){ return this->mWidth; }
	int GetHeight(){ return this->mHeight; }

	virtual ~ChangeResEvent();
};