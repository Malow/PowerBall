#include "DropDownList.h"

DropDownList::DropDownList() : Element()
{

}
DropDownList::DropDownList(Element **elements)
{
	
}
DropDownList::~DropDownList()
{

}

/*! Adds all normal stuff to the renderer*/
bool DropDownList::AddToRenderer(GraphicsEngine* ge)
{
	Element::AddToRenderer(ge);
	return true;
}

/*! Removes all normal stuff from the renderer*/
bool DropDownList::RemoveFromRenderer(GraphicsEngine* ge)
{
	Element::RemoveFromRenderer(ge);
	return true;
}
	
/*! Activat or Deactivate the List*/
bool DropDownList::Control(bool value)
{
	return true;
}