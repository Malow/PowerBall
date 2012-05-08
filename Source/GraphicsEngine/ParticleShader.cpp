#include "ParticleShader.h"


ParticleShader::ParticleShader()
	: Shader()
{
	this->m_pFileName = NULL;
	this->m_pStreamOutputEffectTechnique = NULL;
}
ParticleShader::~ParticleShader()
{
	if(this->m_pFileName) delete this->m_pFileName;
}

HRESULT ParticleShader::Init(ID3D11Device* device, string fileName, const D3D11_INPUT_ELEMENT_DESC* inputElementDesc, unsigned int nrOfElements)
{
	HRESULT hr = S_OK;

	this->mDevice = device;

	this->m_pFileName = new string(fileName);

	ID3DBlob*	pBlobEffect = NULL;
	ID3DBlob*	pBlobErrors = NULL;
	
	DWORD dwShaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;
	
#if defined(DEBUG) || defined(_DEBUG)
	dwShaderFlags |= D3D10_SHADER_DEBUG;
#endif

	hr = D3DX11CompileFromFile(	fileName.c_str(),
								NULL,
								NULL,
								"",
								"fx_5_0",
								dwShaderFlags,
								NULL,
								NULL,
								&pBlobEffect,
								&pBlobErrors,
								NULL);
	if(FAILED(hr))
	{
		char msg[20000];
		strcpy_s(msg, sizeof(msg), (char*)pBlobErrors->GetBufferPointer());
		
		OutputDebugString(msg);
		MessageBox(0, "Cannot create effect from file!", "Shader error", MB_OK | MB_ICONERROR);

		return hr;
	}
	
	hr = D3DX11CreateEffectFromMemory(	pBlobEffect->GetBufferPointer(),
										pBlobEffect->GetBufferSize(),
										dwShaderFlags,
										device,
										&this->m_pEffect );
	if(FAILED(hr))
	{
		MessageBox(0, "Cannot create effect from memory.", "D3DX11CreateEffectFromMemory error", MB_OK | MB_ICONERROR);
		return hr;
	}


	this->m_pStreamOutputEffectTechnique	= this->m_pEffect->GetTechniqueByIndex(0); 
	this->m_pTechnique						= this->m_pEffect->GetTechniqueByIndex(1);


	if(inputElementDesc)
	{
		D3DX11_PASS_DESC PassDesc;
		this->m_pTechnique->GetPassByIndex(0)->GetDesc(&PassDesc);
		hr = this->mDevice->CreateInputLayout(	inputElementDesc,
												nrOfElements,
												PassDesc.pIAInputSignature,
												PassDesc.IAInputSignatureSize,
												&this->m_pInputLayout);
		if(FAILED(hr))
		{
			MessageBox(0, "Cannot create input layout.", "CreateInputLayout error", MB_OK | MB_ICONERROR);
			return hr;
		}
	}

	return hr;
}

const string* ParticleShader::GetFileName() const
{
	return this->m_pFileName;
}

HRESULT ParticleShader::Apply(unsigned int technique, unsigned int pass)
{
	//select technique, stream output or draw
	ID3DX11EffectTechnique* tmpTech = NULL;
	switch(technique)
	{
		case 0: tmpTech = this->m_pTechnique; break;
		case 1: tmpTech = this->m_pStreamOutputEffectTechnique; break;
		default: MessageBox(0, "Technique number out of range", "Particle Shader", MB_ICONWARNING); break;
	}

	ID3DX11EffectPass* effectPass = tmpTech->GetPassByIndex(pass);
	if(effectPass)
	{
		effectPass->Apply(0, this->mImmediateContext);

		if(this->m_pInputLayout) //only set inputlayout to the input assembler once
		{
			this->mImmediateContext->IASetInputLayout(this->m_pInputLayout);
		}

		return S_OK;
	}
	tmpTech = NULL;

	return E_FAIL;
}


ID3D11InputLayout* ParticleShader::GetInputLayout()
{
	return this->m_pInputLayout;
}
ID3DX11EffectTechnique* ParticleShader::GetDrawEffectTechnique()
{
	return this->m_pTechnique;
}
ID3DX11EffectTechnique* ParticleShader::GetStreamOutputEffectTechnique()
{
	return this->m_pStreamOutputEffectTechnique;
}
