#include "Image.h"

Image::Image(D3DXVECTOR2 pos, D3DXVECTOR2 dim)
{
	this->position = pos;
	this->dimensions = dim;
	this->texture = NULL;
	this->opacity = 1.0f;
}

Image::~Image()
{
	if(this->texture)
		this->texture->Release();
}