#pragma once

#include "stdafx.h"
#include "ParticleShader.h"

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
		//ParticleShader*			gParticleShader;

		int							mCapacity;
		int							mNrOfTextures;
		bool						mFirstRun;
		float						mDeltaTime; //**
		float						mGameTime; //**
		float						mAge;
		//all in world space **4d?**
		D3DXVECTOR3					mEyePos; //**
		D3DXVECTOR3					mEmitterPos;
		D3DXVECTOR3					mEmitterDir;
		
		//**stefans bufferklass??**
		ID3D11Buffer*				mInitVB; //**contains only and emitter particle, drawn once (unless system reset)
		ID3D11Buffer*				mDrawVB; //som front buffer - innehåller partiklarna som ska ritas**
		ID3D11Buffer*				mStreamOutVB; //som backbuffer - **uinnehå¨ller partiklarna som håller på att göras? ** **
		ID3D11ShaderResourceView*	mRndTexSRV; //för slumpade 3d vektorer
		ID3D11ShaderResourceView*	mTexArraySRV; //**för flera texturer

	private:
		ParticleSystem(const ParticleSystem &origObj);
		ParticleSystem& operator=(const ParticleSystem &origObj);

		HRESULT CreateRndTex(int size); //**texturemanager** 
		HRESULT CreateTexArray(const vector<string>& filenames); //**texturemanager**
		HRESULT BuildBuffers();

	public:
		ParticleSystem(int capacity); 
		ParticleSystem(int capacity, D3DXVECTOR3 emitterPos, D3DXVECTOR3 emitterDir); 
		virtual ~ParticleSystem();

		//***använda alla funktioner - i particlesystemhandler***
		//int GetCapacity() const;
		//bool FirstRun() const;
		//float GetGameTime() const; //***
		//float GetTimeStep();
		float GetAge() const;
		//D3DXVECTOR3 GetEyePos() const; //**
		//D3DXVECTOR3 GetEmitterPos() const; 
		//D3DXVECTOR3 GetEmitterDir() const; 

		void SetEyePos(const D3DXVECTOR3& eyePos);
		void SetEmitterPos(const D3DXVECTOR3& emitterPos);
		void SetEmitterDir(const D3DXVECTOR3& emitterDir);

		HRESULT Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ParticleShader* particleShader, const vector<string>& fileNames); 
		
		
		void Reset();
		void Move(float deltaTime);
		void Update(float deltaTime, float gameTime);
		
		///void Draw(Camera* camera); //**integrera**
};