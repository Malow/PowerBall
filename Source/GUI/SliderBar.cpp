#include "SliderBar.h"

SliderBar::SliderBar()
{

}
SliderBar::SliderBar(float x, float y, float z, string textureName, float width, float height, string sliderTexPath, float SliderPos, int activeWidth, int activeHeight)
{
	this->mSliderPosition = SliderPos;
	this->mSliderMin = D3DXVECTOR2(x + width*0.10, y + height/4); //Maybe some other calc here later
	this->mSliderMax = D3DXVECTOR2(x + width*0.90, y + height/4);
	this->mSlider = NULL;
	this->mSliderTexPath = sliderTexPath;
	this->mActiveX = this->GetPositionD3D().x + width * 0.10 + ((this->mSliderMax.x - this->mSliderMin.x) * this->mSliderPosition);
	this->mActiveY = 4 / height;

	this->mActiveWidth = activeWidth;
	this->mActiveHeight = activeHeight;
}
SliderBar::~SliderBar()
{
	
}

bool SliderBar::AddToRenderer(GraphicsEngine* ge)
{
	float width, height;
	this->GetWidth(width);
	this->GetHeight(height);
	ge->CreateImage(D3DXVECTOR2(this->mActiveX, this->mActiveY), D3DXVECTOR2(this->mActiveWidth, this->mActiveHeight), this->mSliderTexPath);
	Element::AddToRenderer(ge);

	return true;
}
bool SliderBar::RemoveFromRenderer(GraphicsEngine* ge)
{
	ge->DeleteImage(this->mSlider);
	Element::RemoveFromRenderer(ge);
	
	return true;
}
void SliderBar::CheckCollision(float mouseX, float mouseY, bool mousePressed, GraphicsEngine* ge)
{
	/*if((mouseX < this->mActiveX || mouseX > (this->mActiveX+this->mActiveWidth)) || (mouseY < this->mActiveY || mouseY > (this->mActiveY+this->mActiveHeight)))
	{
		return;
	}
	else
	{
		if(mousePressed)
		{
			this->mActiveX = mouseX;
			this->mSlider->SetP
		}
		else if(!mousePressed)
		{
			if(this->mPressed)
			{
				this->mHovered = true;
				if(this->mHoveredImage == NULL)
				{
					if(this->mTextureNameHovered != "")
						this->mHoveredImage = ge->CreateImage(this->GetPositionD3D(), this->GetDimension(), this->mTextureNameHovered);
				}

				this->mPressed = false;
				if(this->mPressedImage != NULL)
				{
					ge->DeleteImage(this->mPressedImage);
					this->mPressedImage = NULL;
				}
				return this->GetEvent();
			}
			else
			{
				this->mHovered = true;
				if(this->mHoveredImage == NULL)
				{
					if(this->mTextureNameHovered != "")
						this->mHoveredImage = ge->CreateImage(this->GetPositionD3D(), this->GetDimension(), this->mTextureNameHovered);
				}
			}
		}
	}
	return NULL;*/
}