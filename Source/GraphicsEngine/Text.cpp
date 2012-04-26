#include "Text.h"

Text::Text(string text, D3DXVECTOR2 position, float size)
{
	this->text = text;
	this->ToUpper();

	this->position = position;
	this->size = size;
	this->font = new Font();
	this->font->texture = NULL;
	for(int i = 0; i < 256; i++)
		this->font->charTexCoords[i] = 0;
}

void Text::ToUpper()
{
	for(int i = 0; i < this->text.size(); i++)
	{
		this->text[i] = toupper(this->text[i]);
	}
}

void Text::DeleteFromEnd(int CharsToDel) 
{
	if(CharsToDel <= this->text.size())
		this->text = this->text.substr(0, this->text.size() - CharsToDel);
}

Text::~Text()
{
	if(this->font->texture)
		this->font->texture->Release();

	if(this->font)
		delete this->font;
}
