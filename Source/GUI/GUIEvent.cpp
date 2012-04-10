#include "GUIEvent.h"

GUIEvent::GUIEvent()
{
	this->mEventMessage = "unknown";
}
GUIEvent::GUIEvent(string eventMessage)
{
	this->mEventMessage = eventMessage;
}
GUIEvent::~GUIEvent()
{
	
}