#include "Event.h"

Event::Event()
{
	this->mEventMessage = ' ';
	this->mEventInfo = ' ';
}

Event::Event(char eventMessage, char eventInfo)
{
	this->mEventMessage = eventMessage;
	this->mEventInfo = eventInfo;
}

Event::~Event()
{
	
}