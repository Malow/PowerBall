#pragma once

#include "ParticleSystem.h"
#include "ParticleShader.h"

class ParticleSystemHandler
{
	private:
		ID3D11Device*			gDevice;
		ID3D11DeviceContext*	gDeviceContext;
		
		int					mNrOfShaders;
		int					mShaderCapacity;
		ParticleShader**	mShaders;
		string**			mShaderNames;

		int					mNrOfSystems;
		int					mSystemCapacity;
		ParticleSystem**	mSystems;
		string**			mSystemNames;

	private:
		template<typename T>
		T** Expand(T** arr, int& capacity)
		{
			T** temp = new T*[capacity + capacity];
			for(int i = 0; i < capacity; i++)
			{
				temp[i] = arr[i];
			}
			delete arr;
			return temp;
		}

	public:
		ParticleSystemHandler();
		virtual ~ParticleSystemHandler();
		void Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext);

		int				GetNrOfParticleShaders() const;
		int				GetNrOfParticleSystems() const;
		const string*	GetParticleShaderName(int index) const;
		const string*	GetParticleSystemName(int index) const;

		bool AddParticleShader(	string particleShaderName, 
								string fileName, 
								const D3D11_INPUT_ELEMENT_DESC* inputElementDesc, 
								unsigned int nrOfElements);
		bool AddParticleSystem(	string particleShaderName,
								string particleSystemName, 
								int particleCapacity,
								const vector<string>& fileNames);
		bool AddParticleSystem(	string particleShaderName,
								string particleSystemName, 
								int particleCapacity,
								D3DXVECTOR3 emitterPos, 
								D3DXVECTOR3 emitterDir, 
								const vector<string>& fileNames);
		void MoveParticleSystem(string name, float deltaTime); 
		void MoveParticleSystemTo(string name, D3DXVECTOR3 pos);
		void Update(float deltaTime, float gameTime, D3DXVECTOR3 camPos);
		
		void PreRender(GraphicsEngineParams engParams, Camera* cam);
		void PostRender();
};