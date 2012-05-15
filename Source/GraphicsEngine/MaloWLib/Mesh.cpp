#include "Mesh.h"	

Mesh::Mesh(D3DXVECTOR3 pos)
{
	this->specialColor = NULL_COLOR;
	this->usingInvisibilityEffect = false;
	//this->transparency = 0.0f;

	this->strips = new MaloW::Array<MeshStrip*>();

	this->topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	this->pos = pos;

	this->rotQuat = D3DXQUATERNION(0, 0, 0, 1);
	this->scale = D3DXVECTOR3(1, 1, 1);
}

Mesh::~Mesh() 
{
	if(this->strips)
	{
		while(this->strips->size() > 0)
			delete this->strips->getAndRemove(0);

		delete this->strips;
	}
}

void Mesh::LoadFromFile(string file)
{
	// if substr of the last 4 = .obj do this:    - else load other format / print error

	ObjLoader oj;
	ObjData* od = oj.LoadObjFile(file);

	MaloW::Array<MaterialData>* mats = od->mats;
	for(int q = 0; q < mats->size(); q++)
	{
		bool hasFace = false;
		MeshStrip* strip = new MeshStrip();
		

		int nrOfVerts = 0;
		
		Vertex* tempverts = new Vertex[od->faces->size()*3];
		
		for(int i = 0;  i < od->faces->size(); i++)
		{
			if(od->faces->get(i).material == mats->get(q).name)
			{
				int vertpos = od->faces->get(i).data[0][0] - 1;
				int textcoord = od->faces->get(i).data[0][1] - 1;
				int norm = od->faces->get(i).data[0][2] - 1;
				tempverts[nrOfVerts] = Vertex(od->vertspos->get(vertpos), od->textcoords->get(textcoord), od->vertsnorms->get(norm));
				nrOfVerts++;

				vertpos = od->faces->get(i).data[2][0] - 1;
				textcoord = od->faces->get(i).data[2][1] - 1;
				norm = od->faces->get(i).data[2][2] - 1;
				tempverts[nrOfVerts] = Vertex(od->vertspos->get(vertpos), od->textcoords->get(textcoord), od->vertsnorms->get(norm));
				nrOfVerts++;

				vertpos = od->faces->get(i).data[1][0] - 1;
				textcoord = od->faces->get(i).data[1][1] - 1;
				norm = od->faces->get(i).data[1][2] - 1;
				tempverts[nrOfVerts] = Vertex(od->vertspos->get(vertpos), od->textcoords->get(textcoord), od->vertsnorms->get(norm));
				nrOfVerts++;



				hasFace = true;
			}
		}

		strip->setNrOfVerts(nrOfVerts);
		Vertex* verts = new Vertex[nrOfVerts];
		for(int z = 0; z < nrOfVerts; z++)
		{
			verts[z] = tempverts[z];
		}
		delete tempverts;
		strip->SetVerts(verts);
			
		strip->SetTexturePath(od->mats->get(q).texture);

		Material* mat = new Material();
		mat->AmbientColor = od->mats->get(q).ka;
		if(mat->AmbientColor == D3DXVECTOR3(0.0f, 0.0f, 0.0f))				//////////// MaloW Fix, otherwise completely black with most objs
			mat->AmbientColor += D3DXVECTOR3(0.2f, 0.2f, 0.2f);			//////////// MaloW Fix, otherwise completely black with most objs

		mat->DiffuseColor = od->mats->get(q).kd;
		if(mat->DiffuseColor == D3DXVECTOR3(0.0f, 0.0f, 0.0f))				//////////// MaloW Fix, otherwise completely black with most objs
			mat->DiffuseColor += D3DXVECTOR3(0.6f, 0.6f, 0.6f);			//////////// MaloW Fix, otherwise completely black with most objs
			
		mat->SpecularColor = od->mats->get(q).ks;
		strip->SetMaterial(mat);

		if(hasFace)
			this->strips->add(strip);
		else
			delete strip;
	}
	this->topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	delete od;
}

void Mesh::SetSpecialColor(COLOR specialColor)
{
	this->specialColor = specialColor;
}

void Mesh::UseInvisibilityEffect(bool use)
{
	this->usingInvisibilityEffect = use;
}
/*
void Mesh::SetTransparency(float transparency)
{
	if(transparency < 0.0f)
	{
		this->transparency = 0.0f;
	}
	else if(transparency > 1.0f)
	{
		this->transparency = 1.0f;
	}
	else
	{
		this->transparency = transparency;
	}
}*/
void Mesh::SetPosition(D3DXVECTOR3 pos)
{ 
	this->pos = pos;
	this->RecreateWorldMatrix();
}

void Mesh::SetQuaternion(D3DXQUATERNION quat)
{
	this->rotQuat = quat;
	this->RecreateWorldMatrix();
}

void Mesh::MoveBy(D3DXVECTOR3 moveby)
{ 
	this->pos += moveby; 
	this->RecreateWorldMatrix();
}

void Mesh::Rotate(D3DXVECTOR3 radians)
{
	D3DXQUATERNION quaternion;
	D3DXQuaternionRotationYawPitchRoll(&quaternion, radians.y, radians.x, radians.z);
	D3DXQuaternionMultiply(&this->rotQuat, &this->rotQuat, &quaternion);
	this->RecreateWorldMatrix();
}

void Mesh::Rotate(D3DXQUATERNION quat)
{
	D3DXQuaternionMultiply(&this->rotQuat, &this->rotQuat, &quat);
	this->RecreateWorldMatrix();
}

void Mesh::RotateAxis(D3DXVECTOR3 around, float angle)
{
	D3DXQUATERNION quaternion = D3DXQUATERNION(0, 0, 0, 1);
	D3DXQuaternionRotationAxis(&quaternion, &around, angle);
	
	D3DXQuaternionMultiply(&this->rotQuat, &this->rotQuat, &quaternion);
	this->RecreateWorldMatrix();
}

void Mesh::Scale(D3DXVECTOR3 scale)
{
	this->scale.x *= scale.x;
	this->scale.y *= scale.y;
	this->scale.z *= scale.z;
	this->RecreateWorldMatrix();
}

void Mesh::Scale(float scale)
{
	this->scale.x *= scale;
	this->scale.y *= scale;
	this->scale.z *= scale;
	this->RecreateWorldMatrix();
}

void Mesh::RecreateWorldMatrix()
{
	D3DXMATRIX translate;
	D3DXMatrixTranslation(&translate, this->pos.x, this->pos.y, this->pos.z);

	D3DXMATRIX scaling;
	D3DXMatrixScaling(&scaling, this->scale.x, this->scale.y, this->scale.z);

	/*
	// Euler
	D3DXMATRIX x, y, z;
	D3DXMatrixRotationX(&x, this->rot.x);
	D3DXMatrixRotationY(&y, this->rot.y);
	D3DXMatrixRotationZ(&z, this->rot.z);

	D3DXMATRIX world = scaling*x*y*z*translate;
	*/

	// Quaternions
	D3DXMATRIX QuatMat;
	D3DXMatrixRotationQuaternion(&QuatMat, &this->rotQuat); 

	D3DXMATRIX world = scaling*QuatMat*translate;




	this->worldMatrix = world;
}

void Mesh::ResetRotationAndScale()
{
	this->rotQuat = D3DXQUATERNION(0, 0, 0, 1);
	this->scale = D3DXVECTOR3(1, 1, 1);
	this->RecreateWorldMatrix();
}