#pragma once

#include "Element.h"
#include "..\GraphicsEngine\GraphicsEngine.h"
#include "..\stdafx.h"

class TextBox : public Element
{
private:
	float mActiveX;
	float mActiveY;

	float mActiveWidth;
	float mActiveHeight;
	
	bool mFocused;
	bool mPressed;
	bool mPressedOutside;

	int mTextSize;

	string mText;
	string mName;

	Text* mPointText;
public:
	TextBox();
	TextBox(float x, float y, float z, string textureName, float width, float height, string text, string name, float textSize);
	virtual ~TextBox();

	/*! Adds the element to the renderer*/
	virtual bool AddToRenderer(GraphicsEngine* ge);
	/*! Removes the element from the renderer*/
	virtual bool RemoveFromRenderer(GraphicsEngine* ge);

	void GoingToBeFocused(float mouseX, float mouseY, bool mousePressed, GraphicsEngine* ge);

	bool GetFocused(){ return this->mFocused; }

	string GetName(){ return this->mName; }

	string GetText(){ return this->mPointText->GetText(); }

	void CheckString(GraphicsEngine* ge);
};