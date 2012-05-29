#include "DxManager.h"


void DxManager::RenderDeferredGeometry()
{
	//Matrixes
	D3DXMATRIX world, view, proj, wvp, worldInverseTranspose;
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
	
	//Normal (visibile) geometry
	for(int i = 0; i < this->objects.size(); i++)
	{
		if(!this->objects[i]->IsUsingInvisibility())
		{
			MaloW::Array<MeshStrip*>* strips = this->objects[i]->GetStrips();
		
			// Per object
			this->Shader_DeferredGeometry->SetInt("specialColor", this->objects[i]->GetSpecialColor());

			// Set matrixes
			world = this->objects[i]->GetWorldMatrix();
			wvp = world * view * proj;
			D3DXMatrixInverse(&worldInverseTranspose, NULL, &world);
			D3DXMatrixTranspose(&worldInverseTranspose, &worldInverseTranspose);

			this->Shader_DeferredGeometry->SetMatrix("WVP", wvp);
			this->Shader_DeferredGeometry->SetMatrix("worldMatrix", world);
			this->Shader_DeferredGeometry->SetMatrix("worldMatrixInverseTranspose", worldInverseTranspose);

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
		else
		{
			this->invisibleGeometry = true;
		}
	}


	//Normal (visible) Animated meshes
	this->Shader_DeferredAnimatedGeometry->SetFloat4("CameraPosition", D3DXVECTOR4(this->camera->getPosition(), 1));
	for(int i = 0; i < this->animations.size(); i++)
	{
		if(!this->animations[i]->IsUsingInvisibility())
		{
			KeyFrame* one = NULL;
			KeyFrame* two = NULL;
			float t = 0.0f;
			this->animations[i]->SetCurrentTime(this->TimerAnimation);
			this->animations[i]->GetCurrentKeyFrames(&one, &two, t);

			MaloW::Array<MeshStrip*>* stripsOne = one->strips;
			MaloW::Array<MeshStrip*>* stripsTwo = two->strips;
		
			// Set matrixes
			world = this->animations[i]->GetWorldMatrix();
			wvp = world * view * proj;
			D3DXMatrixInverse(&worldInverseTranspose, NULL, &world);
			D3DXMatrixTranspose(&worldInverseTranspose, &worldInverseTranspose);
		
			this->Shader_DeferredAnimatedGeometry->SetMatrix("WVP", wvp);
			this->Shader_DeferredAnimatedGeometry->SetMatrix("worldMatrix", world);
			this->Shader_DeferredAnimatedGeometry->SetMatrix("worldMatrixInverseTranspose", worldInverseTranspose);
			this->Shader_DeferredAnimatedGeometry->SetFloat("t", t);

			this->Shader_DeferredGeometry->SetInt("specialColor", this->animations[i]->GetSpecialColor()); //*kraschar för att antalet animationer > antalet object

			for(int u = 0; u < stripsOne->size(); u++)
			{
				Object3D* objOne = stripsOne->get(u)->GetRenderObject();
				Object3D* objTwo = stripsTwo->get(u)->GetRenderObject();

				this->Dx_DeviceContext->IASetPrimitiveTopology(objOne->GetTopology());

				// Setting lightning from material
				this->Shader_DeferredAnimatedGeometry->SetFloat4("SpecularColor", D3DXVECTOR4(stripsOne->get(u)->GetMaterial()->SpecularColor, 1));
				this->Shader_DeferredAnimatedGeometry->SetFloat("SpecularPower", stripsOne->get(u)->GetMaterial()->SpecularPower);
				this->Shader_DeferredAnimatedGeometry->SetFloat4("AmbientLight", D3DXVECTOR4(stripsOne->get(u)->GetMaterial()->AmbientColor, 1));
				this->Shader_DeferredAnimatedGeometry->SetFloat4("DiffuseColor", D3DXVECTOR4(stripsOne->get(u)->GetMaterial()->DiffuseColor, 1));

				Buffer* vertsOne = objOne->GetVertBuff();
				Buffer* vertsTwo = objTwo->GetVertBuff();

				ID3D11Buffer* vertexBuffers [] = {vertsOne->GetBufferPointer(), vertsTwo->GetBufferPointer()};
				UINT strides [] = {sizeof(Vertex), sizeof(Vertex)};
				UINT offsets [] = {0, 0};

				this->Dx_DeviceContext->IASetVertexBuffers(0, 2, vertexBuffers, strides, offsets);

				if(ID3D11ShaderResourceView* texture = objOne->GetTexture())
				{
					this->Shader_DeferredAnimatedGeometry->SetBool("textured", true);
					this->Shader_DeferredAnimatedGeometry->SetResource("tex2D", texture);
				}
				else
					this->Shader_DeferredAnimatedGeometry->SetBool("textured", false);
			

				this->Shader_DeferredAnimatedGeometry->Apply(0);

				// draw
				this->Dx_DeviceContext->Draw(vertsOne->GetElementCount(), 0);
			}
		}
		else
		{
			this->invisibleGeometry = true;
		}
	}
	this->Shader_DeferredAnimatedGeometry->SetResource("tex2D", NULL);
	this->Shader_DeferredAnimatedGeometry->Apply(0);
	

	// Unbind resources:
	this->Shader_DeferredGeometry->SetResource("tex2D", NULL);
	this->Shader_DeferredGeometry->Apply(0);
}


void DxManager::RenderDeferredSkybox()
{
	if(!this->skybox)
		return;

	//clear and set render target/depth
	this->Dx_DeviceContext->OMSetRenderTargets(this->NrOfRenderTargets, this->Dx_GbufferRTs, this->Dx_DepthStencilView);
	this->Dx_DeviceContext->RSSetViewports(1, &this->Dx_Viewport);
	
	
		
		// Set matrixes
	D3DXMATRIX world, wvp, view, proj;
	view = this->camera->GetViewMatrix();
	proj = this->camera->GetProjectionMatrix();
	world = this->skybox->GetSkyboxWorldMatrix(this->camera->getPosition());
	wvp = world * view * proj;
	
	this->Shader_Skybox->SetMatrix("gWVP", wvp);
	this->Shader_Skybox->SetMatrix("worldMatrix", world);

	MeshStrip* strip = this->skybox->GetStrips()->get(0);

	Object3D* obj = strip->GetRenderObject();
	this->Dx_DeviceContext->IASetPrimitiveTopology(obj->GetTopology());

	obj->GetVertBuff()->Apply();
	this->Shader_Skybox->SetResource("SkyMap", obj->GetTexture());
	obj->GetIndsBuff()->Apply();
	
	this->Shader_Skybox->Apply(0);

	this->Dx_DeviceContext->DrawIndexed(obj->GetIndsBuff()->GetElementCount(), 0, 0);

	// Unbind resources:
	this->Shader_Skybox->SetResource("SkyMap", NULL);
	this->Shader_Skybox->Apply(0);
}

void DxManager::RenderDeferredPerPixel()
{
	//clear and set render target/depth
	this->Dx_DeviceContext->OMSetRenderTargets(1, &this->Dx_RenderTargetView, this->Dx_DepthStencilView);
	this->Dx_DeviceContext->RSSetViewports(1, &this->Dx_Viewport);

	static float ClearColor[4] = {0.5f, 0.71f, 1.0f, 1.0f};
	this->Dx_DeviceContext->ClearRenderTargetView(this->Dx_RenderTargetView, ClearColor);

	this->Dx_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	//DeferredLightning.fx:
	this->Shader_DeferredLightning->SetResource("Texture", this->Dx_GbufferSRVs[0]);
	this->Shader_DeferredLightning->SetResource("NormalAndDepth", this->Dx_GbufferSRVs[1]);
	this->Shader_DeferredLightning->SetResource("Position", this->Dx_GbufferSRVs[2]);
	this->Shader_DeferredLightning->SetResource("Specular", this->Dx_GbufferSRVs[3]);
	D3DXMATRIX vp = this->camera->GetViewMatrix() * this->camera->GetProjectionMatrix();
	this->Shader_DeferredLightning->SetMatrix("CameraVP", vp);
	this->Shader_DeferredLightning->SetFloat4("CameraPosition", D3DXVECTOR4(this->camera->getPosition(), 1));
	//stdafx.fx:
	this->Shader_DeferredLightning->SetFloat("timerMillis", this->TimerAnimation);
	this->Shader_DeferredLightning->SetInt("windowWidth", this->params.windowWidth);
	this->Shader_DeferredLightning->SetInt("windowHeight", this->params.windowHeight);
		
	//ssao.fx:
	this->ssao->PreRender(this->Shader_DeferredLightning, this->params, this->camera);

	//lava.fx:
	this->Shader_DeferredLightning->SetResource("LavaTexture", this->LavaTexture);
	this->Shader_DeferredLightning->SetFloat("outerRadius", this->LavaWavesOuterRadius); 

	this->Shader_DeferredLightning->Apply(0);

	
	this->Dx_DeviceContext->Draw(1, 0);
	
	
	// Unbind resources:
	this->Shader_DeferredLightning->SetResource("Texture", NULL);
	this->Shader_DeferredLightning->SetResource("NormalAndDepth", NULL);
	this->Shader_DeferredLightning->SetResource("Position", NULL);
	this->Shader_DeferredLightning->SetResource("Specular", NULL);
	this->Shader_DeferredLightning->SetResource("LavaTexture", NULL);
	for(int i = 0; i < this->lights.size(); i++)
	{
		this->Shader_DeferredLightning->SetResourceAtIndex(i, "ShadowMap", NULL);
	}

	// Unbind SSAO
	this->ssao->PostRender(this->Shader_DeferredLightning);

	this->Shader_DeferredLightning->Apply(0);
}

void DxManager::RenderInvisibilityEffect() 
{
	HRESULT hr = S_OK;

	//get the surface/texture from the swap chain
	ID3D11Texture2D* backBufferTex = NULL;
	this->Dx_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferTex);
	if(FAILED(hr))
	{
		MaloW::Debug("InvisibilityEffect: Failed to get buffer from swap chain");
		return;
	}

	//get texture description
	D3D11_TEXTURE2D_DESC texDesc;
	backBufferTex->GetDesc(&texDesc);
	
	//change bindflag for use as a shader resource
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE; 

	//create texture
	ID3D11Texture2D* sceneTex = NULL;
	hr = this->Dx_Device->CreateTexture2D(&texDesc, NULL, &sceneTex);	
	if(FAILED(hr))
	{
		MaloW::Debug("InvisibilityEffect: Failed to create texture");
		return;
	}
	
	//get resource from the render target view of the backbuffer
	ID3D11RenderTargetView* backBufferRTV = NULL;
	this->Dx_DeviceContext->OMGetRenderTargets(1, &backBufferRTV, NULL);
	ID3D11Resource* backBufferRTVResource = NULL;
	backBufferRTV->GetResource(&backBufferRTVResource);

	//copy data from the resource to the scene texture
	this->Dx_DeviceContext->CopyResource(sceneTex, backBufferRTVResource);

	//create shader resource view
	ID3D11ShaderResourceView* sceneSRV;
	hr = this->Dx_Device->CreateShaderResourceView(sceneTex, NULL, &sceneSRV);
	if(FAILED(hr))
	{
		MaloW::Debug("InvisibilityEffect: Failed to create shader resource view");
		return;
	}

	//set rendertarget & view port
	this->Dx_DeviceContext->OMSetRenderTargets(1, &this->Dx_RenderTargetView, this->Dx_DepthStencilView);
	this->Dx_DeviceContext->RSSetViewports(1, &this->Dx_Viewport);

	//set shader variables
	this->Shader_InvisibilityEffect->SetResource("sceneTex", sceneSRV);

	//Per frame
	this->Shader_InvisibilityEffect->SetInt("width", this->params.windowWidth); 
	this->Shader_InvisibilityEffect->SetInt("height", this->params.windowHeight); 
	this->Shader_InvisibilityEffect->SetInt("blurSize", 5);

	//this->Shader_InvisibilityEffect->SetBool("ballOwner", true); //**test**
	//this->Shader_InvisibilityEffect->SetInt("teamColor", YELLOW_COLOR); //**test**

	//Invisible(effect) geometry
	D3DXMATRIX wvp;
	for(int i = 0; i < this->objects.size(); i++)
	{
		if(this->objects[i]->IsUsingInvisibility())
		{
			MaloW::Array<MeshStrip*>* strips = this->objects[i]->GetStrips();
		
			//Per object/mesh
			wvp = this->objects[i]->GetWorldMatrix() * this->camera->GetViewMatrix() * this->camera->GetProjectionMatrix();
			this->Shader_InvisibilityEffect->SetMatrix("WVP", wvp);

			//Per strip
			for(int u = 0; u < strips->size(); u++)
			{
				Object3D* obj = strips->get(u)->GetRenderObject();

				//Set texture
				if(ID3D11ShaderResourceView* texture = obj->GetTexture())
				{
					this->Shader_InvisibilityEffect->SetResource("ballTex", texture);
					this->Shader_InvisibilityEffect->SetBool("textured", true);
				}
				else
				{
					this->Shader_InvisibilityEffect->SetBool("textured", false);
				}

				//Set topology
				this->Dx_DeviceContext->IASetPrimitiveTopology(obj->GetTopology());

				//Apply vertex buffer
				Buffer* verts = obj->GetVertBuff();
				if(verts)
					verts->Apply();

				//Apply index buffer
				Buffer* inds = obj->GetIndsBuff();
				if(inds)
					inds->Apply();
			
				//Apply shader
				this->Shader_InvisibilityEffect->Apply(0);

				//Draw
				if(inds)
					this->Dx_DeviceContext->DrawIndexed(inds->GetElementCount(), 0, 0);
				else
					this->Dx_DeviceContext->Draw(verts->GetElementCount(), 0);
			}
		}
	}

	SAFE_RELEASE(backBufferTex);
	SAFE_RELEASE(sceneTex);
	SAFE_RELEASE(backBufferRTV);
	SAFE_RELEASE(backBufferRTVResource);
	SAFE_RELEASE(sceneSRV);

	this->Shader_InvisibilityEffect->SetResource("sceneTex", NULL);
	this->Shader_InvisibilityEffect->SetResource("ballTex", NULL);
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
	this->Shader_DeferredQuad->SetFloat("ScreenWidth", (float)this->params.windowWidth);
	this->Shader_DeferredQuad->SetFloat("ScreenHeight", (float)this->params.windowHeight);
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
	//this->ssao->PreRender(this->Shader_DeferredTexture, this->params, this->camera);
	this->Shader_DeferredTexture->Apply(0);

	
	this->Dx_DeviceContext->Draw(1, 0);
	
	
	// Unbind resources:
	this->Shader_DeferredTexture->SetResource("Texture", NULL);
	this->Shader_DeferredTexture->SetResource("NormalAndDepth", NULL);
	this->Shader_DeferredTexture->SetResource("Position", NULL);
	this->Shader_DeferredTexture->SetResource("Specular", NULL);
	this->Shader_DeferredTexture->SetResource("LightAccu", NULL);

	// Unbind SSAO
	//this->ssao->PostRender(this->Shader_DeferredTexture);

	this->Shader_DeferredTexture->Apply(0);
}