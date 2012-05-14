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
Texture2D ballTex;

//-----------------------------------------------------------------------------------------
// Input and Output Structures
//-----------------------------------------------------------------------------------------
cbuffer PerFrame
{
	uint width; //can't use values set in another shader (windowWidth)
	uint height; //can't use values set in another shader (windowHeight)
	uint blurSize; //in pixel width and height
};
cbuffer PerMesh
{
	matrix WVP;
	bool ballOwner;
	uint teamColor;
};
cbuffer PerStrip
{
	bool textured;
};
//**
struct VSIn
{
	float3 pos : POSITION;
	float2 tex : TEXCOORD;
};
struct PSIn
{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD0;
};
//**
//-----------------------------------------------------------------------------------------
// VertexShader
//-----------------------------------------------------------------------------------------
//float4 VSScene(float3 pos : POSITION) : SV_POSITION
PSIn VSScene(VSIn input)
{
	//return mul(float4(pos, 1.0f), WVP);
	PSIn output;
	output.pos = mul(float4(input.pos, 1.0f), WVP);
	output.tex = input.tex;
	
	return output;
}

//-----------------------------------------------------------------------------------------
// PixelShader
//-----------------------------------------------------------------------------------------
//float4 PSScene(float4 pos : SV_POSITION) : SV_Target
float4 PSScene(PSIn input) : SV_Target
{	
	float dX = 1.0f / width; 
	float dY = 1.0f / height; 
	//float2 center = float2(pos.x * dX, pos.y * dY); //center texture coordinates
	float2 center = float2(input.pos.x * dX, input.pos.y * dY); //center texture coordinates
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

	if(ballOwner)
	{
		if(teamColor != 0)
		{
			switch(teamColor)
			{
				case 1: blurColor += WHITE; break;
				case 2: blurColor += BLACK; break;
				case 3: blurColor += RED; break;
				case 4: blurColor += GREEN; break;
				case 5: blurColor += BLUE; break;
				case 6: blurColor += YELLOW; break;
				case 7: blurColor += CYAN; break;
				case 8: blurColor += MAGENTA; break;
				blurColor.xyz *= 0.5f;
			}
		}
		if(textured)
		{
			blurColor.xyz += ballTex.Sample(LinearWrapSampler, input.tex).xyz;
			blurColor.xyz *= 0.5f;
		}
	}

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