#include "Buffer.h"

Buffer::Buffer()
{
	mBuffer = NULL;
}

Buffer::~Buffer()
{
	SAFE_RELEASE(mBuffer);
}

HRESULT Buffer::Apply(UINT32 misc)
{
	HRESULT hr = S_OK;

	switch(mType)
	{
	case VERTEX_BUFFER:
		{
			UINT32 vertexSize = mElementSize;
			UINT32 offset = 0;
			mImmediateContext->IASetVertexBuffers(misc, 1, &mBuffer, &vertexSize, &offset );
		}
		break;
	case INDEX_BUFFER:
		{
			mImmediateContext->IASetIndexBuffer(mBuffer, DXGI_FORMAT_R32_UINT, 0);
		}
		break;
	case CONSTANT_BUFFER_VS:
		{
			mImmediateContext->VSSetConstantBuffers(misc, 1, &mBuffer);
		}
		break;
	case CONSTANT_BUFFER_GS:
		{
			mImmediateContext->GSSetConstantBuffers(misc, 1, &mBuffer);
		}
		break;
	case CONSTANT_BUFFER_PS:
		{
			mImmediateContext->PSSetConstantBuffers(misc, 1, &mBuffer);
		}
		break;
	default:
		hr = E_FAIL;
		break;
	};

	return hr;
}

HRESULT Buffer::Init(ID3D11Device* device,ID3D11DeviceContext* immediateContext,
		BUFFER_INIT_DESC& initDesc)
{
	mDevice = device;
	mImmediateContext = immediateContext;
	
	D3D11_BUFFER_DESC bufferDesc;

	mType = initDesc.Type;
	switch(mType)
	{
		case VERTEX_BUFFER:
			{
				bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

				if(initDesc.Usage == BUFFER_STREAM_OUT_TARGET)
					bufferDesc.BindFlags |= D3D11_BIND_STREAM_OUTPUT;
			}
			break;
		case INDEX_BUFFER:
			{
				bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			}
			break;
		case CONSTANT_BUFFER_VS:
		case CONSTANT_BUFFER_GS:
		case CONSTANT_BUFFER_PS:
			{
				bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			}
			break;
		default:
			return E_FAIL;
			break;
	};

	mUsage = initDesc.Usage;
	mElementSize = initDesc.ElementSize;
	mElementCount = initDesc.NumElements;

	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;

	if(mUsage == BUFFER_CPU_READ)
	{
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.CPUAccessFlags |= D3D11_CPU_ACCESS_READ;
	}
	else if(mUsage == BUFFER_CPU_WRITE)
	{
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.CPUAccessFlags |= D3D11_CPU_ACCESS_WRITE;
	}
	else if(mUsage == BUFFER_CPU_WRITE_DISCARD)
	{
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.CPUAccessFlags |= D3D11_CPU_ACCESS_WRITE;
	}

	//Desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.MiscFlags = 0;
	bufferDesc.ByteWidth = initDesc.NumElements * initDesc.ElementSize;

	//set at least 16 bytes
	if(bufferDesc.ByteWidth < 16)
		bufferDesc.ByteWidth = 16;

	HRESULT hr = S_OK;
	if(initDesc.InitData)
	{
		D3D11_SUBRESOURCE_DATA data;
		data.pSysMem = initDesc.InitData;
		hr = mDevice->CreateBuffer(&bufferDesc, &data, &mBuffer);
	}
	else
	{
		hr = mDevice->CreateBuffer(&bufferDesc, NULL, &mBuffer);
	}

	if(FAILED(hr))
	{
		MessageBox(NULL, "Unable to create buffer.", "Slenda Error", MB_ICONERROR | MB_OK);
	}

	return hr;
}

void* Buffer::Map()
{
	void* ret = NULL;
	if(mUsage == BUFFER_CPU_WRITE || mUsage == BUFFER_CPU_READ || mUsage == BUFFER_CPU_WRITE_DISCARD)
	{
		D3D11_MAPPED_SUBRESOURCE MappedResource;
		UINT32 mapType = 0;

		if(mUsage == BUFFER_CPU_READ)	mapType = D3D11_MAP_READ;
		else if(mUsage == BUFFER_CPU_WRITE) mapType = D3D11_MAP_WRITE;
		else if(mUsage == BUFFER_CPU_WRITE_DISCARD)	mapType = D3D11_MAP_WRITE_DISCARD;

		HRESULT hr = S_OK;
		if(FAILED(hr = mImmediateContext->Map(
			mBuffer,
			0,
			(D3D11_MAP)mapType,
			0,
			&MappedResource)))
		{
			ret = NULL;
		}
		else
		{
			ret = MappedResource.pData;
		}
	}

	return ret;

}

void Buffer::Unmap()
{
	mImmediateContext->Unmap( mBuffer, 0 );
}

ID3D11Buffer* Buffer::GetBufferPointer()
{
	return mBuffer;
}

UINT32 Buffer::GetVertexSize()
{
	return mElementSize;
}

UINT32 Buffer::GetElementCount()
{
	return mElementCount;
}