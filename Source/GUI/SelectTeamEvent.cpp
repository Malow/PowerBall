#include "SelectTeamEvent.h"

SelectTeamEvent::SelectTeamEvent(int team) : GUIEvent("SelectTeamEvent")
{
	this->mTeam = team;
}
SelectTeamEvent::~SelectTeamEvent()
{

}