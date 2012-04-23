#include "DxManager.h"

void DxManager::RenderForward()
{
	//Matrixes
	D3DXMATRIX world, view, proj, wvp, worldInverseTranspose;
	view = this->camera->GetViewMatrix();
	proj = this->camera->GetProjectionMatrix();

	
	//clear render target
	this->Dx_DeviceContext->OMSetRenderTargets(1, &this->Dx_RenderTargetView, this->Dx_DepthStencilView);
	this->Dx_DeviceContext->RSSetViewports(1, &this->Dx_Viewport);
	static float ClearColor[4] = {0.5f, 0.71f, 1.0f, 1};
	this->Dx_DeviceContext->ClearRenderTargetView(this->Dx_RenderTargetView, ClearColor);
	this->Dx_DeviceContext->ClearDepthStencilView(this->Dx_DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);	

	this->Shader_ForwardRendering->SetFloat4("CameraPosition", D3DXVECTOR4(this->camera->getPosition(), 1));

	for(int i = 0; i < this->objects.size(); i++)
	{
		MaloW::Array<MeshStrip*>* strips = this->objects[i]->GetStrips();

		// Set matrixes
		world = this->objects[i]->GetWorldMatrix();
		wvp = world * view * proj;
		D3DXMatrixInverse(&worldInverseTranspose, NULL, &world);
		D3DXMatrixTranspose(&worldInverseTranspose, &worldInverseTranspose);

		this->Shader_ForwardRendering->SetMatrix("WVP", wvp);
		this->Shader_ForwardRendering->SetMatrix("worldMatrix", world);
		this->Shader_ForwardRendering->SetMatrix("worldMatrixInverseTranspose", worldInverseTranspose);

		for(int u = 0; u < strips->size(); u++)
		{
			Object3D* obj = strips->get(u)->GetRenderObject();
			this->Dx_DeviceContext->IASetPrimitiveTopology(obj->GetTopology());

			// Setting lightning from material
			this->Shader_ForwardRendering->SetFloat4("SpecularColor", D3DXVECTOR4(strips->get(u)->GetMaterial()->SpecularColor, 1));
			this->Shader_ForwardRendering->SetFloat("SpecularPower", strips->get(u)->GetMaterial()->SpecularPower);
			this->Shader_ForwardRendering->SetFloat4("AmbientLight", D3DXVECTOR4(strips->get(u)->GetMaterial()->AmbientColor, 1));
			this->Shader_ForwardRendering->SetFloat4("DiffuseColor", D3DXVECTOR4(strips->get(u)->GetMaterial()->DiffuseColor, 1));

			Buffer* verts = obj->GetVertBuff();
			if(verts)
				verts->Apply();

			if(ID3D11ShaderResourceView* texture = obj->GetTexture())
			{
				this->Shader_ForwardRendering->SetBool("textured", true);
				this->Shader_ForwardRendering->SetResource("tex2D", texture);
			}
			else
				this->Shader_ForwardRendering->SetBool("textured", false);

			Buffer* inds = obj->GetIndsBuff();
			if(inds)
				inds->Apply();
			
			this->Shader_ForwardRendering->Apply(0);
			
			// draw
			if(inds)
				this->Dx_DeviceContext->DrawIndexed(inds->GetElementCount(), 0, 0);
			else
				this->Dx_DeviceContext->Draw(verts->GetElementCount(), 0);
		}
	}

	// Unbind resources:
	this->Shader_ForwardRendering->SetResource("tex2D", NULL);
	for(int i = 0; i < this->lights.size(); i++)
	{
		this->Shader_ForwardRendering->SetResourceAtIndex(i, "ShadowMap", NULL);
	}
	this->Shader_ForwardRendering->Apply(0);
}