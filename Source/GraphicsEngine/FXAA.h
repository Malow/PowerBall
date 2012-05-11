#pragma once

/*!
	Class for using fast-approximate anti-aliasing (FXAA)
*/

#include "Shader.h"
#include "GraphicsEngineParameters.h"
#include "Camera.h"

class FXAA
{
	private:		
		ID3D11Device*			gDevice;
		ID3D11DeviceContext*	gDeviceContext;
		IDXGISwapChain*			gSwapChain;

		int mPreviousPreset;

	public:
		FXAA();
		virtual ~FXAA();
		void Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext, IDXGISwapChain* swapChain);

		/*! Sets all FXAA settings to the shader. Preset range is 0-4. If out of range preset is set to 0. (No FXAA). */
		void PreRender(Shader* shader, unsigned int preset);

		/*! Cleans up the scene texture. */
		void PostRender(Shader* shader);
};