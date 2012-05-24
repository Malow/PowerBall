#pragma once

#include "GUIEvent.h"

class ServerListEvent : public GUIEvent
{
private:
	string message;
public:
	ServerListEvent();
	ServerListEvent(string message);
	virtual ~ServerListEvent();

	string GetEventMessage(){ return this->message; }
};