#include "CheckBox.h"

CheckBox::CheckBox() : Element()
{
	this->mActiveX = 0;
	this->mActiveY = 0;
	this->mActiveWidth = 0;
	this->mActiveHeight = 0;

	this->mEvent = NULL;

	this->mPressed = false;

	this->mName = "";

	this->mOn = false;
	this->mCheckedTextureName = "";
	this->mCheckedImage = NULL;
}
CheckBox::CheckBox(float x, float y, float z, string textureName, float width, float height, string CheckedTextureName, bool on, GUIEvent* tempEvent, string name) : Element(x, y, z, textureName, width, height)
{
	this->mActiveX = x;
	this->mActiveY = y;
	this->mActiveWidth = width;
	this->mActiveHeight = height;

	this->mName = name;

	this->mPressed = false;

	this->mEvent = tempEvent;

	this->mOn = on;
	this->mCheckedTextureName = CheckedTextureName;
	this->mCheckedImage = NULL;
}
CheckBox::~CheckBox()
{
	SAFE_DELETE(this->mEvent);
}
bool CheckBox::AddToRenderer(GraphicsEngine* ge)
{
	Element::AddToRenderer(ge);
	if(this->mOn && this->mCheckedImage == NULL)
	{
		D3DXVECTOR3 temp = this->GetPositionD3D();
		this->mCheckedImage = GetGraphicsEngine()->CreateImage(D3DXVECTOR2(temp.x, temp.y), this->GetDimension(), this->mCheckedTextureName);
	}
	return false;
}
bool CheckBox::RemoveFromRenderer(GraphicsEngine* ge)
{
	Element::RemoveFromRenderer(ge);
	if(this->mCheckedImage != NULL)
	{
		ge->DeleteImage(this->mCheckedImage);
		this->mCheckedImage = NULL;
	}
	return true;
}

GUIEvent* CheckBox::CheckCollision(float mouseX, float mouseY, bool mousePressed, GraphicsEngine* ge)
{
	GUIEvent* returnEvent;
	if(!((mouseX < this->mActiveX || mouseX > (this->mActiveX+this->mActiveWidth)) || (mouseY < this->mActiveY || mouseY > (this->mActiveY+this->mActiveHeight))))
	{
		if(!this->mPressed && mousePressed)
		{
			this->mPressed = true;
		}
		else if(mPressed && !mousePressed)
		{
			this->mPressed = false;
			if(this->mOn == false)
			{
				GameOptions::mouseClick->Play();
				this->mOn = true;
				D3DXVECTOR3 temp = this->GetPositionD3D();
				if(this->mCheckedImage == NULL)
					this->mCheckedImage = ge->CreateImage(D3DXVECTOR2(temp.x, temp.y), this->GetDimension(), this->mCheckedTextureName);

				ChangeOptionEvent* newTempEvent = (ChangeOptionEvent*)this->mEvent;
				newTempEvent->SetValue("true");

				return this->mEvent;
			}
			else if(this->mOn)
			{
				GameOptions::mouseClick->Play();
				this->mOn = false;
				if(this->mCheckedImage != NULL)
				{
					ge->DeleteImage(this->mCheckedImage);
					this->mCheckedImage = NULL;
				}
				ChangeOptionEvent* newTempEvent = (ChangeOptionEvent*)this->mEvent;
				newTempEvent->SetValue("false");

				return this->mEvent;
			}
		}
	}
	return NULL;

}
void CheckBox::SetChecked(bool checked)
{ 
	if(checked == this->mOn)
		return;
	else
	{
		this->mOn = checked;
		if(this->mOn && this->mCheckedImage == NULL)
		{
			this->mCheckedImage = GetGraphicsEngine()->CreateImage(this->GetPositionD3D(), this->GetDimension(), this->mCheckedTextureName);
		}
		else if(!this->mOn && this->mCheckedImage != NULL)
		{
			GetGraphicsEngine()->DeleteImage(this->mCheckedImage);
			this->mCheckedImage = NULL;
		}
	}
}
