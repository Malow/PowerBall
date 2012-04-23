#pragma once

#include "stdafx.h"
#include "Mesh.h"

class StaticMesh : public Mesh
{
private:
	int asd;

public:
	StaticMesh(D3DXVECTOR3 pos);
	virtual ~StaticMesh();

	virtual void LoadFromFile(string file);
	virtual MaloW::Array<MeshStrip*>* GetStrips();
};