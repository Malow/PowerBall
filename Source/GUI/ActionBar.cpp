#include "ActionBar.h"

ActionBar::ActionBar()
{
	this->mPosition = D3DXVECTOR2(-1.0f, -1.0f); 
	this->mIconSize = D3DXVECTOR2(0.0f, 0.0f);
	this->mIcons = new MaloW::Array<Image*>;
}
ActionBar::ActionBar(D3DXVECTOR2 position, D3DXVECTOR2 iconSize)
{
	this->mPosition = position; 
	this->mIconSize = iconSize;
	this->mIcons = new MaloW::Array<Image*>;
}
ActionBar::~ActionBar()
{
	if(this->mIcons)
	{
		while(this->mIcons->size() > 0)
		{
			delete this->mIcons->getAndRemove(0);
		}
	}
}

D3DXVECTOR2 ActionBar::GetPosition()
{
	return this->mPosition;
}
D3DXVECTOR2 ActionBar::GetCenterPosition()
{
	return D3DXVECTOR2(this->GetWidth() * 0.5f, this->GetHeight() * 0.5f);
}
float ActionBar::GetWidth()
{
	return this->mIconSize.x * this->mIcons->size();
}
float ActionBar::GetHeight()
{
	return this->mIconSize.y;
}

void ActionBar::SetPosition(D3DXVECTOR2 position)
{
	this->mPosition = position;
}
void ActionBar::SetCenterPosition(D3DXVECTOR2 position)
{
	this->mPosition = position + D3DXVECTOR2(-(this->GetWidth() * 0.5f), this->GetHeight() * 0.5f);
}

void ActionBar::AddIcon(Image* icon)
{
	this->mIcons->add(icon);
}