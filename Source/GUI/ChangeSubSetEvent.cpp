#include "ChangeSubSetEvent.h"

ChangeSubSetEvent::ChangeSubSetEvent() : GUIEvent("ChangeSubSetEvent")
{

}
ChangeSubSetEvent::ChangeSubSetEvent(int set, int GameMode) : GUIEvent("ChangeSubSetEvent")
{
	this->mSet = set;
	this->mGameMode = GameMode;
}
ChangeSubSetEvent::~ChangeSubSetEvent()
{

}