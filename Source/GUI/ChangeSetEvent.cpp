#include "ChangeSetEvent.h"

ChangeSetEvent::ChangeSetEvent() : Event("ChangeSetEvent")
{

}
ChangeSetEvent::ChangeSetEvent(int set) : Event("ChangeSetEvent")
{
	this->mSet = set;
}
ChangeSetEvent::~ChangeSetEvent()
{

}