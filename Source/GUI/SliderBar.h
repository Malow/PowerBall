#pragma once

/**
* Author: Rikard Johansson
* Create Date: 28/03/2012
*
* A silder bar
**/
#include "Element.h"
#include "..\GraphicsEngine\GraphicsEngine.h"
#include "..\stdafx.h"

class SliderBar : public Element
{
private:
	float mSliderPosition; // value between 0-100
	D3DXVECTOR2 mSliderMin; // position + 10% of position
	D3DXVECTOR2 mSliderMax; // position + 90% of position

	int mActiveWidth;
	int mActiveHeight;
	float mActiveX;
	float mActiveY;

	string mSliderTexPath;
	Image* mSlider;
public:
	SliderBar();
	SliderBar(float x, float y, float z, string textureName, float width, float height, string sliderTexPath, float SliderPos, int activeWidth, int activeHeight);
	virtual ~SliderBar();

	/*! Adds the element to the renderer*/
	virtual bool AddToRenderer(GraphicsEngine* ge);
	/*! Removes the element from the renderer*/
	virtual bool RemoveFromRenderer(GraphicsEngine* ge);

	void CheckCollision(float mouseX, float mouseY, bool mousePressed, GraphicsEngine* ge);
};