#include "DxManager.h"

#include "DxManagerDebugging.h"

HRESULT DxManager::Update(float deltaTime)
{
	// update subsystems
	//ps.update(deltaTime);
	this->camera->update(deltaTime);				// Moved to Life below since deltaTime isnt needed

	return S_OK;
}

void DxManager::Life()
{
	while(this->stayAlive)
	{
		if(MaloW::ProcessEvent* ev = this->PeekEvent())
		{
			if(dynamic_cast<RendererEvent*>(ev) != NULL)
			{
				string msg = ((RendererEvent*)ev)->getMessage();
				if(msg == "Add Mesh")
					this->objects.add(((RendererEvent*)ev)->GetMesh());
				else if(msg == "Add Light")
				{
					((RendererEvent*)ev)->GetLight()->InitShadowMap(this->Dx_Device);
					this->lights.add(((RendererEvent*)ev)->GetLight());
				}
				else if(msg == "Delete Mesh")
				{
					Mesh* mesh = ((RendererEvent*)ev)->GetMesh();
					for(int i = 0; i < this->objects.size(); i++)
					{
						if(this->objects[i] == mesh)
						{
							delete this->objects.getAndRemove(i);
							mesh = NULL;
						}
					}
				}
				else if(msg == "Delete Light")
				{
					Light* light = ((RendererEvent*)ev)->GetLight();
					for(int i = 0; i < this->lights.size(); i++)
					{
						if(this->lights[i] == light)
						{
							delete this->lights.getAndRemove(i);
							light = NULL;
						}
					}
				}
				else if(msg == "Add Image")
					this->images.add(((RendererEvent*)ev)->GetImage());
				else if(msg == "Delete Image")
				{
					Image* img = ((RendererEvent*)ev)->GetImage();
					for(int i = 0; i < this->images.size(); i++)
					{
						if(this->images[i] == img)
						{
							delete this->images.getAndRemove(i);
							img = NULL;
						}
					}
				}
			}

			delete ev;
		}

		this->Render();
		this->framecount++;
	}
}

void DxManager::RenderParticles()
{
	/*
	this->Dx_DeviceContext->OMSetRenderTargets(1, &this->Dx_RenderTargetView, this->Dx_DepthStencilView);
	this->Dx_DeviceContext->RSSetViewports(1, &this->Dx_Viewport);

	//Matrixes
	D3DXMATRIX world, view, proj, wvp;
	view = this->camera->GetViewMatrix();
	proj = this->camera->GetProjectionMatrix();

	// Draw particleEffects
	MaloW::Array<ParticleMesh*>* psMeshes = this->ps.getMeshes();
	for(int i = 0; i < psMeshes->size(); i++)
	{
		Object3D* obj = this->createParticleObject(psMeshes->get(i));
		
		// Set matrixes
		world = psMeshes->get(i)->GetWorldMatrix();
		wvp = world * view * proj;
		Shader_Particle->SetMatrix("WVP", wvp);
		Shader_Particle->SetMatrix("worldMatrix", world);

		Dx_DeviceContext->IASetPrimitiveTopology(obj->GetTopology());

		Buffer* verts = obj->GetVertBuff();
		if(verts)
			verts->Apply();

		if(ID3D11ShaderResourceView* texture = obj->GetTexture())
		{
			Shader_Particle->SetBool("textured", true);
			Shader_Particle->SetResource("tex2D", texture);
		}
		else
			Shader_Particle->SetBool("textured", false);

		Buffer* inds = obj->GetIndsBuff();
		if(inds)
			inds->Apply();

		Shader_Particle->Apply(0);

		// draw
		if(inds)
			Dx_DeviceContext->DrawIndexed(inds->GetElementCount(), 0, 0);
		else
			Dx_DeviceContext->Draw(verts->GetElementCount(), 0);
	}

	// Unbind resources:
	Shader_Particle->SetResource("tex2D", NULL);
	Shader_Particle->Apply(0);

	delete psMeshes;
	*/
}

