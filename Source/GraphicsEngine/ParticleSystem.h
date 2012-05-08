#pragma once

#include "stdafx.h"
#include "ParticleShader.h"

#include "Shader.h"
#include "GraphicsEngineParameters.h"
#include "Camera.h"


struct Particle //klass?**
{
	unsigned int type;
	float age;
	D3DXVECTOR2 size;
	D3DXVECTOR3 vel;
	D3DXVECTOR3 pos;
};

class ParticleSystem
{
	private:
		ID3D11Device*			gDevice;
		ID3D11DeviceContext*	gDeviceContext;
		ParticleShader*			gParticleShader;

		int							mCapacity;
		int							mNrOfTextures;
		bool						mFirstRun;
		float						mDeltaTime; 
		float						mGameTime; //**
		float						mAge;
		
		D3DXVECTOR3					mEyePosW; 
		D3DXVECTOR3					mEmitterPosW;
		D3DXVECTOR3					mEmitterDirW;
		
		//**stefans bufferklass??**
		ID3D11Buffer*				mInitVB; //contains only an emitter particle, drawn once (unless system reset)
		ID3D11Buffer*				mDrawVB; //som front buffer - innehåller partiklarna som ska ritas**
		ID3D11Buffer*				mStreamOutVB; //som backbuffer - innehåller partiklarna som håller på att göras**
		ID3D11ShaderResourceView*	mRndTexSRV; //för slumpade 3d vektorer**
		ID3D11ShaderResourceView*	mTexArraySRV; //för flera texturer**

	private:
		HRESULT CreateRndTex(int size);								//**texturemanager - return texArraySRV;** 
		HRESULT CreateTexArray(const vector<string>& filenames);	//**texturemanager**
		HRESULT BuildBuffers();

	public:
		ParticleSystem(int capacity); 
		ParticleSystem(int capacity, D3DXVECTOR3 emitterPos, D3DXVECTOR3 emitterDir); 
		virtual ~ParticleSystem();

		float GetAge() const;

		/*! Sets the eye (camera) position in world space. */
		void SetEyePos(const D3DXVECTOR3 eyePos);
		/*! Sets the emitter position in world space. */
		void SetEmitterPos(const D3DXVECTOR3 emitterPos);
		/*! Sets the emitter direction-vector in world space. */
		void SetEmitterDir(const D3DXVECTOR3 emitterDir);

		HRESULT Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ParticleShader* particleShader, const vector<string>& fileNames); 
		
		
		void Reset();
		void Move(float deltaTime);
		void Update(float deltaTime, float gameTime);
		
		/*! Sets all Particle system settings to the shader it's using. */
		void PreRender(GraphicsEngineParams engParams, Camera* cam);

		/*! Cleans up the textures used. */
		void PostRender();
};