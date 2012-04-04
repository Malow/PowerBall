#pragma once
/**
* Auther: Rikard Johansson
* Create Date: 04/04/2012
* This is a event that returns that nothing is going to happen.
**/
#include "Event.h"

class NoEvent : public Event
{
private:

public:
	NoEvent();
	virtual ~NoEvent();
};