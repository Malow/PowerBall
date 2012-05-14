#pragma once

#include "Element.h"
#include "..\GraphicsEngine\GraphicsEngine.h"
#include "..\stdafx.h"
enum TEXTBOX {NOTHINGALLOWED,
	ALL,
	NORMALCHAR,
	NORMALCHAR_NR,
	NORMALCHAR_SPECIAL,
	NR,
	NR_SPECIAL,
	SPECIAL};
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

	float mTextSize;
	int mMaxNrOfChars;
	int mAllowedChars;
	int mFrom;
	int mTo;

	string mText;
	string mName;

	Text* mPointText;
public:
	TextBox();
	TextBox(float x, float y, float z, string textureName, float width, float height, string text, string name, float textSize, int maxNrOfChars, int allowedKeys);
	TextBox(float x, float y, float z, string textureName, float width, float height, string text, string name, float textSize, int maxNrOfChars, int allowedKeys, int from, int to);
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