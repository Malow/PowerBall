#ifndef OBJECT_3D
#define OBJECT_3D

#include "stdafx.h"
#include "Buffer.h"
#include "Vertex.h"

class Object3D
{
private:
	Buffer* verts;
	Buffer* inds;
	ID3D11ShaderResourceView* texture;
	D3D_PRIMITIVE_TOPOLOGY topology;

public:
	Object3D(Buffer* verts, Buffer* inds, ID3D11ShaderResourceView* texture, D3D_PRIMITIVE_TOPOLOGY topology);
	virtual ~Object3D();

	Buffer* GetVertBuff() { return this->verts; }
	Buffer* GetIndsBuff() { return this->inds; }
	D3D_PRIMITIVE_TOPOLOGY GetTopology() { return this->topology; }
	ID3D11ShaderResourceView* GetTexture() { return this->texture; }

};

#endif