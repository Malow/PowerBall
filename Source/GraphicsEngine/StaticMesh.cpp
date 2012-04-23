#include "StaticMesh.h"


StaticMesh::StaticMesh(D3DXVECTOR3 pos) : Mesh(pos)
{

}

StaticMesh::~StaticMesh()
{

}

void StaticMesh::LoadFromFile(string file)
{
	Mesh::LoadFromFile(file);
}


MaloW::Array<MeshStrip*>* StaticMesh::GetStrips()
{ 
	return this->strips; 
}