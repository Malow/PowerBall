#pragma once

#include "Element.h"

class GUIPicture : public Element
{
private:

public:
	GUIPicture();
	virtual ~GUIPicture();
	GUIPicture(float x, float y, float z, string textureName, float width, float height, Event tempEvent);
};