#include "Event.h"

Event::Event()
{
	this->mEventMessage = ' ';
	this->mEventInfo = ' ';
}

Event::Event(string eventMessage, string eventInfo)
{
	this->mEventMessage = eventMessage;
	this->mEventInfo = eventInfo;
}

Event::~Event()
{
	
}