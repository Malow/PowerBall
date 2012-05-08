#include "DropDownList.h"

DropDownList::DropDownList() : Element()
{
	this->mNrOfElements = 0;
	this->mMaxNrOfElements = 10;
	this->mElements = new Element*[mMaxNrOfElements];
}
DropDownList::DropDownList(float x, float y, float z, string textureName, float width, float height, string name) 
	: Element(x, y, z, textureName, width, height)
{
	this->mMouseClick = GetGraphicsEngine()->GetSoundEngine()->LoadSoundEffect("Media/Sounds/SoundEffects/Mouse_Click_Menu.mp3");

	this->mActiveX = x;
	this->mActiveY = y;
	this->mActiveHeight = 24;
	this->mActiveWidth = 24;
	
	this->mPressed = false;
	this->mDropActive = false;

	this->mName = name;
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
				this->mMouseClick->Play();
				this->mDropActive = true;
				this->AddListToRenderer(ge);
			}
			else if(this->mDropActive)
			{
				this->mMouseClick->Play();
				this->mDropActive = false;
				this->RemoveListFromRenderer(ge);
			}
			for(int i = 1; i < this->mNrOfElements; i++)
			{
				if(typeid(*this->mElements[i]) == typeid(SimpleButton))
				{
					SimpleButton* temp = (SimpleButton*)this->mElements[i];
					returnEvent = temp->CheckCollision(mouseX, mouseY, mousePressed, ge);
					if(returnEvent != NULL)
					{
						D3DXVECTOR2 firstElementPos = this->mElements[0]->GetImage()->GetPosition();
						D3DXVECTOR2 newElementPos = this->mElements[i]->GetImage()->GetPosition();
						this->mElements[0]->SetPositionD3D(newElementPos);
						this->mElements[i]->SetPositionD3D(firstElementPos);
						Element* temp = this->mElements[0];
						this->mElements[0] = this->mElements[i];
						this->mElements[i] = temp;
						return returnEvent;
					}
				}
			}
		}
	}
	if(this->mDropActive)
	{
		for(int i = 1; i < this->mNrOfElements; i++)
		{
			if(typeid(*this->mElements[i]) == typeid(SimpleButton))
			{
				SimpleButton* temp = (SimpleButton*)this->mElements[i];
				returnEvent = temp->CheckCollision(mouseX, mouseY, mousePressed, ge);
				if(returnEvent != NULL)
				{
					/*if(i != 0)
					{
						D3DXVECTOR2 firstElementPos = this->mElements[0]->GetImage()->GetPosition();
						D3DXVECTOR2 newElementPos = this->mElements[i]->GetImage()->GetPosition();
						this->mElements[0]->SetPositionD3D(newElementPos);
						this->mElements[i]->SetPositionD3D(firstElementPos);
						SimpleButton* tempFirst = (SimpleButton*)this->mElements[0];
						D3DXVECTOR2 firstElementActivePos = tempFirst->GetActivePos();
						D3DXVECTOR2 newElementActivePos = temp->GetActivePos();
						tempFirst->SetActivePos(newElementActivePos);
						temp->SetActivePos(firstElementActivePos);
						Element* temp1 = this->mElements[0];
						this->mElements[0] = this->mElements[i];
						this->mElements[i] = temp1;
					}*/
					this->mDropActive = false;
					this->RemoveFromRenderer(ge);
					this->AddToRenderer(ge);
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

bool DropDownList::AddButton(string textureName, GUIEvent* tempEvent,
		string mTextureNamePressed, string mTextureNameHovered)
{
	if(this->mNrOfElements+1 < this->mMaxNrOfElements)
	{
		float x, y, z;
		float width, height;
		this->GetWidth(width);
		this->GetHeight(height);
		this->GetPosition(x, y, z);
		this->mElements[this->mNrOfElements] = new SimpleButton(x + this->mActiveWidth, y + (height * this->mNrOfElements), 1, textureName, width-this->mActiveWidth, height, tempEvent,
			mTextureNamePressed, mTextureNameHovered, x + this->mActiveWidth, y + (height * this->mNrOfElements), width-this->mActiveWidth, height);
	
		this->mNrOfElements++;

		return true;
	}
	else
		return false;
}

GUIEvent* DropDownList::GetEventFromTop()
{
	if(this->mNrOfElements > 0)
	{
		SimpleButton* temp = (SimpleButton*) this->mElements[0];
		return temp->GetEvent();
	}
	return NULL;
}