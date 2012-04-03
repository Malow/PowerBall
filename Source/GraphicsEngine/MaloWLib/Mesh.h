#ifndef MESH_H
#define MESH_H

#include "stdafx.h"
#include "Buffer.h"
#include "Vertex.h"
#include "ObjLoader.h"
#include "Object3D.h"
#include "Material.h"
#include "MeshStrip.h"

/*
Mesh class that represents a 3d-object on the screen

*/

class Mesh
{
protected:
	MaloW::Array<MeshStrip*>* strips;

	D3D_PRIMITIVE_TOPOLOGY topology;

	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	D3DXVECTOR3 scale;
	D3DXMATRIX worldMatrix;

public:
	Mesh(D3DXVECTOR3 pos);
	virtual ~Mesh();

	void SetPosition(D3DXVECTOR3 pos);
	void MoveBy(D3DXVECTOR3 moveby);
	void Rotate(D3DXVECTOR3 rotation);
	void Scale(D3DXVECTOR3 scale);
	void Scale(float scale);

	D3DXVECTOR3 GetPosition() const { return this->pos; }
	D3DXVECTOR3 GetRotation() const { return this->rot; }
	D3DXVECTOR3 GetScaling() const { return this->scale; }

	D3DXMATRIX GetWorldMatrix() { return this->worldMatrix; }
	void RecreateWorldMatrix();
	D3D_PRIMITIVE_TOPOLOGY GetTopology() const { return this->topology; }

	void LoadFromFile(string file);

	MaloW::Array<MeshStrip*>* GetStrips() const { return this->strips; }
};

class ParticleMesh
{
protected:
	Object3D* RenderObject;
	ParticleVertex* mesh;
	int nrOfVerts;
	string texture;
	D3D_PRIMITIVE_TOPOLOGY topology;

	D3DXVECTOR3 pos;
	D3DXMATRIX worldMatrix;

public:
	ParticleMesh(D3DXVECTOR3 pos)
	{
		this->RenderObject = NULL;
		this->mesh = NULL;
		this->nrOfVerts = 0;
		this->texture = "";
		this->topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		this->pos = pos;
	}
	virtual ~ParticleMesh() 
	{
		if(this->mesh)
			delete [] this->mesh;
		if(this->RenderObject)
			delete this->RenderObject;
	}

	ParticleVertex* getVerts() const { return this->mesh; }
	int getNrOfVerts() const { return this->nrOfVerts; }
	string GetTexturePath() const { return this->texture; }
	D3D_PRIMITIVE_TOPOLOGY GetTopology() const { return this->topology; }

	Object3D* GetRenderObject() const { return this->RenderObject; }
	void SetRenderObject(Object3D* ro) { this->RenderObject = ro; }

	D3DXVECTOR3 GetPosition() const { return this->pos; }

	void RecreateWorldMatrix()
	{
		D3DXMATRIX translate;
		D3DXMatrixTranslation(&translate, this->pos.x, this->pos.y, this->pos.z);

		this->worldMatrix = translate;
	}

	D3DXMATRIX GetWorldMatrix() { return this->worldMatrix; }
};


#endif
