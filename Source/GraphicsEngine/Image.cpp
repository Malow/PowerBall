#include "Image.h"

Image::Image(D3DXVECTOR2 pos, D3DXVECTOR2 dim)
{
	this->position = pos;
	this->dimensions = dim;
	this->texture = NULL;
}

Image::~Image()
{
	if(this->texture)
		this->texture->Release();
}

float Image::GetAlphaAt(int x, int y)
{
	float ret = 0.0f;
	if(this->texture)
	{
		/*
		ID3D11Resource** res;
		this->texture->GetResource(res);
		res[0]->
 		// Copy individual texture elements into texture array.
		// for each texture element...
		for(UINT i = 0; i < this->mNrOfTextures; i++)
		{
			// for each mipmap level...
			for(UINT j = 0; j < texElementDesc.MipLevels; ++j)
			{
				D3D10_MAPPED_TEXTURE2D mappedTex2D;
				srcTex[i]->Map(j, D3D10_MAP_READ, 0, &mappedTex2D);
			
				  this->gDevice->UpdateSubresource(texArray, 
					D3D10CalcSubresource(j, i, texElementDesc.MipLevels),
					0, mappedTex2D.pData, mappedTex2D.RowPitch, 0);

				srcTex[i]->Unmap(J);
			}
		}
		*/
	}
	return ret;
}