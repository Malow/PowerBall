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
	{
		if(typeid(*this->mElements[i]) == typeid(GUIPicture))
		{
			GUIPicture* temp = (GUIPicture*)this->mElements[i];
			temp->RemoveAllFromRenderer(ge);
		}
	}

	return true;
}
GUIEvent* GUISet::UpdateButtons(GraphicsEngine* ge, bool mousePressed)
{
	GUIEvent* returnEvent = NULL;
	D3DXVECTOR2 mousePos = ge->GetKeyListener()->GetMousePosition();
	float windowWidth = (float)ge->GetEngineParameters().windowWidth;
	float windowHeight = (float)ge->GetEngineParameters().windowHeight;
	GUIPicture* temp = NULL;

	float dx = (windowHeight * 4.0f) / 3.0f;
	float offSet = (windowWidth - dx) / 2.0f;
	/*If options*/
	if(mousePos.x < ((dx * (494.0f / 1440))+(offSet)))
	{
		for(int i = 1; i < this->mNrOfElements; i++)
		{
			if(i == 4)
			{
				if(!mousePressed)
				{
					temp = (GUIPicture*)this->mElements[4];

					if(temp->GetPressed())
						returnEvent = this->mElements[4]->GetEvent();
					temp->SetHovered(true);
					temp->SetPressed(false);
					temp->ChangePicture(ge);
				}
				else if(mousePressed)
				{
					temp = (GUIPicture*)this->mElements[4];

					temp->SetHovered(false);
					temp->SetPressed(true);
					temp->ChangePicture(ge);
				}
			}
			else
			{
				temp = (GUIPicture*)this->mElements[i];

				temp->SetHovered(false);
				temp->SetPressed(false);
				temp->ChangePicture(ge);
			}
		}
	}
	/*If Credit*/
	else if(mousePos.x > ((dx * (945.0f / 1440))+(offSet)))
	{
		for(int i = 1; i < this->mNrOfElements; i++)
		{
			if(i == 2)
			{
				if(!mousePressed)
				{
					temp = (GUIPicture*)this->mElements[2];

					if(temp->GetPressed())
						returnEvent = this->mElements[2]->GetEvent();
					temp->SetHovered(true);
					temp->SetPressed(false);
					temp->ChangePicture(ge);
				}
				else if(mousePressed)
				{
					temp = (GUIPicture*)this->mElements[2];

					temp->SetHovered(false);
					temp->SetPressed(true);
					temp->ChangePicture(ge);
				}
			}
			else
			{
				temp = (GUIPicture*)this->mElements[i];

				temp->SetHovered(false);
				temp->SetPressed(false);
				temp->ChangePicture(ge);
			}
		}
	}
	/*If Play*/
	else if(mousePos.y < (windowHeight * (397.0f / 1080)))
	{
		for(int i = 1; i < this->mNrOfElements; i++)
		{
			if(i == 1)
			{
				if(!mousePressed)
				{
					temp = (GUIPicture*)this->mElements[1];

					if(temp->GetPressed())
						returnEvent = this->mElements[1]->GetEvent();
					temp->SetHovered(true);
					temp->SetPressed(false);
					temp->ChangePicture(ge);
				}
				else if(mousePressed)
				{
					temp = (GUIPicture*)this->mElements[1];

					temp->SetHovered(false);
					temp->SetPressed(true);
					temp->ChangePicture(ge);
				}
			}
			else
			{
				temp = (GUIPicture*)this->mElements[i];

				temp->SetHovered(false);
				temp->SetPressed(false);
				temp->ChangePicture(ge);
			}
		}
	}
	/*If Exit*/
	else if(mousePos.y > (windowHeight * (681.0f / 1080)))
	{
		for(int i = 1; i < this->mNrOfElements; i++)
		{
			if(i == 3)
			{
				if(!mousePressed)
				{
					temp = (GUIPicture*)this->mElements[3];

					if(temp->GetPressed())
						returnEvent = this->mElements[3]->GetEvent();
					temp->SetHovered(true);
					temp->SetPressed(false);
					temp->ChangePicture(ge);
				}
				else if(mousePressed)
				{
					temp = (GUIPicture*)this->mElements[3];

					temp->SetHovered(false);
					temp->SetPressed(true);
					temp->ChangePicture(ge);
				}
			}
			else
			{
				temp = (GUIPicture*)this->mElements[i];

				temp->SetHovered(false);
				temp->SetPressed(false);
				temp->ChangePicture(ge);
			}
		}
	}
	/*Safe Side*/
	else
	{
		for(int i = 1; i < this->mNrOfElements; i++)
		{
			temp = (GUIPicture*)this->mElements[i];

			temp->SetHovered(false);
			temp->ChangePicture(ge);
		}
	}

	return returnEvent;
}

GUIEvent* GUISet::CheckCollision(float mouseX, float mouseY)
{
	GUIEvent* returnEvent = NULL; // = NoEvent();
	GUIEvent* tempReturnEvent;
	for(int i = 0; i < this->mNrOfElements; i++)
	{
		if(typeid(*this->mElements[i]) != typeid(GUIPicture))
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
	}

	return returnEvent;
}