#include "DxManager.h"

// Draw normals:
inline void DrawNormals(MaloW::Array<StaticMesh*>* meshes, ID3D11Device* g_Device, ID3D11DeviceContext* g_DeviceContext, D3DXMATRIX VP)
{
	D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	Shader* shadnorm = new Shader();
	if(FAILED(shadnorm->Init(g_Device, g_DeviceContext, "../../MaloWLib/MaloWEngine/DebugNormals.fx", inputDesc, 4)))	// + on last if added above
	{
		MaloW::Debug("Failed to open DebugNormals.fx");
		return;
	}

	//Matrixes
	D3DXMATRIX world, wvp;
	//clear render target
	for(int i = 0; i < meshes->size(); i++)
	{
		MaloW::Array<MeshStrip*>* strips = meshes->get(i)->GetStrips();

		// Set matrixes
		world = meshes->get(i)->GetWorldMatrix();
		wvp = world * VP;

		shadnorm->SetMatrix("WVP", wvp);
		shadnorm->SetMatrix("worldMatrix", world);

		for(int u = 0; u < strips->size(); u++)
		{
			Object3D* obj = strips->get(u)->GetRenderObject();
			g_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

			Buffer* verts = obj->GetVertBuff();
			if(verts)
				verts->Apply();

			Buffer* inds = obj->GetIndsBuff();
			if(inds)
				inds->Apply();
			
			shadnorm->Apply(0);

			// draw
			if(inds)
				g_DeviceContext->DrawIndexed(inds->GetElementCount(), 0, 0);
			else
				g_DeviceContext->Draw(verts->GetElementCount(), 0);
		}
	}

	delete shadnorm;
}

inline void DrawWireFrame(MaloW::Array<StaticMesh*>* meshes, ID3D11Device* g_Device, ID3D11DeviceContext* g_DeviceContext, D3DXMATRIX VP)
{
	D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	Shader* ShadeWF = new Shader();
	if(FAILED(ShadeWF->Init(g_Device, g_DeviceContext, "../../MaloWLib/MaloWEngine/DebugWireframe.fx", inputDesc, 4)))	// + on last if added above
	{
		MaloW::Debug("Failed to open DebugWireframe.fx");
		return;
	}

	//Matrixes
	D3DXMATRIX world, wvp;
	//clear render target
	for(int i = 0; i < meshes->size(); i++)
	{
		MaloW::Array<MeshStrip*>* strips = meshes->get(i)->GetStrips();

		// Set matrixes
		world = meshes->get(i)->GetWorldMatrix();
		wvp = world * VP;

		ShadeWF->SetMatrix("WVP", wvp);
		ShadeWF->SetMatrix("worldMatrix", world);

		for(int u = 0; u < strips->size(); u++)
		{
			Object3D* obj = strips->get(u)->GetRenderObject();
			g_DeviceContext->IASetPrimitiveTopology(obj->GetTopology());
			
			Buffer* verts = obj->GetVertBuff();
			if(verts)
				verts->Apply();

			Buffer* inds = obj->GetIndsBuff();
			if(inds)
				inds->Apply();
			
			ShadeWF->Apply(0);

			// draw
			if(inds)
				g_DeviceContext->DrawIndexed(inds->GetElementCount(), 0, 0);
			else
				g_DeviceContext->Draw(verts->GetElementCount(), 0);
		}
	}

	delete ShadeWF;
}

inline void DrawScreenSpaceBillboardDebug(ID3D11DeviceContext* g_DeviceContext, Shader* Shader, ID3D11ShaderResourceView* view, int ScreenPosition)
{
	D3DXVECTOR2 pos = D3DXVECTOR2(-1.0f, 0.5f);
	D3DXVECTOR2 dim = D3DXVECTOR2(0.3f, 0.4f);

	switch(ScreenPosition)
	{
	case 1:
		pos = D3DXVECTOR2(-1.0f, 0.0f);
		break;
	case 2:
		pos = D3DXVECTOR2(-1.0f, -0.5f);
		break;
	case 3:
		pos = D3DXVECTOR2(-1.0f, -1.0f);
		break;
	case 4:
		pos = D3DXVECTOR2(0.5f, 0.5f);
		break;
	case 5:
		pos = D3DXVECTOR2(0.5f, 0.0f);
		break;
	case 6:
		pos = D3DXVECTOR2(0.5f, -0.5f);
		break;
	case 7:
		pos = D3DXVECTOR2(0.5f, -1.0f);
		break;
	}



	Shader->SetFloat("posx", pos.x);
	Shader->SetFloat("posy", pos.y);
	Shader->SetFloat("dimx", dim.x);
	Shader->SetFloat("dimy", dim.y);
		
	Shader->SetResource("tex2D", view);
	Shader->Apply(0);
	g_DeviceContext->Draw(1, 0);
	
	Shader->SetResource("tex2D", NULL);
	Shader->Apply(0);
}

inline int GetTrianglesQuantity(ID3D11Device* Dx_Device, ID3D11DeviceContext* Dx_DeviceContext)
{
	ID3D11Query* q;
	D3D11_QUERY_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.MiscFlags = 0;
	desc.Query = D3D11_QUERY_PIPELINE_STATISTICS;
	Dx_Device->CreateQuery(&desc, &q);
	D3D11_QUERY_DATA_PIPELINE_STATISTICS stats;
	int prims = 0;
	if (S_OK == Dx_DeviceContext->GetData(q, &stats, sizeof(stats), D3D11_ASYNC_GETDATA_DONOTFLUSH))
	{
		prims = (int)stats.CInvocations;
	}
	q->Release();
	return prims;
}