void DxManager::RenderShadowMap()
{
	// Generate and send shadowmaps to the main-shader
	for (int l = 0; l < this->lights.size(); l++)
	{
		Dx_DeviceContext->OMSetRenderTargets(0, 0, this->lights[l]->GetShadowMapDSV(this->currentShadowMapSize));
		D3D11_VIEWPORT wp = this->lights[l]->GetShadowMapViewPort(this->currentShadowMapSize);
		Dx_DeviceContext->RSSetViewports(1, &wp);
		Dx_DeviceContext->ClearDepthStencilView(this->lights[l]->GetShadowMapDSV(this->currentShadowMapSize), D3D11_CLEAR_DEPTH, 1.0f, 0);
		for(int i = 0; i < this->objects.size(); i++)
		{
			MaloW::Array<MeshStrip*>* strips = this->objects[i]->GetStrips();
			D3DXMATRIX wvp = this->objects[i]->GetWorldMatrix() * this->lights[l]->GetViewProjMatrix();
			this->Shader_ShadowMap->SetMatrix("LightWVP", wvp);
			for(int u = 0; u < strips->size(); u++)
			{
				Object3D* obj = strips->get(u)->GetRenderObject();
				Dx_DeviceContext->IASetPrimitiveTopology(obj->GetTopology());
				Buffer* verts = obj->GetVertBuff();
				if(verts)
					verts->Apply();
				Shader_ShadowMap->SetBool("textured", false);

				Buffer* inds = obj->GetIndsBuff();
				if(inds)
					inds->Apply();

				Shader_ShadowMap->Apply(0);

				// draw
				if(inds)
					Dx_DeviceContext->DrawIndexed(inds->GetElementCount(), 0, 0);
				else
					Dx_DeviceContext->Draw(verts->GetElementCount(), 0);
			}
		}
		this->Shader_ForwardRendering->SetResourceAtIndex(l, "ShadowMap", this->lights[l]->GetShadowMapSRV(this->currentShadowMapSize));
		D3DXMATRIX vp = this->lights[l]->GetViewProjMatrix();
		this->Shader_ForwardRendering->SetStructMemberAtIndexAsMatrix(l, "lights", "LightViewProj", vp);
		this->Shader_ForwardRendering->SetStructMemberAtIndexAsFloat4(l, "lights", "LightPosition", D3DXVECTOR4(this->lights[l]->GetPosition(), 1));
		this->Shader_ForwardRendering->SetStructMemberAtIndexAsFloat4(l, "lights", "LightColor", D3DXVECTOR4(this->lights[l]->GetColor(), 1));
		this->Shader_ForwardRendering->SetStructMemberAtIndexAsFloat(l, "lights", "LightIntensity", this->lights[l]->GetIntensity());

		// For deferred:
		this->Shader_DeferredLightning->SetResourceAtIndex(l, "ShadowMap", this->lights[l]->GetShadowMapSRV(this->currentShadowMapSize));
		this->Shader_DeferredLightning->SetStructMemberAtIndexAsMatrix(l, "lights", "LightViewProj", vp);
		this->Shader_DeferredLightning->SetStructMemberAtIndexAsFloat4(l, "lights", "LightPosition", D3DXVECTOR4(this->lights[l]->GetPosition(), 1));
		this->Shader_DeferredLightning->SetStructMemberAtIndexAsFloat4(l, "lights", "LightColor", D3DXVECTOR4(this->lights[l]->GetColor(), 1));
		this->Shader_DeferredLightning->SetStructMemberAtIndexAsFloat(l, "lights", "LightIntensity", this->lights[l]->GetIntensity());

		// For deferred quad:
		this->Shader_DeferredQuad->SetResourceAtIndex(l, "ShadowMap", this->lights[l]->GetShadowMapSRV(this->currentShadowMapSize));
		this->Shader_DeferredQuad->SetStructMemberAtIndexAsMatrix(l, "lights", "LightViewProj", vp);
		this->Shader_DeferredQuad->SetStructMemberAtIndexAsFloat4(l, "lights", "LightPosition", D3DXVECTOR4(this->lights[l]->GetPosition(), 1));
		this->Shader_DeferredQuad->SetStructMemberAtIndexAsFloat4(l, "lights", "LightColor", D3DXVECTOR4(this->lights[l]->GetColor(), 1));
		this->Shader_DeferredQuad->SetStructMemberAtIndexAsFloat(l, "lights", "LightIntensity", this->lights[l]->GetIntensity());

	}
	float PCF_SIZE = 3.0f;
	this->Shader_ForwardRendering->SetFloat("PCF_SIZE", (float)(int)PCF_SIZE);
	this->Shader_ForwardRendering->SetFloat("PCF_SIZE_SQUARED", PCF_SIZE * PCF_SIZE);
	this->Shader_ForwardRendering->SetFloat("SMAP_DX", 1.0f / (256 * pow(2.0f, this->currentShadowMapSize)));
	this->Shader_ForwardRendering->SetFloat("NrOfLights", (float)this->lights.size());

	// Deferred:
	this->Shader_DeferredLightning->SetFloat("PCF_SIZE", (float)(int)PCF_SIZE);
	this->Shader_DeferredLightning->SetFloat("PCF_SIZE_SQUARED", PCF_SIZE * PCF_SIZE);
	this->Shader_DeferredLightning->SetFloat("SMAP_DX", 1.0f / (256 * pow(2.0f, this->currentShadowMapSize)));
	this->Shader_DeferredLightning->SetFloat("NrOfLights", (float)this->lights.size());

	// for deferred quad:
	this->Shader_DeferredQuad->SetFloat("PCF_SIZE", (float)(int)PCF_SIZE);
	this->Shader_DeferredQuad->SetFloat("PCF_SIZE_SQUARED", PCF_SIZE * PCF_SIZE);
	this->Shader_DeferredQuad->SetFloat("SMAP_DX", 1.0f / (256 * pow(2.0f, this->currentShadowMapSize)));
	this->Shader_DeferredQuad->SetFloat("NrOfLights", (float)this->lights.size());
}

