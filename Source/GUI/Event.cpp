#include "Event.h"

Event::Event()
{
	this->mEventMessage = "unknown";
}
Event::Event(string eventMessage)
{
	this->mEventMessage = eventMessage;
}
Event::~Event()
{
	
}