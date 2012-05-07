//#include "ParticleShader.h"
//
//
//ParticleShader::ParticleShader()
//	: Shader()
//{
//	this->mFileName = NULL;
//	this->mStreamOutputEffectTechnique = NULL;
//}
//ParticleShader::~ParticleShader()
//{
//	if(this->mFileName) delete this->mFileName;
//}
//
//HRESULT ParticleShader::Init(ID3D10Device* device, string fileName, const D3D10_INPUT_ELEMENT_DESC* inputElementDesc, unsigned int nrOfElements)
//{
//	HRESULT hr = S_OK;
//
//	//this->gDevice = device;
//
//	this->mFileName = new string(fileName);
//
//	ID3DBlob*	pBlobEffect = NULL;
//	ID3DBlob*	pBlobErrors = NULL;
//	
//	DWORD dwShaderFlags = D3D10_SHADER_ENABLE_STRICTNESS; //D3D10_SHADER_ENABLE_BACKWARDS_COMPATIBILITY;
//	
//#if defined(DEBUG) || defined(_DEBUG)
//	dwShaderFlags |= D3D10_SHADER_DEBUG;
//#endif
//
//	//create effect from file
//	hr = D3DX10CreateEffectFromFile(fileName.c_str(),//name of effectfile(.fx)
//									NULL,			//macros
//									NULL,			//include
//									"fx_5_0",		//shader model
//									dwShaderFlags,	//HLSL flag
//									0,				//FX flag
//									device,			//the device		
//									NULL,			//effect pool
//									NULL,			//thread pump
//									&this->mEffect,	//effect
//									&pBlobErrors,			//(pointer to)effect compile errors
//									NULL);			//return value
//	
//	if(FAILED(hr))
//	{
//		char msg[20000];
//		strcpy_s(msg, sizeof(msg), (char*)pBlobErrors->GetBufferPointer());
//		
//		OutputDebugString(msg);
//		MessageBox(0, "Cannot create effect from file!", "Shader error", MB_OK | MB_ICONERROR);
//
//
//		return hr;
//	}
//
//	this->mStreamOutputEffectTechnique	= this->mEffect->GetTechniqueByIndex(0); 
//	this->mDrawEffectTechnique			= this->mEffect->GetTechniqueByIndex(1);
//
//
//	if(inputElementDesc)
//	{
//		D3D10_PASS_DESC passDesc;
//		
//		//**the techniques use the same layout??**
//		
//		//streamout
//		/*
//		this->mStreamOutputEffectTech->GetPassByIndex(0)->GetDesc(&passDesc);
//		hr = this->gDevice->CreateInputLayout(	inputElementDesc,
//												numElements,
//												passDesc.pIAInputSignature,
//												passDesc.IAInputSignatureSize,
//												&this->mInputLayout);
//												*/
//		
//		//draw
//		this->mDrawEffectTechnique->GetPassByIndex(0)->GetDesc(&passDesc);
//		hr = this->gDevice->CreateInputLayout(	inputElementDesc,
//												nrOfElements,
//												passDesc.pIAInputSignature,
//												passDesc.IAInputSignatureSize,
//												&this->mInputLayout);
//		if(FAILED(hr))
//		{
//			MessageBox(0, "Cannot create input layout.", "CreateInputLayout error", MB_OK | MB_ICONERROR);
//			return hr;
//		}
//	}
//
//	return hr;
//}
//
//const string* ParticleShader::GetFileName() const
//{
//	return this->mFileName;
//}
//
//HRESULT ParticleShader::Apply(unsigned int technique, unsigned int pass)
//{
//	//select technique, stream output or draw
//	ID3D10EffectTechnique* tmpTech = NULL;
//	switch(technique)
//	{
//		case 0: tmpTech = this->mDrawEffectTechnique; break;
//		case 1: tmpTech = this->mStreamOutputEffectTechnique; break;
//		default: MessageBox(0, "Technique number out of range", "Particle Shader", MB_ICONWARNING); break;
//	}
//
//	ID3D10EffectPass* effectPass = tmpTech->GetPassByIndex(pass);
//	if(effectPass)
//	{
//		effectPass->Apply(0);
//
//		if(this->mInputLayout) //only set inputlayout to the input assembler once
//		{
//			this->gDevice->IASetInputLayout(this->mInputLayout);
//		}
//
//		return S_OK;
//	}
//	tmpTech = NULL;
//
//	return E_FAIL;
//}
//
//
//ID3D10EffectTechnique* ParticleShader::GetStreamOutputEffectTechnique()
//{
//	return this->mStreamOutputEffectTechnique;
//}
