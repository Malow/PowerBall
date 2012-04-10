#pragma once
/**
* Auther: Rikard Johansson
* Create Date: 28/03/2012
* 
* This abstract class will be used as a base of every GUI element
**/

#include "..\GraphicsEngine\GraphicsEngine.h"
#include "..\GraphicsEngine\Image.h"
#include <string>
#include "Event.h"
#include "NoEvent.h"
#include "ChangeSetEvent.h"

using namespace std;

class Element
{
private:
	float mX;
	float mY;
	float mZ;
	
	float mWidth;
	float mHeight;

	float mActiveX;
	float mActiveY;

	float mActiveWidth;
	float mActiveHeight;

	string mTextureName;

	Image* mImage;
	GUIEvent* mEvent;

public:
	Element();
	Element(float x, float y, float z, string textureName, float width, float height, float activeX, float activeY, float activeWidth, float activeHeight, GUIEvent* tempEvent);
	/*Element(Element& origObj);*/
	virtual ~Element();

	/*! Sets the position of the element*/
	void SetPosition(float x, float y, float z){ this->mX = x; this->mY = y;this->mZ = z; }
	/*! Saves the position of the element in the parameters*/
	void GetPosition(float& x, float& y, float& z){ x = this->mX; y = this->mY; z = this->mZ; }

	/*! Sets the width of the element*/
	void SetWidth(float width){ this->mWidth = width; }
	/*! Saves the width of the element in parameter*/
	void GetWidth(float& width){ width = this->mWidth; }

	/*! Set the height of the element*/
	void SetHeight(float height){ this->mHeight = height; }
	/*! Saves the height of the element in parameter*/
	void GetHeight(float& height){ height = this->mHeight; }

	/*! Sets the position of active position*/
	void SetPosition(float x, float y){ this->mActiveX = x; this->mActiveY = y; }
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

	/*! Sets the event of the element*/
	void SetEvent(GUIEvent* tempEvent){ this->mEvent = tempEvent; }
	/*! Gets the event from the element*/
	void GetEvent(GUIEvent* tempEvent){ tempEvent = this->mEvent; }

	/*! Sets the texture variable*/
	void SetTextureName(string textureName){ this->mTextureName = textureName; }
	/*! Saves the texture name in parameter*/
	void GetTextureName(string& textureName){ textureName = this->mTextureName; }
	
	/*! Sets the image*/
	void SetImage(Image* image){ this->mImage = image; }
	/*! Saves the image in the parameter*/
	void GetImage(Image* image){ image = this->mImage; }

	/*! Adds the element to the renderer*/
	bool AddToRenderer(GraphicsEngine* ge);
	/*! Removes the element from the renderer*/
	bool RemoveFromRenderer(GraphicsEngine* ge);
	
	/*! Check if the mouse is inside the active area and returns an event if it is*/
	GUIEvent* CheckCollision(float mouseX, float mouseY);
};