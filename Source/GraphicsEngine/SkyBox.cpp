#include "SkyBox.h"


SkyBox::SkyBox(D3DXVECTOR3 pos, int latitude, int longitude) : StaticMesh(pos)
{
	int m_numSphereVertices = ((latitude-2) * longitude) + 2;
	int m_numSphereFaces = ((latitude-3)*(longitude)*2) + (longitude*2);

	float sphereYaw = 0.0f;
	float spherePitch = 0.0f;
	D3DXMATRIX rotationx;
	D3DXMATRIX rotationy;
	Vertex *vertices = new Vertex[m_numSphereVertices];

	D3DXVECTOR3 currVertPos = D3DXVECTOR3(0,0,0);

	vertices[0].pos.x = 0.0f;
	vertices[0].pos.y = 0.0f;
	vertices[0].pos.z = 1.0f;
	
	for(int i = 0; i < latitude-2; ++i)
	{
		spherePitch = (i + 1) * (3.14f / (latitude - 1));
		D3DXMatrixRotationX(&rotationx, spherePitch);
		for(int j = 0; j < longitude; ++j)
		{
			sphereYaw = j * (6.28f / (longitude));
			D3DXMatrixRotationZ(&rotationy, sphereYaw);
			D3DXVECTOR3 pv = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
			D3DXMATRIX pm = rotationx * rotationy;
			D3DXVec3TransformCoord(&currVertPos, &pv, &pm);	
			D3DXVec3Normalize(&currVertPos, &currVertPos );
			vertices[i * longitude + j + 1].pos.x = currVertPos.x;
			vertices[i * longitude + j + 1].pos.y = currVertPos.y;
			vertices[i * longitude + j + 1].pos.z = currVertPos.z;
		}
	}

	vertices[m_numSphereVertices - 1].pos.x =  0.0f;
	vertices[m_numSphereVertices - 1].pos.y =  0.0f;
	vertices[m_numSphereVertices - 1].pos.z = -1.0f;


	MeshStrip* strip = new MeshStrip();
	strip->SetVerts(vertices);
	strip->setNrOfVerts(m_numSphereVertices);





	
	int *indices = new int[m_numSphereFaces * 3];

	int k = 0;
	for(int l = 0; l < longitude-1; ++l)
	{
		indices[k] = 0;
		indices[k+1] = l+1;
		indices[k+2] = l+2;
		k += 3;
	}

	indices[k] = 0;
	indices[k+1] = longitude;
	indices[k+2] = 1;
	k += 3;

	for(int i = 0; i < latitude-3; ++i)
	{
		for(int j = 0; j < longitude-1; ++j)
		{
			indices[k]   = i*longitude+j+1;
			indices[k+1] = i*longitude+j+2;
			indices[k+2] = (i+1)*longitude+j+1;

			indices[k+3] = (i+1)*longitude+j+1;
			indices[k+4] = i*longitude+j+2;
			indices[k+5] = (i+1)*longitude+j+2;

			k += 6; // next quad
		}

		indices[k]   = (i*longitude)+longitude;
		indices[k+1] = (i*longitude)+1;
		indices[k+2] = ((i+1)*longitude)+longitude;

		indices[k+3] = ((i+1)*longitude)+longitude;
		indices[k+4] = (i*longitude)+1;
		indices[k+5] = ((i+1)*longitude)+1;

		k += 6;
	}

	for(int l = 0; l < longitude-1; ++l)
	{
		indices[k] = m_numSphereVertices-1;
		indices[k+1] = (m_numSphereVertices-1)-(l+1);
		indices[k+2] = (m_numSphereVertices-1)-(l+2);
		k += 3;
	}

	indices[k] = m_numSphereVertices-1;
	indices[k+1] = (m_numSphereVertices-1)-longitude;
	indices[k+2] = m_numSphereVertices-2;


	strip->setNrOfIndicies(m_numSphereFaces * 3);
	strip->SetIndicies(indices);

	this->strips->add(strip);
	

	this->topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
}

SkyBox::~SkyBox()
{
	
}

D3DXMATRIX SkyBox::GetSkyboxWorldMatrix(D3DXVECTOR3 camPos)
{
	//Reset sphereWorld
	D3DXMatrixIdentity(&this->worldMatrix);
	
	//Define sphereWorld's world space matrix
	D3DXMATRIX Scale;
	D3DXMatrixScaling(&Scale, 1900.0f, 1900.0f, 1900.0f );
	//Make sure the sphere is always centered around camera
	D3DXMATRIX Translation;
	D3DXMatrixTranslation(&Translation, camPos.x, camPos.y, camPos.z );

	//Set sphereWorld's world space using the transformations
	D3DXMATRIX worldMatrix = Scale * Translation;

	return worldMatrix;
}