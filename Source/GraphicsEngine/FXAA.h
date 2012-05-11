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

		bool mAppliedPresetToShader;
		int mPreset; /*! range [0,4]. 1 is lowest quality and highest performance. Default is 0. (FXAA turned off). */

	public:
		FXAA();
		virtual ~FXAA();
		void Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext, IDXGISwapChain* swapChain);

		int GetPreset() const;

		/*! Set preset. Range is 0-4. If out of range preset is set to 0. (No FXAA). */
		void SetPreset(unsigned int preset);

		/*! Sets all FXAA settings to the shader. */
		void PreRender(Shader* shader);

		/*! Cleans up the scene texture. */
		void PostRender(Shader* shader);
};