void DxManager::RenderForward()
{
	//Matrixes
	D3DXMATRIX world, view, proj, wvp;
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

		this->Shader_ForwardRendering->SetMatrix("WVP", wvp);
		this->Shader_ForwardRendering->SetMatrix("worldMatrix", world);

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

void DxManager::RenderDeferredGeometry()
{
	//Matrixes
	D3DXMATRIX world, view, proj, wvp;
	view = this->camera->GetViewMatrix();
	proj = this->camera->GetProjectionMatrix();

	//clear and set render target/depth
	this->Dx_DeviceContext->OMSetRenderTargets(this->NrOfRenderTargets, this->Dx_GbufferRTs, this->Dx_DepthStencilView);
	this->Dx_DeviceContext->RSSetViewports(1, &this->Dx_Viewport);

	this->Dx_DeviceContext->ClearDepthStencilView(this->Dx_DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	//float ClearColor[4] = {0.5f, 0.71f, 1.0f, 1};
	float ClearColor[4] = {-1.0f, -1.0f, -1.0f, -1.0f};
	for(int i = 0; i < this->NrOfRenderTargets; i++)
		this->Dx_DeviceContext->ClearRenderTargetView(this->Dx_GbufferRTs[i], ClearColor);
	float ClearColor2[4] = {0.5f, 0.71f, 1.0f, 1};
	this->Dx_DeviceContext->ClearRenderTargetView(this->Dx_GbufferRTs[0], ClearColor2);
	
	this->Shader_DeferredGeometry->SetFloat4("CameraPosition", D3DXVECTOR4(this->camera->getPosition(), 1));

	for(int i = 0; i < this->objects.size(); i++)
	{
		MaloW::Array<MeshStrip*>* strips = this->objects[i]->GetStrips();
		
		// Set matrixes
		world = this->objects[i]->GetWorldMatrix();
		wvp = world * view * proj;

		this->Shader_DeferredGeometry->SetMatrix("WVP", wvp);
		this->Shader_DeferredGeometry->SetMatrix("worldMatrix", world);

		for(int u = 0; u < strips->size(); u++)
		{
			Object3D* obj = strips->get(u)->GetRenderObject();
			this->Dx_DeviceContext->IASetPrimitiveTopology(obj->GetTopology());

			// Setting lightning from material
			this->Shader_DeferredGeometry->SetFloat4("SpecularColor", D3DXVECTOR4(strips->get(u)->GetMaterial()->SpecularColor, 1));
			this->Shader_DeferredGeometry->SetFloat("SpecularPower", strips->get(u)->GetMaterial()->SpecularPower);
			this->Shader_DeferredGeometry->SetFloat4("AmbientLight", D3DXVECTOR4(strips->get(u)->GetMaterial()->AmbientColor, 1));
			this->Shader_DeferredGeometry->SetFloat4("DiffuseColor", D3DXVECTOR4(strips->get(u)->GetMaterial()->DiffuseColor, 1));

			Buffer* verts = obj->GetVertBuff();
			if(verts)
				verts->Apply();

			if(ID3D11ShaderResourceView* texture = obj->GetTexture())
			{
				this->Shader_DeferredGeometry->SetBool("textured", true);
				this->Shader_DeferredGeometry->SetResource("tex2D", texture);
			}
			else
				this->Shader_DeferredGeometry->SetBool("textured", false);

			Buffer* inds = obj->GetIndsBuff();
			if(inds)
				inds->Apply();
			
			this->Shader_DeferredGeometry->Apply(0);

			// draw
			if(inds)
				this->Dx_DeviceContext->DrawIndexed(inds->GetElementCount(), 0, 0);
			else
				this->Dx_DeviceContext->Draw(verts->GetElementCount(), 0);
		}
	}

	// Unbind resources:
	this->Shader_DeferredGeometry->SetResource("tex2D", NULL);
	this->Shader_DeferredGeometry->Apply(0);
}

void DxManager::RenderDeferredPerPixel()
{
	//clear and set render target/depth
	this->Dx_DeviceContext->OMSetRenderTargets(1, &this->Dx_RenderTargetView, this->Dx_DepthStencilView);
	this->Dx_DeviceContext->RSSetViewports(1, &this->Dx_Viewport);

	static float ClearColor[4] = {0.5f, 0.71f, 1.0f, 1};
	this->Dx_DeviceContext->ClearRenderTargetView(this->Dx_RenderTargetView, ClearColor);

	this->Dx_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	this->Shader_DeferredLightning->SetResource("Texture", this->Dx_GbufferSRVs[0]);
	this->Shader_DeferredLightning->SetResource("NormalAndDepth", this->Dx_GbufferSRVs[1]);
	this->Shader_DeferredLightning->SetResource("Position", this->Dx_GbufferSRVs[2]);
	this->Shader_DeferredLightning->SetResource("Specular", this->Dx_GbufferSRVs[3]);
	D3DXMATRIX vp = this->camera->GetViewMatrix() * this->camera->GetProjectionMatrix();
	this->Shader_DeferredLightning->SetMatrix("CameraVP", vp);
	this->Shader_DeferredLightning->SetFloat4("CameraPosition", D3DXVECTOR4(this->camera->getPosition(), 1));
	

	// Set SSAO settings
	this->ssao.PreRender(this->Shader_DeferredLightning, this->params, this->camera);
	this->Shader_DeferredLightning->Apply(0);

	
	this->Dx_DeviceContext->Draw(1, 0);
	
	
	// Unbind resources:
	this->Shader_DeferredLightning->SetResource("Texture", NULL);
	this->Shader_DeferredLightning->SetResource("NormalAndDepth", NULL);
	this->Shader_DeferredLightning->SetResource("Position", NULL);
	this->Shader_DeferredLightning->SetResource("Specular", NULL);
	for(int i = 0; i < this->lights.size(); i++)
	{
		this->Shader_DeferredLightning->SetResourceAtIndex(i, "ShadowMap", NULL);
	}

	// Unbind SSAO
	this->ssao.PostRender(this->Shader_DeferredLightning);

	this->Shader_DeferredLightning->Apply(0);
}

void DxManager::RenderImages()
{
	/*
	this->Dx_DeviceContext->OMSetRenderTargets(1, &this->Dx_RenderTargetView, this->Dx_DepthStencilView);
	this->Dx_DeviceContext->RSSetViewports(1, &this->Dx_Viewport);
	this->Dx_DeviceContext->ClearDepthStencilView(this->Dx_DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	*/
	this->Dx_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	

	for(int i = 0; i < this->images.size(); i++)
	{
		Image* img = this->images[i];
		// if Convert from screenspace is needed, which it isnt.
		this->Shader_BillBoard->SetFloat("posx", (img->GetPosition().x / this->params.windowWidth) * 2 - 1);
		this->Shader_BillBoard->SetFloat("posy", 2 - (img->GetPosition().y / this->params.windowHeight) * 2 - 1);
		this->Shader_BillBoard->SetFloat("dimx", (img->GetDimensions().x / this->params.windowWidth) * 2);
		this->Shader_BillBoard->SetFloat("dimy", -(img->GetDimensions().y / this->params.windowHeight) * 2);
		
		/*// if -1 to 1
		this->Shader_BillBoard->SetFloat("posx", img->GetPosition().x);
		this->Shader_BillBoard->SetFloat("posy", img->GetPosition().y);
		this->Shader_BillBoard->SetFloat("dimx", img->GetDimensions().x);
		this->Shader_BillBoard->SetFloat("dimy", img->GetDimensions().y);
		*/
		this->Shader_BillBoard->SetResource("tex2D", img->GetTexture());
		this->Shader_BillBoard->Apply(0);
		this->Dx_DeviceContext->Draw(1, 0);
	}
	this->Shader_BillBoard->SetResource("tex2D", NULL);
	this->Shader_BillBoard->Apply(0);
}

void DxManager::RenderQuadDeferred()
{
	//clear and set render target/depth
	this->Dx_DeviceContext->OMSetRenderTargets(1, &this->Dx_DeferredQuadRT, this->Dx_DepthStencilView);
	this->Dx_DeviceContext->RSSetViewports(1, &this->Dx_Viewport);

	static float ClearColor[4] = {0.0f, 0.0f, 0.0f, 1.0f};
	this->Dx_DeviceContext->ClearRenderTargetView(this->Dx_DeferredQuadRT, ClearColor);

	this->Dx_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	
	this->Shader_DeferredQuad->SetResource("Texture", this->Dx_GbufferSRVs[0]);
	this->Shader_DeferredQuad->SetResource("NormalAndDepth", this->Dx_GbufferSRVs[1]);
	this->Shader_DeferredQuad->SetResource("Position", this->Dx_GbufferSRVs[2]);
	this->Shader_DeferredQuad->SetResource("Specular", this->Dx_GbufferSRVs[3]);
	D3DXMATRIX v = this->camera->GetViewMatrix();
	D3DXMATRIX p = this->camera->GetProjectionMatrix();
	D3DXMATRIX vp = v * p;
	this->Shader_DeferredQuad->SetMatrix("CameraVP", vp);
	this->Shader_DeferredQuad->SetMatrix("CameraView", v);
	this->Shader_DeferredQuad->SetMatrix("CameraProj", p);
	this->Shader_DeferredQuad->SetFloat("CameraFar", 200.0f);
	this->Shader_DeferredQuad->SetFloat("CameraNear", 1.0f);
	this->Shader_DeferredQuad->SetFloat("ScreenWidth", this->params.windowWidth);
	this->Shader_DeferredQuad->SetFloat("ScreenHeight", this->params.windowHeight);
	this->Shader_DeferredQuad->SetFloat4("CameraPosition", D3DXVECTOR4(this->camera->getPosition(), 1));
	
	this->Shader_DeferredQuad->Apply(0);
	
	
	this->Dx_DeviceContext->Draw(this->lights.size(), 0);
	
	
	// Unbind resources:
	this->Shader_DeferredQuad->SetResource("Texture", NULL);
	this->Shader_DeferredQuad->SetResource("NormalAndDepth", NULL);
	this->Shader_DeferredQuad->SetResource("Position", NULL);
	this->Shader_DeferredQuad->SetResource("Specular", NULL);
	for(int i = 0; i < this->lights.size(); i++)
	{
		this->Shader_DeferredQuad->SetResourceAtIndex(i, "ShadowMap", NULL);
	}
	this->Shader_DeferredQuad->Apply(0);
}

void DxManager::RenderDeferredTexture()
{
	//clear and set render target/depth
	this->Dx_DeviceContext->OMSetRenderTargets(1, &this->Dx_RenderTargetView, this->Dx_DepthStencilView);
	this->Dx_DeviceContext->RSSetViewports(1, &this->Dx_Viewport);

	static float ClearColor[4] = {0.5f, 0.71f, 1.0f, 1};
	this->Dx_DeviceContext->ClearRenderTargetView(this->Dx_RenderTargetView, ClearColor);

	this->Dx_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	this->Shader_DeferredTexture->SetResource("Texture", this->Dx_GbufferSRVs[0]);
	this->Shader_DeferredTexture->SetResource("NormalAndDepth", this->Dx_GbufferSRVs[1]);
	this->Shader_DeferredTexture->SetResource("Position", this->Dx_GbufferSRVs[2]);
	this->Shader_DeferredTexture->SetResource("Specular", this->Dx_GbufferSRVs[3]);
	this->Shader_DeferredTexture->SetResource("LightAccu", this->Dx_DeferredSRV);

	// Set SSAO settings
	this->ssao.PreRender(this->Shader_DeferredTexture, this->params, this->camera);
	this->Shader_DeferredTexture->Apply(0);

	
	this->Dx_DeviceContext->Draw(1, 0);
	
	
	// Unbind resources:
	this->Shader_DeferredTexture->SetResource("Texture", NULL);
	this->Shader_DeferredTexture->SetResource("NormalAndDepth", NULL);
	this->Shader_DeferredTexture->SetResource("Position", NULL);
	this->Shader_DeferredTexture->SetResource("Specular", NULL);
	this->Shader_DeferredTexture->SetResource("LightAccu", NULL);

	// Unbind SSAO
	this->ssao.PostRender(this->Shader_DeferredTexture);

	this->Shader_DeferredTexture->Apply(0);
}

HRESULT DxManager::Render()
{
	this->RenderShadowMap();

	//this->RenderForward();

	this->RenderDeferredGeometry();
	
	// Debug: Render Normals
	//MaloW::Array<Mesh*>* meshes = &this->objects;
	//DrawNormals(meshes, this->Dx_Device, this->Dx_DeviceContext, this->camera->GetViewMatrix() * this->camera->GetProjectionMatrix());
	// Debug: Render Wireframe
	//MaloW::Array<Mesh*>* meshes = &this->objects;
	//DrawWireFrame(meshes, this->Dx_Device, this->Dx_DeviceContext, this->camera->GetViewMatrix() * this->camera->GetProjectionMatrix());

	
	//this->RenderQuadDeferred();
	//this->RenderDeferredTexture();
	this->RenderDeferredPerPixel();

	this->RenderParticles();

	this->RenderImages();

	

	// Debugging:

	
	// Render RTs pictures
	/*
	this->Dx_DeviceContext->ClearDepthStencilView(this->Dx_DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);	
	for(int q = 0; q < this->NrOfRenderTargets; q++)
		DrawScreenSpaceBillboard(this->Dx_DeviceContext, this->Shader_BillBoard, this->Dx_GbufferSRVs[q], q); 
	*/
	
	// Render shadowmap pictures:
	/*
	for(int q = 0; q < this->lights.size(); q++)
		DrawScreenSpaceBillboard(this->Dx_DeviceContext, this->Shader_BillBoard, this->lights[q]->GetShadowMapSRV(this->currentShadowMapSize), q); 
	*/

	if(FAILED(Dx_SwapChain->Present( 0, 0 )))
		return E_FAIL;

	return S_OK;
}






















