#include "GUISet.h"


GUISet::GUISet()
{
	this->mNrOfElements = 0;
	this->mElements = new Element[10]();
}
GUISet::~GUISet()
{

}
bool GUISet::AddElement(Element element)
{
	this->mElements[this->mNrOfElements++] = element;

	return true;
}

bool GUISet::AddSetToRenderer()
{
	for(int i = 0; i < this->mNrOfElements; i++)
		this->mElements[i].AddToRenderer();

	return true;
}

bool GUISet::RemoveSetFromRenderer()
{
	for(int i = 0; i < this->mNrOfElements; i++)
		this->mElements[i].RemoveFromRenderer();

	return true;
}