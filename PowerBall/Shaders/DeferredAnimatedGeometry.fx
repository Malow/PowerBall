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


cbuffer EveryStrip
{
	matrix WVP;
	matrix worldMatrix;
	matrix worldMatrixInverseTranspose;
	bool textured;

	float4 AmbientLight;
	float SpecularPower;
	float4 SpecularColor;
	float4 DiffuseColor;
	float t;
};
cbuffer EveryMesh
{
	uint specialColor;
}
cbuffer EveryFrame
{
	float4 CameraPosition;
};

struct VSIn
{
	float4 Pos : POSITION;
	float2 tex : TEXCOORD;
	float3 norm : NORMAL;
	float4 Color : COLOR;

	float4 Pos_Morph : POSITION_MORPH;
	float2 tex_Morph : TEXCOORD_MORPH;
	float3 norm_Morph : NORMAL_MORPH;
	float4 Color_Morph : COLOR_MORPH;
};


struct PSSceneIn
{
	float4 Pos : SV_POSITION;
	float2 tex : TEXCOORD;
	float3 norm : NORMAL;
	float4 Color : COLOR;

	float4 WorldPos : POSITION;
};

struct PSout
{
	float4 Texture : SV_TARGET0;
	float4 NormalAndDepth : SV_TARGET1;
	float4 Position : SV_TARGET2;
	float4 Specular : SV_TARGET3;
};

/*
RTs:
1: Texture XYZ, W Special Color
2: Normal XYZ, W Depth
3: Position XYZ, W unused
4: Specular XYZ, W Specular Power


*/

//-----------------------------------------------------------------------------------------
// State Structures
//-----------------------------------------------------------------------------------------
RasterizerState Culling
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
PSSceneIn VSScene(VSIn input)
{
	input.Color.w = 1.0;
	input.Pos.w = 1.0;

	PSSceneIn output = (PSSceneIn)0;
	output.Pos = mul(lerp(input.Pos, input.Pos_Morph, t), WVP);
	output.WorldPos = mul(lerp(input.Pos, input.Pos_Morph, t), worldMatrix);
	output.tex = lerp(input.tex, input.tex_Morph, t);
	output.norm = normalize(mul(lerp(input.norm, input.norm_Morph, t), (float3x3)worldMatrixInverseTranspose));
	output.Color = lerp(input.Color, input.Color_Morph, t);

	return output;
}

//-----------------------------------------------------------------------------------------
// PixelShader: PSSceneMain
//-----------------------------------------------------------------------------------------
PSout PSScene(PSSceneIn input) : SV_Target
{	
	float4 textureColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
	if(textured)
	{
		textureColor.xyz = tex2D.Sample(linearSampler, input.tex).xyz;
	}
	float4 finalColor = (textureColor + input.Color) * DiffuseColor;

	finalColor.w = (float)specialColor;

	PSout output;
	output.Texture = finalColor;
	output.NormalAndDepth = float4(input.norm.xyz, input.Pos.z / input.Pos.w);		// pos.z / pos.w should work?

	float depth = length(CameraPosition.xyz - input.WorldPos.xyz) / 200.0f;		// Haxfix
	output.NormalAndDepth.w = depth;

	output.Position = input.WorldPos;
	output.Specular = SpecularColor;
	output.Specular.w = SpecularPower;

	return output;
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
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PSScene() ) );
	    

		SetDepthStencilState( EnableDepth, 0 );
	    SetRasterizerState( Culling );
    }  
}