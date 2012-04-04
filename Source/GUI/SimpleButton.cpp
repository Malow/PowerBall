#include "SimpleButton.h"

SimpleButton::SimpleButton() : Element()
{

}
SimpleButton::~SimpleButton()
{

}
SimpleButton::SimpleButton(float x, float y, float z, string textureName, float width, float height, Event tempEvent) : Element(x, y, z, textureName, width, height, x, y, width, height, tempEvent)
{

}