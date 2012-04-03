#pragma once
#include "Element.h"


class SimpleButton : public Element
{
private:
	float mActiveX;
	float mActiveY;
	float mActiveZ;

	int mActiveWidth;
	int mActiveHeight;
public:
	SimpleButton();
	SimpleButton(float x, float y, float z, char textureName, int width, int height);
};