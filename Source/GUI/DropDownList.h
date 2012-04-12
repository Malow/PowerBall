#pragma once
/**
* Auther: Rikard Johansson
* Create Date: 12/04/2012
* 
* A class that will handle drop down windows
**/
#include "Element.h"


class DropDownList : public Element
{
private:
	bool mDropActive;
	Element **mElements;
	
public:
	DropDownList();
	DropDownList(Element **elements);
	virtual ~DropDownList();
	/*! Sets the state of mDropActive*/
	void SetDropActive(bool value){ this->mDropActive = value; }
	/*! Returns the active state of mDropActive*/
	bool GetDropActive(){ return this->mDropActive; }

	/*! Adds all normal stuff to the renderer*/
	virtual bool AddToRenderer(GraphicsEngine* ge);

	/*! Removes all normal stuff from the renderer*/
	virtual bool RemoveFromRenderer(GraphicsEngine* ge);
	
	/*! Activat or Deactivate the List*/
	bool Control(bool value);
};