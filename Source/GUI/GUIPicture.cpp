#include "GUIPicture.h"

GUIPicture::GUIPicture() : Element()
{

}
GUIPicture::~GUIPicture()
{

}
GUIPicture::GUIPicture(float x, float y, float z, string textureName, float width, float height, Event* tempEvent) : Element(x, y, z, textureName, width, height, 0, 0, 0, 0, tempEvent)
{

}