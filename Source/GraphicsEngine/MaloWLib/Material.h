#ifndef MATERIAL
#define MATERIAL

#include "stdafx.h"

enum MaterialType
{
	LAMBERT = 0,
	BLINN = 1,
};

class Material
{
private:


	
public:
	float SpecularPower;
	D3DXVECTOR3 SpecularColor;
	D3DXVECTOR3 AmbientColor;
	D3DXVECTOR3 DiffuseColor;

	Material(MaterialType type)
	{
		if(type == LAMBERT)
		{
			this->SpecularPower = 30.0f;
			this->SpecularColor = D3DXVECTOR3(0.05f, 0.05f, 0.05f);
			this->AmbientColor = D3DXVECTOR3(0.2f, 0.2f, 0.2f);
			this->DiffuseColor = D3DXVECTOR3(0.6f, 0.6f, 0.6f);
		}
		if(type == BLINN)
		{
			this->SpecularPower = 10.0f;
			this->SpecularColor = D3DXVECTOR3(0.5f, 0.5f, 0.5f);
			this->AmbientColor = D3DXVECTOR3(0.2f, 0.2f, 0.2f);
			this->DiffuseColor = D3DXVECTOR3(0.6f, 0.6f, 0.6f);
		}

	}

	Material(const Material& obj)
	{
		this->SpecularColor = obj.SpecularColor;
		this->SpecularPower = obj.SpecularPower;
		this->AmbientColor = obj.AmbientColor;
		this->DiffuseColor = obj.DiffuseColor;
	}

	Material(const Material* obj)
	{
		this->SpecularColor = obj->SpecularColor;
		this->SpecularPower = obj->SpecularPower;
		this->AmbientColor = obj->AmbientColor;
		this->DiffuseColor = obj->DiffuseColor;
	}

	Material()
	{
			this->SpecularPower = 30.0f;
			this->SpecularColor = D3DXVECTOR3(0.15f, 0.15f, 0.15f);
			this->AmbientColor = D3DXVECTOR3(0.2f, 0.2f, 0.2f);
			this->DiffuseColor = D3DXVECTOR3(0.6f, 0.6f, 0.6f);
	}

	virtual ~Material() { }
};

#endif