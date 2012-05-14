#include "DxManager.h"

void DxManager::RenderAntiAliasing()
{
	if(!this->fxaa) return;
	if(!this->params.FXAAQuality) return;

	//set render target & depth stencil and viewport
	this->Dx_DeviceContext->OMSetRenderTargets(1, &this->Dx_RenderTargetView, this->Dx_DepthStencilView);
	this->Dx_DeviceContext->RSSetViewports(1, &this->Dx_Viewport);

	//set input assembler params
	this->Dx_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//set shader variables & apply
	this->fxaa->PreRender(this->Shader_Fxaa, this->params.FXAAQuality);

	//draw quad (triangle strip)
	this->Dx_DeviceContext->Draw(4, 0);

	//unbind texture(s) (scene texture) used
	this->fxaa->PostRender(this->Shader_Fxaa);
}