#pragma once
#include "Element.h"


class SimpleButton : public Element
{
private:
	float mActiveX;
	float mActiveY;

	float mActiveWidth;
	float mActiveHeight;
	
	bool mPressed;
	bool mHovered;

	string mTextureNamePressed;
	string mTextureNameHovered;

	Image* mHoveredImage;
	Image* mPressedImage;

	GUIEvent* mEvent;
public:
	SimpleButton();
	virtual ~SimpleButton();
	SimpleButton(float x, float y, float z, string textureName, float width, float height, GUIEvent* tempEvent,
		string mTextureNamePressed, string mTextureNameHovered, float activeX, float activeY, float activeWidth, float activeHeight);


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

	/*! Sets the event of the element*/
	void SetEvent(GUIEvent* tempEvent){ this->mEvent = tempEvent; }
	/*! Gets the event from the element*/
	GUIEvent* GetEvent(){ return this->mEvent; }

	virtual bool RemoveFromRenderer(GraphicsEngine* ge);

	/*! Check if the mouse is inside the active area and returns an event if it is*/
	GUIEvent* CheckCollision(float mouseX, float mouseYm, bool mousePressed, GraphicsEngine* ge);
};