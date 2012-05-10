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
	float opacity;
	ID3D11ShaderResourceView* texture;


public:
	Image(D3DXVECTOR2 pos, D3DXVECTOR2 dim);
	virtual ~Image();

	D3DXVECTOR2 GetPosition() const { return this->position; }
	void SetPosition(D3DXVECTOR2 pos){ this->position = pos; }
	ID3D11ShaderResourceView* GetTexture() const { return this->texture; }
	void SetTexture(ID3D11ShaderResourceView* text) { this->texture = text; }
	D3DXVECTOR2 GetDimensions() const { return this->dimensions; }
	void SetDimensions(D3DXVECTOR2 dims) { this->dimensions = dims; }
	float GetOpacity() const { return this->opacity; }
	void SetOpacity(float opacity) { this->opacity = opacity; }
};