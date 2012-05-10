#include "SSAO.fx"

// Marcus Löwegren

// For textures
Texture2D Texture;
Texture2D NormalAndDepth;
Texture2D Position;
Texture2D Specular;
Texture2D LightAccu;

SamplerState linearSampler
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};

Texture2D ShadowMap[10];
SamplerState shadowMapSampler
{
	Filter = MIN_MAG_MIP_POINT;
	AddressU = Clamp;
	AddressV = Clamp;
};

DepthStencilState DisableDepthWrite
{
    DepthEnable = FALSE;
    DepthWriteMask = ZERO;
};

//-----------------------------------------------------------------------------------------
// Input and Output Structures
//-----------------------------------------------------------------------------------------

struct VSIn
{
	float4 Pos : POSITION;
	float2 tex : TEXCOORD;
	float3 norm : NORMAL;
	float4 Color : COLOR;
};

struct PSSceneIn
{
	float4 Pos : SV_POSITION;
	float2 tex : TEXCOORD;
};

//-----------------------------------------------------------------------------------------
// State Structures
//-----------------------------------------------------------------------------------------
RasterizerState NoCulling
{
	CullMode = NONE;
};

//-----------------------------------------------------------------------------------------
// VertexShader: VSScene
//-----------------------------------------------------------------------------------------
VSIn VSScene(VSIn input)
{
	return input;
}


// GS
[maxvertexcount(4)]
void GS( point VSIn input[1], inout TriangleStream<PSSceneIn> triStream )
{
	PSSceneIn output;

	//bottom left
	output.Pos = float4(-1, -1, 0, 1);
	output.tex = float2(0,1);
	triStream.Append(output);
	
	//bottom right
	output.Pos = float4(1, -1, 0, 1);
	output.tex = float2(1,1);
	triStream.Append(output);

	//top left
	output.Pos = float4(-1, 1, 0, 1);
	output.tex = float2(0,0);
	triStream.Append(output);

	//top right
	output.Pos = float4(1, 1, 0, 1);
	output.tex = float2(1,0);
	triStream.Append(output);
}


//-----------------------------------------------------------------------------------------
// PixelShader: PSSceneMain
//-----------------------------------------------------------------------------------------
float4 PSScene(PSSceneIn input) : SV_Target
{	
	float4 DiffuseColor = Texture.Sample(linearSampler, input.tex);
	float4 Lighting = LightAccu.Sample(linearSampler, input.tex);
	
	DiffuseColor.w = 1.0f;

	float4 AmbientColor = float4(DiffuseColor.xyz * 0.3f, 1.0f);	

	float4 finalColor = float4((AmbientColor.xyz + DiffuseColor.xyz * Lighting.xyz), DiffuseColor.w);

	/*
	// Basic fog:
	finalColor = saturate(finalColor);
	float fogDepth = NormsAndDepth.w;
	if(fogDepth > 0.25f)
	{
		float fogfactor = (fogDepth - 0.25f) * 1.33f;

		finalColor = lerp(finalColor, float4(0.5, 0.5, 0.5, 1.0f), saturate(fogfactor));
	}
	if(fogDepth < -0.5f)
		finalColor = float4(0.5, 0.5, 0.5, 1.0f);
	*/

	/*
	if(finalColor.x < 0.0f)		// Haxfix, want it above but I lose 75% of my FPS then (??!?!? :S:S:S:S:S)
		return DiffuseColor;
		*/

	//finalColor = SSAO(input.Pos, NormalAndDepth);
	//finalColor = float4(Lighting.xyz, 1.0f);
	return saturate(finalColor);
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
	    

		SetDepthStencilState( DisableDepthWrite, 0 );
	    SetRasterizerState( NoCulling );
    }  
}