#include "GameObject.h"

GameObject::GameObject()
{
	this->mMesh = NULL;
	this->mStartPosition = D3DXVECTOR3(0,0,0);
}

GameObject::~GameObject()
{
	if(this->mMesh)
		GetGraphicsEngine()->DeleteStaticMesh(this->mMesh);
}
D3DXVECTOR3 GameObject::GetPosition() const 
{ 
	return this->mMesh->GetPosition(); 
}

Vector3 GameObject::GetPositionVector3() const
{
	D3DXVECTOR3 pos = this->mMesh->GetPosition();
	Vector3 position = Vector3(pos.x, pos.y, pos.z);
	return position;
}
	
StaticMesh* GameObject::GetMesh() const
{
	return this->mMesh;
}

void GameObject::SetPosition(const D3DXVECTOR3 position) 
{ 
	this->mMesh->SetPosition(position); 
}
			
void GameObject::SetPosition(const float x, const float y, const float z) 
{ 
	this->mMesh->SetPosition(D3DXVECTOR3(x,y,z)); 
}

void GameObject::SetPosition(const Vector3 position) 
{ 
	this->mMesh->SetPosition(D3DXVECTOR3(position.x,position.y,position.z)); 
}

void GameObject::SetMesh(StaticMesh* mesh) 
{
	this->mMesh = mesh;
}