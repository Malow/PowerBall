#include "ProgressBar.h"

ProgressBar::ProgressBar(std::string bar, std::string bar2, std::string bar3, std::string background, D3DXVECTOR2 positionFractionTopLeft, float fractionSizeOfWindowX, 
						float fractionSizeOfWindowY, 	float fractionBoarderOfBarX, float fractionBoarderOfBarY)
{
	

	/* height and width of window */
	this->mWidth = GetGraphicsEngine()->GetEngineParameters().windowWidth;
	this->mHeight = GetGraphicsEngine()->GetEngineParameters().windowHeight;

	/* size of the little smaller background progressbar. */
	this->mStartBgX = positionFractionTopLeft.x*this->mWidth;
	this->mStartBgY = positionFractionTopLeft.y*this->mHeight;
	this->mFullX = fractionSizeOfWindowX*this->mWidth;
	this->mFullY = fractionSizeOfWindowY*this->mHeight;

	/* size of the boarder around the progressbar. */
	this->mFractionBoarderX = fractionBoarderOfBarX;
	this->mFractionBoarderY = fractionBoarderOfBarY;

	/* size of the progressbar in reference to the backgroundbar. */
	this->mStartBarX = this->mStartBgX + this->mFullX*this->mFractionBoarderX;
	this->mStartBarY = this->mStartBgY + this->mFullY*this->mFractionBoarderY;
	this->mFullBarX = this->mFullX*(1.0f - 2.0f*this->mFractionBoarderX);
	this->mFullBarY = this->mFullBarY = this->mFullY*(1.0f - 2.0f*this->mFractionBoarderY);

	this->mBackground = GetGraphicsEngine()->CreateImage(D3DXVECTOR2(this->mStartBgX, this->mStartBgY), D3DXVECTOR2(this->mFullX, this->mFullY ), background);
	this->mBarColor1 = GetGraphicsEngine()->CreateImage(D3DXVECTOR2(this->mStartBarX, this->mStartBarY), D3DXVECTOR2(this->mFullBarX, this->mFullBarY), bar);
	this->mBarColor2 = GetGraphicsEngine()->CreateImage(D3DXVECTOR2(this->mStartBarX, this->mStartBarY), D3DXVECTOR2(this->mFullBarX, this->mFullBarY), bar2);
	this->mBarColor3 = GetGraphicsEngine()->CreateImage(D3DXVECTOR2(this->mStartBarX, this->mStartBarY), D3DXVECTOR2(this->mFullBarX, this->mFullBarY), bar3);
}

ProgressBar::ProgressBar(D3DXVECTOR2 positionFractionTopLeft, float fractionSizeOfWindowX, float fractionSizeOfWindowY)
{
	std::string bar = "Media/LoadingScreen/DarkBlue.png";
	std::string bar2 = "Media/LoadingScreen/Red.png";
	std::string bar3 = "Media/LoadingScreen/BlueOcean.png";
	std::string background = "Media/LoadingScreen/Black.png";
	
	
	float fractionBoarderOfBarX = 0.01f;
	float fractionBoarderOfBarY = 0.05f;


	/* height and width of window */
	this->mWidth = GetGraphicsEngine()->GetEngineParameters().windowWidth;
	this->mHeight = GetGraphicsEngine()->GetEngineParameters().windowHeight;

	/* size of the little smaller background progressbar. */
	this->mStartBgX = positionFractionTopLeft.x*this->mWidth;
	this->mStartBgY = positionFractionTopLeft.y*this->mHeight;
	this->mFullX = fractionSizeOfWindowX*this->mWidth;
	this->mFullY = fractionSizeOfWindowY*this->mHeight;

	/* size of the boarder around the progressbar. */
	this->mFractionBoarderX = fractionBoarderOfBarX;
	this->mFractionBoarderY = fractionBoarderOfBarY;

	/* size of the progressbar in reference to the backgroundbar. */
	this->mStartBarX = this->mStartBgX + this->mFullX*this->mFractionBoarderX;
	this->mStartBarY = this->mStartBgY + this->mFullY*this->mFractionBoarderY;
	this->mFullBarX = this->mFullX*(1.0f - 2.0f*this->mFractionBoarderX);
	this->mFullBarY = this->mFullBarY = this->mFullY*(1.0f - 2.0f*this->mFractionBoarderY);

	this->mBackground = GetGraphicsEngine()->CreateImage(D3DXVECTOR2(this->mStartBgX, this->mStartBgY), D3DXVECTOR2(this->mFullX, this->mFullY ), background);
	this->mBarColor1 = GetGraphicsEngine()->CreateImage(D3DXVECTOR2(this->mStartBarX, this->mStartBarY), D3DXVECTOR2(this->mFullBarX, this->mFullBarY), bar);
	this->mBarColor2 = GetGraphicsEngine()->CreateImage(D3DXVECTOR2(this->mStartBarX, this->mStartBarY), D3DXVECTOR2(this->mFullBarX, this->mFullBarY), bar2);
	this->mBarColor3 = GetGraphicsEngine()->CreateImage(D3DXVECTOR2(this->mStartBarX, this->mStartBarY), D3DXVECTOR2(this->mFullBarX, this->mFullBarY), bar3);
}

