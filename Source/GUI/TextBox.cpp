#include "TextBox.h"

TextBox::TextBox() : Element()
{
	this->mActiveX = 0;
	this->mActiveY = 0;

	this->mActiveWidth = 0;
	this->mActiveHeight = 0;

	this->mText = "";
	this->mName = "";

	this->mFocused = false;
	this->mPressed = false;
	this->mPressedOutside = false;
	this->mTextSize = 0;
}
TextBox::TextBox(float x, float y, float z, string textureName, float width, float height, string text, string name, float textSize, int maxNrOfChars, int allowedKeys) : Element(x, y, z, textureName, width, height)
{
	this->mActiveX = x;
	this->mActiveY = y;

	this->mActiveWidth = width;
	this->mActiveHeight = height;

	this->mText = text;
	this->mName = name;
	
	this->mMaxNrOfChars = maxNrOfChars;
	this->mAllowedChars = allowedKeys;

	this->mFocused = false;
	this->mPressed = false;
	this->mPressedOutside = false;
	this->mTextSize = textSize;
}
TextBox::~TextBox()
{

}
bool TextBox::AddToRenderer(GraphicsEngine* ge)
{
	Element::AddToRenderer(ge);
	this->mPointText = ge->CreateText(this->mText, D3DXVECTOR2(this->GetPositionD3D().x + 3.0f, this->GetPositionD3D().y-0.5f), mTextSize, "Media/Fonts/1");

	return true;
}

 bool TextBox::RemoveFromRenderer(GraphicsEngine* ge)
{
	ge->DeleteText(this->mPointText);
	Element::RemoveFromRenderer(ge);
	this->mPointText = NULL;

	return true;
}

 void TextBox::GoingToBeFocused(float mouseX, float mouseY, bool mousePressed, GraphicsEngine* ge)
 {
	if((mouseX < this->mActiveX || mouseX > (this->mActiveX+this->mActiveWidth)) || (mouseY < this->mActiveY || mouseY > (this->mActiveY+this->mActiveHeight)))
	{
		if(mousePressed)
		{
			if(!this->mPressedOutside)
			{
				this->mPressedOutside = true;
			}

		}
		else if(!mousePressed)
		{
			if(this->mPressedOutside)
			{
				this->mPressedOutside = false;
				this->mFocused = false;
			}

		}
				return;
		return;
	}
	else
	{
		if(mousePressed)
		{
			if(!this->mPressed)
			{
				this->mPressed = true;
			}
		}
		else if(!mousePressed)
		{
			if(this->mPressed)
			{
				this->mPointText->DeleteFromEnd(this->mPointText->GetText().size());
				this->mPressed = false;
				this->mFocused = true;
				return;
			}
		}
	}
	return;
 }

 void TextBox::CheckString(GraphicsEngine* ge)
 {
	if(this->mPointText->GetText().size() < this->mMaxNrOfChars)
	{
		string pushString = "";
		const int NROFLETTERS = 27;
		const int NROFSPECIAL = 11;
		char keys[NROFLETTERS] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', ' '};
		char KEYS[NROFLETTERS] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', ' '};
		char specialChars[NROFSPECIAL] = {',', '.', ';', ':', '*', '?', '!', '<', '>', '-', '_'};
		char numbers[10] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0'};

		/*Check if a char is pressed*/
		if(this->mAllowedChars == ALL || this->mAllowedChars == NORMALCHAR || this->mAllowedChars == NORMALCHAR_NR ||
			this->mAllowedChars == NORMALCHAR_SPECIAL)
		{
			for(int i = 0; i < NROFLETTERS; i++)
			{
				if(ge->GetKeyListener()->IsPressed(KEYS[i]))
				{
					if(!ge->GetKeyListener()->IsPressed(VK_SHIFT))
						pushString = keys[i];
					else
						pushString = KEYS[i];

					this->mPointText->AppendText(pushString);
					ge->GetKeyListener()->KeyUp(KEYS[i]);
				}
			}
		}
		/*Check if a number is pressed*/
		if(this->mAllowedChars == ALL || this->mAllowedChars == NR || this->mAllowedChars == NORMALCHAR_NR ||
				this->mAllowedChars == NR_SPECIAL)
		{
			for(int i = 0; i < 10; i++)
			{
				if(ge->GetKeyListener()->IsPressed(numbers[i]))
				{
					pushString = numbers[i];

					this->mPointText->AppendText(pushString);
					ge->GetKeyListener()->KeyUp(numbers[i]);
				}
			}
		}
		/*Check with special characters*/
		if(this->mAllowedChars == ALL || this->mAllowedChars == SPECIAL || this->mAllowedChars == NORMALCHAR_SPECIAL ||
				this->mAllowedChars == NR_SPECIAL)
		{
			for(int i = 0; i < NROFSPECIAL; i++)
			{
				if(ge->GetKeyListener()->IsPressed(specialChars[i]))
				{
					pushString = specialChars[i];

					this->mPointText->AppendText(pushString);
					ge->GetKeyListener()->KeyUp(specialChars[i]);
				}
			}
		}

	}
	if(this->mPointText->GetText().size() > 0)
	{
		if(ge->GetKeyListener()->IsPressed(VK_BACK))
		{
			this->mPointText->DeleteFromEnd(1);
			ge->GetKeyListener()->KeyUp(VK_BACK);
		}
	}
 }
