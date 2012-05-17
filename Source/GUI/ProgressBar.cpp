#include "ProgressBar.h"

ProgressBar::ProgressBar(std::string bar, std::string bar2, std::string background, D3DXVECTOR2 positionPercentageTopLeft, float percentageSizeOfWindowX, 
						float percentageSizeOfWindowY, 	float percentageBoarderOfBarX, float percentageBoarderOfBarY)
{
	

	/* height and width of window */
	this->mWidth = GetGraphicsEngine()->GetEngineParameters().windowWidth;
	this->mHeight = GetGraphicsEngine()->GetEngineParameters().windowHeight;

	/* size of the little smaller background progressbar. */
	this->mStartBgX = positionPercentageTopLeft.x*this->mWidth;
	this->mStartBgY = positionPercentageTopLeft.y*this->mHeight;
	this->mFullX = percentageSizeOfWindowX*this->mWidth;
	this->mFullY = percentageSizeOfWindowY*this->mHeight;

	/* size of the boarder around the progressbar. */
	this->mProcentBoarderX = percentageBoarderOfBarX;
	this->mProcentBoarderY = percentageBoarderOfBarY;

	/* size of the progressbar in reference to the backgroundbar. */
	this->mStartBarX = this->mStartBgX + this->mFullX*this->mProcentBoarderX;
	this->mStartBarY = this->mStartBgY + this->mFullY*this->mProcentBoarderY;
	this->mFullBarX = this->mFullX*(1.0f - 2.0f*this->mProcentBoarderX);
	this->mFullBarY = this->mFullBarY = this->mFullY*(1.0f - 2.0f*this->mProcentBoarderY);

	this->mBackground = GetGraphicsEngine()->CreateImage(D3DXVECTOR2(this->mStartBgX, this->mStartBgY), D3DXVECTOR2(this->mFullX, this->mFullY ), background);
	this->mBarColor1 = GetGraphicsEngine()->CreateImage(D3DXVECTOR2(this->mStartBarX, this->mStartBarY), D3DXVECTOR2(this->mFullBarX, this->mFullBarY), bar);
	this->mBarColor2 = GetGraphicsEngine()->CreateImage(D3DXVECTOR2(this->mStartBarX, this->mStartBarY), D3DXVECTOR2(this->mFullBarX, this->mFullBarY), bar2);
}

ProgressBar::ProgressBar(D3DXVECTOR2 positionPercentageTopLeft)
{
	
	std::string bar = "Media/LoadingScreen/DarkBlue.png";
	std::string bar2 = "Media/LoadingScreen/Red.png";
	std::string background = "Media/LoadingScreen/Black.png";
	
	float percentageSizeOfWindowX = 0.2f;
	float percentageSizeOfWindowY = 0.05f;
	float percentageBoarderOfBarX = 0.01f;
	float percentageBoarderOfBarY = 0.05f;


	/* height and width of window */
	this->mWidth = GetGraphicsEngine()->GetEngineParameters().windowWidth;
	this->mHeight = GetGraphicsEngine()->GetEngineParameters().windowHeight;

	/* size of the little smaller background progressbar. */
	this->mStartBgX = positionPercentageTopLeft.x*this->mWidth;
	this->mStartBgY = positionPercentageTopLeft.y*this->mHeight;
	this->mFullX = percentageSizeOfWindowX*this->mWidth;
	this->mFullY = percentageSizeOfWindowY*this->mHeight;

	/* size of the boarder around the progressbar. */
	this->mProcentBoarderX = percentageBoarderOfBarX;
	this->mProcentBoarderY = percentageBoarderOfBarY;

	/* size of the progressbar in reference to the backgroundbar. */
	this->mStartBarX = this->mStartBgX + this->mFullX*this->mProcentBoarderX;
	this->mStartBarY = this->mStartBgY + this->mFullY*this->mProcentBoarderY;
	this->mFullBarX = this->mFullX*(1.0f - 2.0f*this->mProcentBoarderX);
	this->mFullBarY = this->mFullBarY = this->mFullY*(1.0f - 2.0f*this->mProcentBoarderY);

	this->mBackground = GetGraphicsEngine()->CreateImage(D3DXVECTOR2(this->mStartBgX, this->mStartBgY), D3DXVECTOR2(this->mFullX, this->mFullY ), background);
	this->mBarColor1 = GetGraphicsEngine()->CreateImage(D3DXVECTOR2(this->mStartBarX, this->mStartBarY), D3DXVECTOR2(this->mFullBarX, this->mFullBarY), bar);
	this->mBarColor2 = GetGraphicsEngine()->CreateImage(D3DXVECTOR2(this->mStartBarX, this->mStartBarY), D3DXVECTOR2(this->mFullBarX, this->mFullBarY), bar2);
}
ProgressBar::~ProgressBar()
{
	GetGraphicsEngine()->DeleteImage(this->mBackground);
	GetGraphicsEngine()->DeleteImage(this->mBarColor1);
	GetGraphicsEngine()->DeleteImage(this->mBarColor2);
}

void ProgressBar::SetPercentOfProgressBarColor1(float percentageX)
{
	if(percentageX <= 100.0f && percentageX >= 0.0f)
	{
		float scale = percentageX*0.01f*this->mFullX;
		this->mBackground->SetDimensions(D3DXVECTOR2(scale,this->mFullY));
		float newPercentage = this->mProcentBoarderX*(this->mFullBarX/scale);
		float newFullBar = scale*(1.0f -2.0f*newPercentage);
		if( newFullBar >= 0)
			this->mBarColor1->SetDimensions(D3DXVECTOR2(newFullBar,this->mFullBarY));
		//this->mBarColor1->SetPosition(D3DXVECTOR2(this->mStartBgX + this->mFullX*this->mProcentBoarderX*percentageX*0.01f, this->mStartBarY)); 
		this->mBarColor2->SetDimensions(D3DXVECTOR2(0,this->mFullBarY));
	}
}

void ProgressBar::SetPercentOfProgressBarColor2(float percentageX)
{
	if(percentageX <= 100.0f && percentageX >= 0.0f)
	{
		float scale = percentageX*0.01f*this->mFullX;
		this->mBackground->SetDimensions(D3DXVECTOR2(scale,this->mFullY));
		float newPercentage = this->mProcentBoarderX*(this->mFullBarX/scale);
		float newFullBar = scale*(1.0f -2.0f*newPercentage);
		this->mBarColor1->SetDimensions(D3DXVECTOR2(0,this->mFullBarY));
		if( newFullBar >= 0)
			this->mBarColor2->SetDimensions(D3DXVECTOR2(newFullBar,this->mFullBarY));
		
	}
}

void ProgressBar::ResetBar()
{
	this->mBackground->SetDimensions(D3DXVECTOR2(this->mFullX,this->mFullY));
	this->mBarColor1->SetDimensions(D3DXVECTOR2(this->mFullBarX*0.8f,this->mFullBarY));
	this->mBarColor2->SetDimensions(D3DXVECTOR2(this->mFullBarX*0.4f,this->mFullBarY));

}
