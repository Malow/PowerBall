#pragma once
#include "Element.h"


class SimpleButton : public Element
{
private:

public:
	SimpleButton();
	virtual ~SimpleButton();
	SimpleButton(float x, float y, float z, string textureName, float width, float height, Event* tempEvent);
};