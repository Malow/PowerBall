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
		while(MaloW::ProcessEvent* ev = this->PeekEvent())
		{
			if(dynamic_cast<RendererEvent*>(ev) != NULL)
			{
				string msg = ((RendererEvent*)ev)->getMessage();
				if(msg == "Add Mesh")
				{
					this->objects.add(((RendererEvent*)ev)->GetStaticMesh());
				}
				else if(msg == "Delete Mesh")
				{
					StaticMesh* mesh = ((RendererEvent*)ev)->GetStaticMesh();
					for(int i = 0; i < this->objects.size(); i++)
					{
						if(this->objects[i] == mesh)
						{
							delete this->objects.getAndRemove(i);
							mesh = NULL;
						}
					}
				}
				if(msg == "Add AniMesh")
				{
					this->animations.add(((RendererEvent*)ev)->GetAnimatedMesh());
				}
				else if(msg == "Delete AniMesh")
				{
					AnimatedMesh* mesh = ((RendererEvent*)ev)->GetAnimatedMesh();
					for(int i = 0; i < this->animations.size(); i++)
					{
						if(this->animations[i] == mesh)
						{
							delete this->animations.getAndRemove(i);
							mesh = NULL;
						}
					}
				}

				else if(msg == "Add Light with shadows")
				{
					((RendererEvent*)ev)->GetLight()->InitShadowMap(this->Dx_Device, this->params.ShadowMapSettings);
					this->lights.add(((RendererEvent*)ev)->GetLight());
				}
				else if(msg == "Add Light")
				{
					this->lights.add(((RendererEvent*)ev)->GetLight());
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
		Dx_DeviceContext->OMSetRenderTargets(0, 0, this->lights[l]->GetShadowMapDSV());
		D3D11_VIEWPORT wp = this->lights[l]->GetShadowMapViewPort();
		Dx_DeviceContext->RSSetViewports(1, &wp);
		Dx_DeviceContext->ClearDepthStencilView(this->lights[l]->GetShadowMapDSV(), D3D11_CLEAR_DEPTH, 1.0f, 0);
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
		D3DXMATRIX vp = this->lights[l]->GetViewProjMatrix();

		/*
		// Forward
		this->Shader_ForwardRendering->SetResourceAtIndex(l, "ShadowMap", this->lights[l]->GetShadowMapSRV());
		this->Shader_ForwardRendering->SetStructMemberAtIndexAsMatrix(l, "lights", "LightViewProj", vp);
		this->Shader_ForwardRendering->SetStructMemberAtIndexAsFloat4(l, "lights", "LightPosition", D3DXVECTOR4(this->lights[l]->GetPosition(), 1));
		this->Shader_ForwardRendering->SetStructMemberAtIndexAsFloat4(l, "lights", "LightColor", D3DXVECTOR4(this->lights[l]->GetColor(), 1));
		this->Shader_ForwardRendering->SetStructMemberAtIndexAsFloat(l, "lights", "LightIntensity", this->lights[l]->GetIntensity());
		*/

		// For deferred:
		this->Shader_DeferredLightning->SetResourceAtIndex(l, "ShadowMap", this->lights[l]->GetShadowMapSRV());
		this->Shader_DeferredLightning->SetStructMemberAtIndexAsMatrix(l, "lights", "LightViewProj", vp);
		this->Shader_DeferredLightning->SetStructMemberAtIndexAsFloat4(l, "lights", "LightPosition", D3DXVECTOR4(this->lights[l]->GetPosition(), 1));
		this->Shader_DeferredLightning->SetStructMemberAtIndexAsFloat4(l, "lights", "LightColor", D3DXVECTOR4(this->lights[l]->GetColor(), 1));
		this->Shader_DeferredLightning->SetStructMemberAtIndexAsFloat(l, "lights", "LightIntensity", this->lights[l]->GetIntensity());
		
		/*
		// For deferred quad:
		this->Shader_DeferredQuad->SetResourceAtIndex(l, "ShadowMap", this->lights[l]->GetShadowMapSRV());
		this->Shader_DeferredQuad->SetStructMemberAtIndexAsMatrix(l, "lights", "LightViewProj", vp);
		this->Shader_DeferredQuad->SetStructMemberAtIndexAsFloat4(l, "lights", "LightPosition", D3DXVECTOR4(this->lights[l]->GetPosition(), 1));
		this->Shader_DeferredQuad->SetStructMemberAtIndexAsFloat4(l, "lights", "LightColor", D3DXVECTOR4(this->lights[l]->GetColor(), 1));
		this->Shader_DeferredQuad->SetStructMemberAtIndexAsFloat(l, "lights", "LightIntensity", this->lights[l]->GetIntensity());
		*/
	}
	
	float PCF_SIZE = (float)this->params.ShadowMapSettings + 1;
	float PCF_SQUARED = 1 / (PCF_SIZE * PCF_SIZE);

	/*
	// Forward
	this->Shader_ForwardRendering->SetFloat("PCF_SIZE", PCF_SIZE);
	this->Shader_ForwardRendering->SetFloat("PCF_SIZE_SQUARED", PCF_SQUARED);
	this->Shader_ForwardRendering->SetFloat("SMAP_DX", 1.0f / (256 * pow(2.0f, this->params.ShadowMapSettings/2)));
	this->Shader_ForwardRendering->SetFloat("NrOfLights", (float)this->lights.size());
	*/

	
	// Deferred:
	this->Shader_DeferredLightning->SetFloat("PCF_SIZE", PCF_SIZE);
	this->Shader_DeferredLightning->SetFloat("PCF_SIZE_SQUARED", PCF_SQUARED);
	//this->Shader_DeferredLightning->SetFloat("SMAP_DX", 1.0f / (256 * pow(2.0f, this->params.ShadowMapSettings/2)));
	this->Shader_DeferredLightning->SetFloat("SMAP_DX", 1.0f / 256.0f);
	this->Shader_DeferredLightning->SetFloat("NrOfLights", (float)this->lights.size());
	
	
	/*
	// for deferred quad:
	this->Shader_DeferredQuad->SetFloat("PCF_SIZE", PCF_SIZE);
	this->Shader_DeferredQuad->SetFloat("PCF_SIZE_SQUARED", PCF_SQUARED);
	this->Shader_DeferredQuad->SetFloat("SMAP_DX", 1.0f / (256 * pow(2.0f, this->params.ShadowMapSettings/2)));
	this->Shader_DeferredQuad->SetFloat("NrOfLights", (float)this->lights.size());
	*/
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

HRESULT DxManager::Render()
{
	// Timer
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	float diff = (li.QuadPart - prevTimeStamp) / this->PCFreq;
	this->prevTimeStamp = li.QuadPart;

	this->TimerAnimation += diff / 1000.0f;

	this->RenderShadowMap();

	//this->RenderForward();
	
	this->RenderDeferredGeometry();

	this->RenderDeferredSkybox();
	
	// Debug: Render Normals
	//MaloW::Array<StaticMesh*>* meshes = &this->objects;
	//DrawNormals(meshes, this->Dx_Device, this->Dx_DeviceContext, this->camera->GetViewMatrix() * this->camera->GetProjectionMatrix());
	// Debug: Render Wireframe
	//MaloW::Array<StaticMesh*>* meshes = &this->objects;
	//DrawWireFrame(meshes, this->Dx_Device, this->Dx_DeviceContext, this->camera->GetViewMatrix() * this->camera->GetProjectionMatrix());
	
	
	//this->RenderQuadDeferred();
	//this->RenderDeferredTexture();
	this->RenderDeferredPerPixel();

	this->RenderParticles();
	
	this->RenderImages();


	// Debugging:
	
	/*
	// Render RTs pictures
	this->Dx_DeviceContext->ClearDepthStencilView(this->Dx_DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);	
	for(int q = 0; q < this->NrOfRenderTargets; q++)
		DrawScreenSpaceBillboardDebug(this->Dx_DeviceContext, this->Shader_BillBoard, this->Dx_GbufferSRVs[q], q); 
	*/
	
	/*
	// Render shadowmap pictures:
	for(int q = 0; q < this->lights.size(); q++)
		DrawScreenSpaceBillboardDebug(this->Dx_DeviceContext, this->Shader_BillBoard, this->lights[q]->GetShadowMapSRV(), q); 
	*/

	this->RenderAntiAliasing();

	if(FAILED(Dx_SwapChain->Present( 0, 0 )))
		return E_FAIL;

	return S_OK;
}






















