#pragma once
/**
* Author: Rikard Johansson
* Create Date: 28/03/2012
*
* This class will contain a number of buttons and
* other things.
**/

//includes
#include "Element.h"
#include "..\GraphicsEngine\GraphicsEngine.h"
#include "GUIArchButton.h"
#include "SimpleButton.h"
#include "DropDownList.h"

class GUISet
{
private:
	int mNrOfElements;
	int mMaxNrOfElements;
	Element** mElements;

	GUIEvent* UpdateButtons(float mouseX, float mouseY, bool mousePressed, GraphicsEngine* ge);

	GUIEvent* CheckCollision(float mouseX, float mouseY, bool mousePressed, GraphicsEngine* ge);
public:
	GUISet();
	virtual ~GUISet();

	/*! Adds a element to the set*/
	bool AddElement(Element* element);

	/*! Loads all elements into the renderer*/
	bool AddSetToRenderer(GraphicsEngine* ge);

	/*! Removes the set from the renderer*/
	bool RemoveSetFromRenderer(GraphicsEngine* ge);

	/*! Updates the hover and pressed images*/
	GUIEvent* UpdateAndCheckCollision(float mouseX, float mouseY, bool mousePressed, GraphicsEngine* ge);

};