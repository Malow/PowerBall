#pragma once
/**
* Auther: Rikard Johansson
* Create Date: 28/03/2012
* 
* This abstract class will be used as a base of every GUI element
**/

#include "..\GraphicsEngine\GraphicsEngine.h"
#include "..\GraphicsEngine\Image.h"
#include <string>

using namespace std;

class Element
{
private:
	float mX;
	float mY;
	float mZ;

	int mWidth;
	int mHeight;

	float mActiveX;
	float mActiveY;
	float mActiveZ;

	int mActiveWidth;
	int mActiveHeight;

	string mTextureName;

	Image* mImage;

public:
	Element();
	Element(float x, float y, float z, string textureName, int width, int height);
	virtual ~Element();

	/*! Sets the position of the element*/
	void SetPosition(float x, float y, float z){ this->mX = x; this->mY = y;this->mZ = z; };
	/*! Saves the position of the element in the parameters*/
	void GetPosition(float& x, float& y, float& z){ x = this->mX; y = this->mY; z = this->mZ; };

	/*! Sets the width of the element*/
	void SetWidth(int width){ this->mWidth = width; };
	/*! Saves the width of the element in parameter*/
	void GetWidth(int& width){ width = this->mWidth; };

	/*! Set the height of the element*/
	void SetHeight(int height){ this->mHeight = height; };
	/*! Saves the height of the element in parameter*/
	void GetHeight(int& height){ height = this->mHeight; };

	/*! Adds the element to the renderer*/
	bool AddToRenderer(GraphicsEngine* ge);
	/*! Removes the element from the renderer*/
	bool RemoveFromRenderer(GraphicsEngine* ge);
};