#include "DropDownList.h"

DropDownList::DropDownList() : Element()
{
	this->mNrOfElements = 0;
	this->mMaxNrOfElements = 10;
	this->mElements = new Element*[mMaxNrOfElements];
}
DropDownList::DropDownList(float x, float y, float z, string textureName, float width, float height) 
	: Element(x, y, z, textureName, width, height)
{
	// temp for testing
	this->mActiveX = x+50;
	this->mActiveY = y+30;
	this->mActiveHeight = 24;
	this->mActiveWidth = 24;
	//////
	this->mPressed = false;
	this->mDropActive = false;

	this->mMaxNrOfElements = 10;
	this->mNrOfElements = 0;
	this->mElements = new Element*[mMaxNrOfElements];
	
}
DropDownList::~DropDownList()
{
	if(this->mElements)
	{
		for(int i = 0; i < this->mNrOfElements; i++)
		{
			if(this->mElements[i])
				delete this->mElements[i];
		}
		delete [] this->mElements;
	}
}

/*! Adds all normal stuff to the renderer*/
bool DropDownList::AddToRenderer(GraphicsEngine* ge)
{
	Element::AddToRenderer(ge);
	if(this->mElements[0])
		this->mElements[0]->AddToRenderer(ge);
	return true;
}

/*! Removes all normal stuff from the renderer*/
bool DropDownList::RemoveFromRenderer(GraphicsEngine* ge)
{
	this->mPressed = false;
	this->mDropActive = false;
	if(this->mElements[0])
		this->mElements[0]->RemoveFromRenderer(ge);
	this->RemoveListFromRenderer(ge);
	Element::RemoveFromRenderer(ge);
	return true;
}

/*! Checks the collision*/
GUIEvent* DropDownList::CheckCollision(float mouseX, float mouseY, bool mousePressed, GraphicsEngine* ge)
{
	GUIEvent* returnEvent;
	if(!((mouseX < this->mActiveX || mouseX > (this->mActiveX+this->mActiveWidth)) || (mouseY < this->mActiveY || mouseY > (this->mActiveY+this->mActiveHeight))))
	{
		if(!this->mPressed && mousePressed)
			this->mPressed = true;
		else if(mPressed && !mousePressed)
		{
			this->mPressed = false;
			if(!this->mDropActive)
			{
				this->mDropActive = true;
				this->AddListToRenderer(ge);
			}
			else if(this->mDropActive)
			{
				this->mDropActive = false;
				this->RemoveListFromRenderer(ge);
			}
			for(int i = 0; i < this->mNrOfElements; i++)
			{
				if(typeid(*this->mElements[i]) == typeid(SimpleButton))
				{
					SimpleButton* temp = (SimpleButton*)this->mElements[i];
					returnEvent = temp->CheckCollision(mouseX, mouseY, mousePressed, ge);
					if(returnEvent != NULL)
					{
						/*Change positions here*/
						return returnEvent;
					}
				}
			}
		}
	}
	if(this->mDropActive)
	{
		for(int i = 0; i < this->mNrOfElements; i++)
		{
			if(typeid(*this->mElements[i]) == typeid(SimpleButton))
			{
				SimpleButton* temp = (SimpleButton*)this->mElements[i];
				returnEvent = temp->CheckCollision(mouseX, mouseY, mousePressed, ge);
				if(returnEvent != NULL)
				{
					/*Change positions here*/
					return returnEvent;
				}

			}
		}
	}
	return NULL;
}
void DropDownList::AddListToRenderer(GraphicsEngine* ge)
{
	for(int i = 1; i < this->mNrOfElements; i++)
	{
		this->mElements[i]->AddToRenderer(ge);
	}
}
void DropDownList::RemoveListFromRenderer(GraphicsEngine* ge)
{
	for(int i = 1; i < this->mNrOfElements; i++)
	{
		this->mElements[i]->RemoveFromRenderer(ge);
	}
}

bool DropDownList::AddButton(float x, float y, float z, string textureName, float width, float height, GUIEvent* tempEvent,
		string mTextureNamePressed, string mTextureNameHovered, float activeX, float activeY, float activeWidth, float activeHeight)
{
	if(this->mNrOfElements+1 < this->mMaxNrOfElements)
	{
		this->mElements[this->mNrOfElements] = new SimpleButton(x, y, z, textureName, width, height, tempEvent,
			mTextureNamePressed, mTextureNameHovered, activeX, activeY, activeWidth, activeHeight);
	
		this->mNrOfElements++;

		return true;
	}
	else
		return false;
}