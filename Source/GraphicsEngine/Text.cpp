#include "Text.h"

Text::Text(string text, D3DXVECTOR2 position, float size)
{
	this->text = text;
	this->position = position;
	this->size = size;
	this->font.texture = NULL;
	for(int i = 0; i < 256; i++)
		this->font.charTexCoords[i] = 0;
}

Text::~Text()
{
	if(this->font.texture)
		this->font.texture->Release();
}
