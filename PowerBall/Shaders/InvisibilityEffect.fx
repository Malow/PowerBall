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
cbuffer PerObject
{
	matrix WVP;
};
//***
struct VSIn
{
	float3 Pos : POSITION;
	float2 tex : TEXCOORD;
	float3 norm : NORMAL;
	float3 Color : COLOR;
};
//-----------------------------------------------------------------------------------------
// VertexShader
//-----------------------------------------------------------------------------------------
float4 VSScene(float3 pos : POSITION) : SV_POSITION
//float4 VSScene(VSIn input) : SV_POSITION
{
	return mul(float4(pos, 1.0f), WVP);

	//input.Pos.w = 1.0f;
	//return mul(float4(input.Pos, 1.0f), WVP);
}

//-----------------------------------------------------------------------------------------
// PixelShader
//-----------------------------------------------------------------------------------------
float4 PSScene(float4 pos : SV_POSITION) : SV_Target
{	
	float4 test = float4(0,0,0,1);
	float2 texCoords = float2(pos.x / windowWidth, pos.y / windowHeight);
	texCoords.y *= -1;
	test.xyz = sceneTex.Sample(LinearWrapSampler, texCoords).xyz;

	float deltaX = 1.0f / windowWidth;
	float deltaY = 1.0f / windowHeight;
	float PCF_SIZE = 3.0f;
	float4 blurColor = float4(1,1,1,1);
	for(float s = 0; s < PCF_SIZE; s++)
	{
		for(float q = 0; q < PCF_SIZE; q++)
		{
			blurColor += sceneTex.Sample(PointClampSampler, texCoords + float2(deltaX * (s - PCF_SIZE / 2) , deltaY * (q - PCF_SIZE / 2)));
		}
	}
	blurColor *= pow(PCF_SIZE, 2.0f);

	/*		// To check that coords really are in ScreenSpace so that the texCoords above works.
	if(pos.x > 400 && pos.x < 800 && pos.y > 200 && pos.y < 900)
		test = float4(1,0,0,1);
		*/
	
	return test;//+ float4(0,1,0,1);
	//return float4(0.5f, 1.0f, 0.5f, 1.0f);
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