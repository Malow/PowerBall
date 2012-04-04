#include "GUISet.h"


GUISet::GUISet()
{
	this->mNrOfElements = 0;
	this->mMaxNrOfElements = 20;
	this->mElements = new Element*[mMaxNrOfElements];
	for(int i = 0; i < this->mMaxNrOfElements; i++)
		this->mElements[i] = NULL;
}
GUISet::~GUISet()
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
bool GUISet::AddElement(Element* element)
{
	this->mElements[this->mNrOfElements] = element;
	this->mNrOfElements++;

	return true;
}

bool GUISet::AddSetToRenderer(GraphicsEngine* ge)
{
	for(int i = 0; i < this->mNrOfElements; i++)
		this->mElements[i]->AddToRenderer(ge);
	
	return true;
}

bool GUISet::RemoveSetFromRenderer(GraphicsEngine* ge)
{
	for(int i = 0; i < this->mNrOfElements; i++)
		this->mElements[i]->RemoveFromRenderer(ge);

	return true;
}
Event* GUISet::CheckCollision(float mouseX, float mouseY)
{
	Event* returnEvent = NULL; // = NoEvent();
	Event* tempReturnEvent;
	for(int i = 0; i < this->mNrOfElements; i++)
	{
		tempReturnEvent = this->mElements[i]->CheckCollision(mouseX, mouseY);
		if(tempReturnEvent != NULL)
		{
			if(tempReturnEvent->GetEventMessage() != "NoEvent")
			{
				returnEvent = tempReturnEvent;
				break;
			}
		}
	}

	return returnEvent;
}