ProgressBar::ProgressBar(D3DXVECTOR2 positionFractionTopLeft)
{
	
	std::string bar = "Media/LoadingScreen/DarkBlue.png";
	std::string bar2 = "Media/LoadingScreen/Red.png";
	std::string bar3 = "Media/LoadingScreen/BlueOcean.png";
	std::string background = "Media/LoadingScreen/Black.png";
	
	float fractionSizeOfWindowX = 0.2f;
	float fractionSizeOfWindowY = 0.05f;
	float fractionBoarderOfBarX = 0.01f;
	float fractionBoarderOfBarY = 0.05f;


	/* height and width of window */
	this->mWidth = GetGraphicsEngine()->GetEngineParameters().windowWidth;
	this->mHeight = GetGraphicsEngine()->GetEngineParameters().windowHeight;

	/* size of the little smaller background progressbar. */
	this->mStartBgX = positionFractionTopLeft.x*this->mWidth;
	this->mStartBgY = positionFractionTopLeft.y*this->mHeight;
	this->mFullX = fractionSizeOfWindowX*this->mWidth;
	this->mFullY = fractionSizeOfWindowY*this->mHeight;

	/* size of the boarder around the progressbar. */
	this->mFractionBoarderX = fractionBoarderOfBarX;
	this->mFractionBoarderY = fractionBoarderOfBarY;

	/* size of the progressbar in reference to the backgroundbar. */
	this->mStartBarX = this->mStartBgX + this->mFullX*this->mFractionBoarderX;
	this->mStartBarY = this->mStartBgY + this->mFullY*this->mFractionBoarderY;
	this->mFullBarX = this->mFullX*(1.0f - 2.0f*this->mFractionBoarderX);
	this->mFullBarY = this->mFullBarY = this->mFullY*(1.0f - 2.0f*this->mFractionBoarderY);

	this->mBackground = GetGraphicsEngine()->CreateImage(D3DXVECTOR2(this->mStartBgX, this->mStartBgY), D3DXVECTOR2(this->mFullX, this->mFullY ), background);
	this->mBarColor1 = GetGraphicsEngine()->CreateImage(D3DXVECTOR2(this->mStartBarX, this->mStartBarY), D3DXVECTOR2(this->mFullBarX, this->mFullBarY), bar);
	this->mBarColor2 = GetGraphicsEngine()->CreateImage(D3DXVECTOR2(this->mStartBarX, this->mStartBarY), D3DXVECTOR2(this->mFullBarX, this->mFullBarY), bar2);
	this->mBarColor3 = GetGraphicsEngine()->CreateImage(D3DXVECTOR2(this->mStartBarX, this->mStartBarY), D3DXVECTOR2(this->mFullBarX, this->mFullBarY), bar3);
}

ProgressBar::ProgressBar(int x, int y, int width, int height)
{
	
	std::string bar = "Media/LoadingScreen/DarkBlue.png";
	std::string bar2 = "Media/LoadingScreen/Red.png";
	std::string bar3 = "Media/LoadingScreen/BlueOcean.png";
	std::string background = "Media/LoadingScreen/Black.png";
	
	float fractionSizeOfWindowX = 0.2f;
	float fractionSizeOfWindowY = 0.05f;
	float fractionBoarderOfBarX = 0.01f;
	float fractionBoarderOfBarY = 0.05f;


	/* height and width of window */
	this->mWidth = GetGraphicsEngine()->GetEngineParameters().windowWidth;
	this->mHeight = GetGraphicsEngine()->GetEngineParameters().windowHeight;

	/* size of the little smaller background progressbar. */
	this->mStartBgX = x;
	this->mStartBgY = y;
	this->mFullX = width;
	this->mFullY = height;

	/* size of the boarder around the progressbar. */
	this->mFractionBoarderX = fractionBoarderOfBarX;
	this->mFractionBoarderY = fractionBoarderOfBarY;

	/* size of the progressbar in reference to the backgroundbar. */
	this->mStartBarX = this->mStartBgX + this->mFullX*this->mFractionBoarderX;
	this->mStartBarY = this->mStartBgY + this->mFullY*this->mFractionBoarderY;
	this->mFullBarX = this->mFullX*(1.0f - 2.0f*this->mFractionBoarderX);
	this->mFullBarY = this->mFullBarY = this->mFullY*(1.0f - 2.0f*this->mFractionBoarderY);

	this->mBackground = GetGraphicsEngine()->CreateImage(D3DXVECTOR2(this->mStartBgX, this->mStartBgY), D3DXVECTOR2(this->mFullX, this->mFullY ), background);
	this->mBarColor1 = GetGraphicsEngine()->CreateImage(D3DXVECTOR2(this->mStartBarX, this->mStartBarY), D3DXVECTOR2(this->mFullBarX, this->mFullBarY), bar);
	this->mBarColor2 = GetGraphicsEngine()->CreateImage(D3DXVECTOR2(this->mStartBarX, this->mStartBarY), D3DXVECTOR2(this->mFullBarX, this->mFullBarY), bar2);
	this->mBarColor3 = GetGraphicsEngine()->CreateImage(D3DXVECTOR2(this->mStartBarX, this->mStartBarY), D3DXVECTOR2(this->mFullBarX, this->mFullBarY), bar3);
}

