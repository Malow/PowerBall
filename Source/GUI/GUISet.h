#pragma once
/**
* Auther: Rikard Johansson
* Create Date: 28/03/2012
*
* This class will contain a number of buttons and
* other things.
**/

//includes
#include "Element.h"
#include "..\GraphicsEngine\GraphicsEngine.h"
#include "GUIPicture.h"
#include "SimpleButton.h"

class GUISet
{
private:
	int mNrOfElements;
	Element* mElements;
public:
	GUISet();
	virtual ~GUISet();

	/*! Adds a element to the set*/
	bool AddElement(Element element);

	/*! Loads all elements into the renderer*/
	bool AddSetToRenderer(GraphicsEngine* ge);

	/*! Removes the set from the renderer*/
	bool RemoveSetFromRenderer(GraphicsEngine* ge);

	Event* CheckCollision(float mouseX, float mouseY);

};