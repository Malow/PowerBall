#pragma once
/**
* Author: Rikard Johansson
* Create Date: 12/04/2012
* 
* A class that will handle drop down windows
**/
#include "Element.h"
#include "SimpleButton.h"


class DropDownList : public Element
{
private:
	float mActiveX;
	float mActiveY;

	float mActiveWidth;
	float mActiveHeight;

	bool mPressed;

	string mName;

	bool mDropActive;
	int mNrOfElements;
	int mMaxNrOfElements;
	Element **mElements;
	
	void AddListToRenderer(GraphicsEngine* ge);
	void RemoveListFromRenderer(GraphicsEngine* ge);
public:
	DropDownList();
	DropDownList(float x, float y, float z, string textureName, float width, float height, string name);
	virtual ~DropDownList();

	/*! Sets the position of active position*/
	void SetActivePos(float x, float y){ this->mActiveX = x; this->mActiveY = y; }
	/*! Saves the pos of active pos in parameters*/
	void GetActivePos(float& x, float& y){ x = this->mActiveX; y = this->mActiveY; }

	/*! Saves the actviation width of the button*/
	void SetActiveWidth(float activeWidth){ this->mActiveWidth = activeWidth; }
	/*! Returns the actvite width of the button*/
	void GetActiveWidth(float& width){ width = this->mActiveWidth; }

	/*! Saves the actviation height of the button*/
	void SetActiveHeight(float activeHeight){ this->mActiveHeight = activeHeight; }
	/*! Returns the actvite height of the button*/
	void GetActiveHeight(float& height){ height = this->mActiveHeight; }

	/*! Sets the state of mDropActive*/
	void SetDropActive(bool value){ this->mDropActive = value; }
	/*! Returns the active state of mDropActive*/
	bool GetDropActive(){ return this->mDropActive; }

	/*! Sets the name of the drop down list*/
	void SetName(string name){ this->mName = name; }
	/*! Gets the name of the drop down list*/
	string GetName(){ return this->mName; }

	/*! Add a button to the drop down menu*/
	bool AddButton(string textureName, GUIEvent* tempEvent,
		string mTextureNamePressed, string mTextureNameHovered);

	/*! Checks the collision*/
	GUIEvent* CheckCollision(float mouseX, float mouseY, bool mousePressed, GraphicsEngine* ge);

	GUIEvent* GetEventFromTop();

	/*! Adds all normal stuff to the renderer*/
	virtual bool AddToRenderer(GraphicsEngine* ge);

	/*! Removes all normal stuff from the renderer*/
	virtual bool RemoveFromRenderer(GraphicsEngine* ge);
};