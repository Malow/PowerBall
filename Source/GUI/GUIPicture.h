#pragma once

#include "Element.h"

class GUIPicture : public Element
{
private:

public:
	GUIPicture();
	GUIPicture(float x, float y, float z, char textureName, int width, int height);
};