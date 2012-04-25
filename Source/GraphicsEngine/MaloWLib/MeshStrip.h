#ifndef MESHSTRIP
#define MESHSTRIP

#include "stdafx.h"
#include "Object3D.h"
#include "Material.h"

class MeshStrip
{
private:
	Object3D* RenderObject;
	Vertex* mesh;
	int nrOfVerts;
	int nrOfIndicies;
	int* indicies;
	string texture;
	Material* material;

public:
	MeshStrip();
	virtual ~MeshStrip();
	MeshStrip(const MeshStrip* origObj);

	Vertex* getVerts() const { return this->mesh; }
	void SetVerts(Vertex* verts) { this->mesh = verts; }
	int getNrOfVerts() const { return this->nrOfVerts; }
	void setNrOfVerts(int vertno) { this->nrOfVerts = vertno; }
	int* getIndicies() const { return this->indicies; }
	void SetIndicies(int* inds) { this->indicies = inds; }
	int getNrOfIndicies() const { return this->nrOfIndicies; }
	void setNrOfIndicies(int indcount) { this->nrOfIndicies = indcount; }
	string GetTexturePath() const { return this->texture; }
	void SetTexturePath(string path);

	Object3D* GetRenderObject() const { return this->RenderObject; }
	void SetRenderObject(Object3D* ro) { this->RenderObject = ro; }

	void SetMaterial(Material* mat);
	Material* GetMaterial() const { return this->material; }
};

#endif