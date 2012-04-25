#include "MeshStrip.h"

MeshStrip::MeshStrip()
{
	this->RenderObject = NULL;
	this->indicies = NULL;
	this->mesh = NULL;
	this->material = new Material(LAMBERT);
	this->nrOfIndicies = 0;
	this->nrOfVerts = 0;
	this->texture = "";
}
	
MeshStrip::~MeshStrip() 
{
	if(this->mesh)
		delete [] this->mesh;
	if(this->indicies)
		delete [] this->indicies;
	if(this->RenderObject)
		delete this->RenderObject;
	if(this->material)
		delete this->material;
}

MeshStrip::MeshStrip(const MeshStrip* origObj)
{
	this->RenderObject = origObj->RenderObject;
	this->mesh = new Vertex(origObj->mesh);
	this->nrOfVerts = origObj->nrOfVerts;
	this->nrOfIndicies = origObj->nrOfIndicies;
	this->indicies = new int[origObj->nrOfIndicies];
	for(int i = 0; i < origObj->nrOfIndicies; i++)
	{
		this->indicies[i] = origObj->indicies[i];
	}
	this->texture = origObj->texture;
	this->material = new Material(origObj->material);
}

void MeshStrip::SetMaterial(Material* mat)
{
	if(this->material)
		delete this->material;
	this->material = NULL;

	this->material = mat;
}

void MeshStrip::SetTexturePath(string path)
{ 
	this->texture = path; 
	if(path == "")		// Workaround for obj loading meshes without texture
	{
		for(int i = 0; i < this->nrOfVerts; i++)
			this->mesh[i].color = D3DXVECTOR3(0.5, 0.5, 0.5);
	}

}