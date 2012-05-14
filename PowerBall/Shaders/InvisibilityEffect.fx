//------------------------------------------------------------------------------------------------------
//	Invisibility effect (**blur/PCF**)
//	Todo: description
//	Requirements:	world positions of pixels**Todo: more**
//------------------------------------------------------------------------------------------------------
#include "stdafx.fx"

//-----------------------------------------------------------------------------------------
//	Global variables (non-numeric values cannot be added to a constantbuffer.)
//-----------------------------------------------------------------------------------------
Texture2D sceneTex;

//-----------------------------------------------------------------------------------------
// Input and Output Structures
//-----------------------------------------------------------------------------------------
cbuffer PerFrame
{
	uint width; //can't use values set in another shader (windowWidth)
	uint height; //can't use values set in another shader (windowHeight)
	uint blurSize; //in pixel width and height
};
cbuffer PerObject
{
	matrix WVP;
};
//**
struct VSIn
{
	float3 Pos : POSITION;
	float2 tex : TEXCOORD;
	float3 norm : NORMAL;
	float3 Color : COLOR;
};
//**
//-----------------------------------------------------------------------------------------
// VertexShader
//-----------------------------------------------------------------------------------------
float4 VSScene(float3 pos : POSITION) : SV_POSITION
{
	return mul(float4(pos, 1.0f), WVP);
}

//-----------------------------------------------------------------------------------------
// PixelShader
//-----------------------------------------------------------------------------------------
float4 PSScene(float4 pos : SV_POSITION) : SV_Target
{	
	float dX = 1.0f / width; 
	float dY = 1.0f / height; 
	float2 center = float2(pos.x * dX, pos.y * dY); //center texture coordinates
	float2 offset = float2(-dX * ((blurSize - 1) * 0.5f), dY * ((blurSize - 1) * 0.5f));
	float4 blurColor = float4(0,0,0,1);
	//sample the surrounding pixels + the center pixel
	for(uint i = 0; i < blurSize; i++)
	{
		for(uint j = 0; j < blurSize; j++)
		{
			//starting position: center
			//offset to start in upper left corner
			//sample the surrounding pixels: + float2(i * dX, j * dY)
			blurColor.xyz += sceneTex.Sample(LinearWrapSampler, center + offset + float2(i * dX, j * dY)).xyz;
		}
	}
	blurColor.xyz /= pow(blurSize, 2);

	//make it slightly more visible by ever so slightly reduce the brightness
	blurColor.xyz -= 0.025f;

	return blurColor;
}


//-----------------------------------------------------------------------------------------
// Technique
//-----------------------------------------------------------------------------------------
technique11 Tech
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_4_0, VSScene()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0, PSScene()));
	    

		SetDepthStencilState(EnableDepth, 0);
	    SetRasterizerState(BackCulling);
    }  
}