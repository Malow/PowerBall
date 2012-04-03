//--------------------------------------------------------------------------------------
// Basic.fx
// Direct3D 11 Shader Model 4.0 Demo
// Copyright (c) Stefan Petersson, 2011
//--------------------------------------------------------------------------------------

// Marcus Löwegren

// For textures
Texture2D tex2D;
SamplerState linearSampler
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};

//-----------------------------------------------------------------------------------------
// Input and Output Structures
//-----------------------------------------------------------------------------------------


cbuffer EveryObject
{
	matrix WVP;
	matrix worldMatrix;
};

struct VSIn
{
	float4 Pos : POSITION;
	float2 tex : TEXCOORD;
	float3 norm : NORMAL;
	float4 Color : COLOR;
};

struct GSIn
{
	float4 Pos : POSITION;
};

struct PSSceneIn
{
	float4 Pos : SV_POSITION;
};

//-----------------------------------------------------------------------------------------
// State Structures
//-----------------------------------------------------------------------------------------
RasterizerState NoCulling
{
	CullMode = Back;
};

DepthStencilState EnableDepth
{
    DepthEnable = TRUE;
    DepthWriteMask = ALL;
    DepthFunc = LESS_EQUAL;
};


//-----------------------------------------------------------------------------------------
// VertexShader: VSScene
//-----------------------------------------------------------------------------------------
GSIn VSScene(VSIn input)
{
	GSIn output = (GSIn)0;

	output.Pos = mul(input.Pos, WVP);
	
	return output;
}

// GS
[maxvertexcount(6)]
void GS(triangle GSIn input[3], inout LineStream<PSSceneIn> lineStream)
{
	PSSceneIn output;

	output.Pos = input[0].Pos;
	lineStream.Append(output);
	output.Pos = input[1].Pos;
	lineStream.Append(output);

	output.Pos = input[0].Pos;
	lineStream.Append(output);
	output.Pos = input[2].Pos;
	lineStream.Append(output);

	output.Pos = input[1].Pos;
	lineStream.Append(output);
	output.Pos = input[2].Pos;
	lineStream.Append(output);
}

//-----------------------------------------------------------------------------------------
// PixelShader: PSSceneMain
//-----------------------------------------------------------------------------------------
float4 PSScene(PSSceneIn input) : SV_Target
{	
	return float4(0.9f, 0.9f, 0.9f, 1.0f);
}


//-----------------------------------------------------------------------------------------
// Technique: RenderTextured  
//-----------------------------------------------------------------------------------------
technique11 BasicTech
{
    pass p0
    {
		// Set VS, GS, and PS
        SetVertexShader( CompileShader( vs_4_0, VSScene() ) );
        SetGeometryShader( CompileShader( gs_4_0, GS() ) );
        SetPixelShader( CompileShader( ps_4_0, PSScene() ) );
	    

		SetDepthStencilState( EnableDepth, 0 );
	    SetRasterizerState( NoCulling );
    }  
}