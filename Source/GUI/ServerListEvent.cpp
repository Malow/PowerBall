#include "ServerListEvent.h"

ServerListEvent::ServerListEvent() : GUIEvent("ServerListEvent")
{

}
ServerListEvent::ServerListEvent(string message) : GUIEvent("ServerListEvent")
{
	this->message = message;
}
ServerListEvent::~ServerListEvent()
{

}