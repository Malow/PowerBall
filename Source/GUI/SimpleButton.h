#pragma once
#include "Element.h"


class SimpleButton : public Element
{
private:
	float mActiveX;
	float mActiveY;
	float mActiveZ;

	float mActiveWidth;
	float mActiveHeight;
public:
	SimpleButton();
	virtual ~SimpleButton();
	SimpleButton(float x, float y, float z, string textureName, float width, float height, Event tempEvent);
};