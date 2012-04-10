#include "ChangeSetEvent.h"

ChangeSetEvent::ChangeSetEvent() : GUIEvent("ChangeSetEvent")
{

}
ChangeSetEvent::ChangeSetEvent(int set) : GUIEvent("ChangeSetEvent")
{
	this->mSet = set;
}
ChangeSetEvent::~ChangeSetEvent()
{

}