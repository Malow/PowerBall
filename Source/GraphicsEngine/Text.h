#pragma once

#include "stdafx.h"

struct Font
{
	ID3D11ShaderResourceView* texture;
	int charTexCoords[256];
};

class Text
{
private:
	string text;
	D3DXVECTOR2 position;
	float size;
	Font font;

public:
	Text(string text, D3DXVECTOR2 position, float size);
	virtual ~Text();
	
	void SetText(string newText) { this->text = newText; }
	string GetText() const { return this->text; }
	void SetPosition(D3DXVECTOR2 pos) { this->position = pos; }
	D3DXVECTOR2 GetPostition() const { return this->position; }
	void SetSize(float size) { this->size = size; }
	float GetSize() const { return this->size; }
	void SetFont(Font newFont) { this->font = newFont; }
	Font GetFont() const { return this->font; }
	
};