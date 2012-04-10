#pragma once
/**
* Auther: Rikard Johansson
* Create Date: 04/04/2012
*
* This event will be returned when you are going into a new menu
**/

#include "GUIEvent.h"

class ChangeSetEvent : public GUIEvent
{
private:
	int mSet;
public:
	ChangeSetEvent();
	ChangeSetEvent(int set);
	virtual ~ChangeSetEvent();

	/*! Changes the value that explane what set that is going to be active*/
	void SetSet(int set){ this->mSet = set; }
	/*! Returns the Set value*/
	void GetSet(int& set){ set = this->mSet; }
};