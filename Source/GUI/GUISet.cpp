#include "GUISet.h"


GUISet::GUISet()
{
	this->mNrOfElements = 0;
	this->mElements = new Element[20]();
}
GUISet::~GUISet()
{
	delete [] this->mElements;
}
bool GUISet::AddElement(Element element)
{
	this->mElements[this->mNrOfElements] = element;
	this->mNrOfElements++;

	return true;
}

bool GUISet::AddSetToRenderer(GraphicsEngine* ge)
{
	for(int i = 0; i < this->mNrOfElements; i++)
		this->mElements[i].AddToRenderer(ge);

	return true;
}

bool GUISet::RemoveSetFromRenderer(GraphicsEngine* ge)
{
	for(int i = 0; i < this->mNrOfElements; i++)
		this->mElements[i].RemoveFromRenderer(ge);

	return true;
}
Event* GUISet::CheckCollision(float mouseX, float mouseY)
{
	Event* returnEvent = new NoEvent(); // = NoEvent();
	Event* tempReturnEvent;
	for(int i = 0; i < this->mNrOfElements; i++)
	{
		tempReturnEvent = this->mElements[i].CheckCollision(mouseX, mouseY);
		if(tempReturnEvent->GetEventMessage() != "NoEvent")
		{
			returnEvent = tempReturnEvent;
			break;
		}
	}

	return returnEvent;
}