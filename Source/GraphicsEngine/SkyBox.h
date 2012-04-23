#pragma once

#include "stdafx.h"
#include "Vertex.h"
#include "Buffer.h"
#include "Shader.h"
#include "StaticMesh.h"

using namespace std;

class SkyBox : public StaticMesh
{
private: //Private variables

public:
	SkyBox(D3DXVECTOR3 pos, int latitude, int longitude);
	virtual ~SkyBox();
	D3DXMATRIX GetSkyboxWorldMatrix(D3DXVECTOR3 camPos);
};




/*
Usage:

g_SkyBox = new SkyBox();
g_SkyBox->CreateSphere(10, 10, g_Device, g_DeviceContext);



wvp = g_SkyBox->GetWorldMatrix() * view * proj;
g_SkyBoxShader->SetMatrix("gWVP", wvp);
g_SkyBox->Apply();
g_SkyBoxShader->Apply(0);
g_DeviceContext->DrawIndexed(g_SkyBox->GetIndexCount(), 0, 0);



*/