#include "ChangeResEvent.h"

ChangeResEvent::ChangeResEvent() : GUIEvent("ChangeResEvent")
{

}
ChangeResEvent::ChangeResEvent(int width, int height) : GUIEvent("ChangeResEvent")
{
	this->mHeight = height;
	this->mWidth = width;
}
ChangeResEvent::~ChangeResEvent()
{

}