ProgressBar::~ProgressBar()
{
	GetGraphicsEngine()->DeleteImage(this->mBackground);
	GetGraphicsEngine()->DeleteImage(this->mBarColor1);
	GetGraphicsEngine()->DeleteImage(this->mBarColor2);
	GetGraphicsEngine()->DeleteImage(this->mBarColor3);
}

void ProgressBar::SetPercentOfProgressBarColor1(float percentageX)
{
	if(percentageX <= 100.0f && percentageX >= 0.0f)
	{
		float scale = percentageX*0.01f*this->mFullX;
		//this->mBackground->SetDimensions(D3DXVECTOR2(scale,this->mFullY));
		float newFraction;
		if( scale != 0)
			newFraction = this->mFractionBoarderX*(this->mFullBarX/scale);
		else
			newFraction = 0;
		float newFullBar = scale*(1.0f -2.0f*newFraction);
		this->mBarColor2->SetDimensions(D3DXVECTOR2(0,this->mFullBarY));
		this->mBarColor3->SetDimensions(D3DXVECTOR2(0,this->mFullBarY));
		if( newFullBar >= 0)
			this->mBarColor1->SetDimensions(D3DXVECTOR2(newFullBar,this->mFullBarY));
		//this->mBarColor1->SetPosition(D3DXVECTOR2(this->mStartBgX + this->mFullX*this->mFractionBoarderX*percentageX*0.01f, this->mStartBarY)); 
		
	}
}

void ProgressBar::SetPercentOfProgressBarColor2(float percentageX)
{
	if(percentageX <= 100.0f && percentageX >= 0.0f)
	{
		float scale = percentageX*0.01f*this->mFullX;
		//this->mBackground->SetDimensions(D3DXVECTOR2(scale,this->mFullY));
		float newFraction;
		if( scale != 0)
			newFraction = this->mFractionBoarderX*(this->mFullBarX/scale);
		else
			newFraction = 0;
		
		float newFullBar = scale*(1.0f -2.0f*newFraction);
		this->mBarColor1->SetDimensions(D3DXVECTOR2(0,this->mFullBarY));
		this->mBarColor3->SetDimensions(D3DXVECTOR2(0,this->mFullBarY));
		if( newFullBar >= 0)
			this->mBarColor2->SetDimensions(D3DXVECTOR2(newFullBar,this->mFullBarY));
		
	}
}

void ProgressBar::SetPercentOfProgressBarColor3(float percentageX)
{
	if(percentageX <= 100.0f && percentageX >= 0.0f)
	{
		float scale = percentageX*0.01f*this->mFullX;
		//this->mBackground->SetDimensions(D3DXVECTOR2(scale,this->mFullY));
		float newFraction;
		if( scale != 0)
			newFraction = this->mFractionBoarderX*(this->mFullBarX/scale);
		else
			newFraction = 0;
		float newFullBar = scale*(1.0f -2.0f*newFraction);
		this->mBarColor1->SetDimensions(D3DXVECTOR2(this->mFullBarX,this->mFullBarY));
		this->mBarColor2->SetDimensions(D3DXVECTOR2(0,this->mFullBarY));
		if( newFullBar >= 0)
			this->mBarColor3->SetDimensions(D3DXVECTOR2(newFullBar,this->mFullBarY));
		
	}
}


void ProgressBar::SetPercentOfProgressBackground(float percentageX)
{
	float scale = percentageX*0.01f*this->mFullX;
	this->mBackground->SetDimensions(D3DXVECTOR2(scale,this->mFullY));
}

void ProgressBar::HideBar()
{
	this->mBackground->SetDimensions(D3DXVECTOR2(0,this->mFullY));
	this->mBarColor1->SetDimensions(D3DXVECTOR2(0,this->mFullBarY));
	this->mBarColor2->SetDimensions(D3DXVECTOR2(0,this->mFullBarY));
	this->mBarColor3->SetDimensions(D3DXVECTOR2(0,this->mFullBarY));
}

void ProgressBar::ResetBar()
{
	this->mBackground->SetDimensions(D3DXVECTOR2(this->mFullX,this->mFullY));
	this->mBarColor1->SetDimensions(D3DXVECTOR2(this->mFullBarX*0.8f,this->mFullBarY));
	this->mBarColor2->SetDimensions(D3DXVECTOR2(this->mFullBarX*0.4f,this->mFullBarY));

}
