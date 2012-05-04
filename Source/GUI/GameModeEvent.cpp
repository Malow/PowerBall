#include "GameModeEvent.h"

GameModeEvent::GameModeEvent() : GUIEvent("GameModeEvent")
{
	this->mGameMode = NONE;
}
GameModeEvent::GameModeEvent(int GameMode) : GUIEvent("GameModeEvent")
{
	this->mGameMode = GameMode;
}
GameModeEvent::~GameModeEvent()
{

}