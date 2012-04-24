#pragma once
/**
* Author: Rikard Johansson
* Create Date: 04/04/2012
* This is a event that returns that nothing is going to happen.
**/
#include "GUIEvent.h"

class NoEvent : public GUIEvent
{
private:

public:
	NoEvent();
	virtual ~NoEvent();
};