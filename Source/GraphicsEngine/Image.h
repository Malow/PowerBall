#pragma once

/*
Image class that represents a 2d object on the screen

*/

#include "stdafx.h"

class Image
{
private:
	D3DXVECTOR2 position;
	D3DXVECTOR2 dimensions;
	ID3D11ShaderResourceView* texture;


public:
	Image(D3DXVECTOR2 pos, D3DXVECTOR2 dim);
	virtual ~Image();

	D3DXVECTOR2 GetPosition() const { return this->position; }
	ID3D11ShaderResourceView* GetTexture() const { return this->texture; }
	void SetTexture(ID3D11ShaderResourceView* text) { this->texture = text; }
	D3DXVECTOR2 GetDimensions() const { return this->dimensions; }

};