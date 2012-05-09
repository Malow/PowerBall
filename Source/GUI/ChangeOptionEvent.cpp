#include "ChangeOptionEvent.h"
ChangeOptionEvent::ChangeOptionEvent() : GUIEvent("ChangeOptionEvent")
{

}
ChangeOptionEvent::ChangeOptionEvent(string option, string value) : GUIEvent("ChangeOptionEvent")
{
	this->mOption = option;
	this->mValue = value;
}
ChangeOptionEvent::~ChangeOptionEvent()
{

}