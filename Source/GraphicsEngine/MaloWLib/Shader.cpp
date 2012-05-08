#include "Shader.h"

Shader::Shader()
{
	m_pEffect = NULL;
	m_pTechnique = NULL;
	m_pInputLayout = NULL;
}

Shader::~Shader()
{
	SAFE_RELEASE(m_pInputLayout);
	//SAFE_RELEASE(m_pEffect);

	if(m_pEffect)
	{
		while(m_pEffect->Release());
		m_pEffect = NULL;
	}
}

HRESULT Shader::Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename,
		const D3D11_INPUT_ELEMENT_DESC* inputElementDesc, unsigned int numElements)
{
	mImmediateContext = deviceContext;
	mDevice = device;

	HRESULT hr = S_OK;

	ID3DBlob*	pBlobEffect = NULL;
	ID3DBlob*	pBlobErrors = NULL;
	
	DWORD dwShaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;

#if defined(DEBUG) || defined(_DEBUG)
	dwShaderFlags |= D3D10_SHADER_DEBUG;
#endif

	hr = D3DX11CompileFromFile(
		filename,
		NULL,
		NULL,
		"",
		"fx_5_0",
		dwShaderFlags,
		NULL,
		NULL,
		&pBlobEffect,
		&pBlobErrors,
		NULL
		);

	if( FAILED(hr) )
	{
		char msg[20000];
		strcpy_s(msg, sizeof(msg), (char*)pBlobErrors->GetBufferPointer());
		OutputDebugString(msg);
		MessageBox(GetDesktopWindow(), msg, "Effect compilation error", MB_OK | MB_ICONERROR);
		return hr;
	}

	if(FAILED(hr = D3DX11CreateEffectFromMemory(
		pBlobEffect->GetBufferPointer(),
		pBlobEffect->GetBufferSize(),
		dwShaderFlags,
		mDevice,
		&m_pEffect
		)))
	{
		MessageBox(0, "Cannot create effect from memory.", "D3DX11CreateEffectFromMemory error", MB_OK | MB_ICONERROR);
		return hr;
	}


	m_pTechnique = m_pEffect->GetTechniqueByIndex(0);


	if(inputElementDesc)
	{
		D3DX11_PASS_DESC PassDesc;
		m_pTechnique->GetPassByIndex(0)->GetDesc(&PassDesc);
		if(FAILED(hr = mDevice->CreateInputLayout(
			inputElementDesc,
			numElements,
			PassDesc.pIAInputSignature,
			PassDesc.IAInputSignatureSize,
			&m_pInputLayout
			)))
		{
			MessageBox(0, "Cannot create input layout.", "CreateInputLayout error", MB_OK | MB_ICONERROR);
			return hr;
		}
	}

	return hr;
}

HRESULT Shader::Apply(unsigned int pass)
{
	ID3DX11EffectPass* p = m_pTechnique->GetPassByIndex(pass);
	if(p)
	{
		p->Apply(0, mImmediateContext);

		if(m_pInputLayout)
		{
			mImmediateContext->IASetInputLayout(m_pInputLayout);
		}
		return S_OK;
	}

	return E_FAIL;
}

void Shader::SetFloat(char* variable, float value)
{
	m_pEffect->GetVariableByName(variable)->AsScalar()->SetFloat(value);
}

void Shader::SetMatrix(char* variable, D3DXMATRIX& mat)
{
	m_pEffect->GetVariableByName(variable)->AsMatrix()->SetMatrix((float*)&mat);
}

void Shader::SetFloat4(char* variable, D3DXVECTOR4 value)
{
	m_pEffect->GetVariableByName(variable)->AsVector()->SetFloatVector((float*)&value);
}

void Shader::SetResource(char* variable, ID3D11ShaderResourceView* value)
{
	m_pEffect->GetVariableByName(variable)->AsShaderResource()->SetResource(value);
}

void Shader::SetUnorderedAccessView(char* variable, ID3D11UnorderedAccessView* value)
{
	m_pEffect->GetVariableByName(variable)->AsUnorderedAccessView()->SetUnorderedAccessView(value);
}

void Shader::SetInt(char* variable, int value)
{
	m_pEffect->GetVariableByName(variable)->AsScalar()->SetInt(value);
}

void Shader::SetBool(char* variable, bool value)
{
	m_pEffect->GetVariableByName(variable)->AsScalar()->SetBool(value);
}

void Shader::SetRawData(char* variable, void* data, size_t size)
{
	m_pEffect->GetVariableByName(variable)->SetRawValue(data, 0, (UINT)size);
}


// Written by MaloW
void Shader::SetFloatAtIndex(int index, char* variable, float value)
{
	m_pEffect->GetVariableByName(variable)->GetElement(index)->AsScalar()->SetFloat(value);
}

void Shader::SetStructMemberAsFloat4(char* structVariable, char* memberVariable, D3DXVECTOR4 value)
{
	m_pEffect->GetVariableByName(structVariable)->GetMemberByName(memberVariable)->AsVector()->SetFloatVector((float*)&value);
}

void Shader::SetStructMemberAtIndexAsFloat4(int index, char* structVariable, char* memberVariable, D3DXVECTOR4 value)
{
	m_pEffect->GetVariableByName(structVariable)->GetElement(index)->GetMemberByName(memberVariable)->AsVector()->SetFloatVector((float*)&value);
}

void Shader::SetStructMemberAtIndexAsFloat(int index, char* structVariable, char* memberVariable, float value)
{
	m_pEffect->GetVariableByName(structVariable)->GetElement(index)->GetMemberByName(memberVariable)->AsScalar()->SetFloat(value);
}

void Shader::SetResourceAtIndex(int index, char* variable, ID3D11ShaderResourceView* value)
{
	m_pEffect->GetVariableByName(variable)->GetElement(index)->AsShaderResource()->SetResource(value);
}

void Shader::SetMatrixAtIndex(int index, char* variable, D3DXMATRIX& mat)
{
	m_pEffect->GetVariableByName(variable)->GetElement(index)->AsMatrix()->SetMatrix((float*)&mat);
}

void Shader::SetStructMemberAtIndexAsMatrix(int index, char* structVariable, char* memberVariable, D3DXMATRIX& mat)
{
	m_pEffect->GetVariableByName(structVariable)->GetElement(index)->GetMemberByName(memberVariable)->AsMatrix()->SetMatrix((float*)&mat);
}



void Shader::SetFloat3(char* variable, D3DXVECTOR3 value)
{
	m_pEffect->GetVariableByName(variable)->AsVector()->SetFloatVector((float*)&value);
}
void Shader::SetFloatVectorArray(char* variable, D3DXVECTOR3* values, unsigned int nrOfElements)
{
	m_pEffect->GetVariableByName(variable)->AsVector()->SetFloatVectorArray((const float*)values, 0, nrOfElements);
}