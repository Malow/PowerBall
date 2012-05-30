#pragma once 
/**
* Author: Jerry Rahmqvist
* Create Date: 16/05/2012
* 
* This Class is a ProgressBar that is scaling with help of one background image and
* two bar images. You can select how much percentage boarder you want around the bar. 
* You can select different boardersize on the top/bottom and right/left. 
* The percentage is calculated from the backgrounds point of view. 
*
* Usage: 
*		ProgressBar* temp = new ProgressBar();		// will give you a bar around top and center (start from 40% of the width)
*		temp->SetPercentOfProgressBarColor1(50);	// give you 50% bar from original size and scalex along x-axis
**/
#include "GraphicsEngine.h"


class ProgressBar
{
	private:
		float mHeight;
		float mWidth;
		
		float mStartBgX;
		float mStartBgY;
		float mFullX;
		float mFullY;

		float mFractionBoarderX;
		float mFractionBoarderY;
		float mStartBarX;
		float mStartBarY;
		float mFullBarX;
		float mFullBarY;

		Image* mBackground;
		Image* mBarColor1;
		Image* mBarColor2;
		Image* mBarColor3;
		Image* mBarColor4;
	public:
		
		ProgressBar(std::string bar = "Media/LoadingScreen/DarkBlue.png", std::string bar2 = "Media/LoadingScreen/Red.png", std::string bar3 = "Media/LoadingScreen/BlueOcean.png" , 
					std::string bar4 = "Media/LoadingScreen/Yellow.png", std::string background = "Media/LoadingScreen/Black.png", 
						D3DXVECTOR2 positionFractionTopLeft = D3DXVECTOR2(0.4f, 0.03f),
						float fractionSizeOfWindowX = 0.2f, float fractionSizeOfWindowY = 0.05f, 	
						float fractionBoarderOfBarX = 0.01f, float fractionBoarderOfBarY = 0.05f);
		ProgressBar(D3DXVECTOR2 positionFractionTopLeft);
		ProgressBar(D3DXVECTOR2 positionFractionTopLeft, float fractionSizeOfWindowX, float fractionSizeOfWindowY);
		ProgressBar(int x, int y, int width, int height);
		~ProgressBar();
		void SetPercentOfProgressBarColor1(float procentageX);
		void SetPercentOfProgressBarColor2(float procentageX);
		void SetPercentOfProgressBarColor3(float procentageX);
		void SetPercentOfProgressBarColor4(float procentageX);
		void SetPercentOfProgressBackground(float percentageX);
		void SetPercentOfProgressBarMixC1AndC2(float percentageX);
		void SetPercentOfProgressBarMixC2AndC3(float percentageX);
		void HideBar();
		void ResetBar();
};