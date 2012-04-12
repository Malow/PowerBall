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

Texture2D ShadowMap[10];
SamplerState shadowMapSampler
{
	Filter = MIN_MAG_MIP_POINT;
	AddressU = Clamp;
	AddressV = Clamp;
};

//-----------------------------------------------------------------------------------------
// Input and Output Structures
//-----------------------------------------------------------------------------------------

struct Light
{
	float4 LightPosition;
	float4 LightColor;
	float LightIntensity;
	matrix LightViewProj;
};

cbuffer EveryObject
{
	matrix WVP;
	matrix worldMatrix;
	matrix worldMatrixInverseTranspose;
	bool textured;

	float4 AmbientLight;
	float SpecularPower;
	float4 SpecularColor;
	float4 DiffuseColor;
};

cbuffer EveryFrame
{
	float4 CameraPosition;

	float NrOfLights;
	Light lights[10];
	float SMAP_DX;
	//float PCF_SIZE;
	float PCF_SIZE_SQUARED;
};

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
	float3 norm : NORMAL;
	float4 Color : COLOR;

	float3 WorldPos : POSITION;
	float4 posLightH[10] : TEXCOORD1;
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
PSSceneIn VSScene(VSIn input)
{
	input.Color.w = 1.0;
	input.Pos.w = 1.0;

	PSSceneIn output = (PSSceneIn)0;
	output.Pos = mul(input.Pos, WVP);
	output.WorldPos = mul(input.Pos, worldMatrix).xyz;
	output.tex = input.tex;
	output.norm = normalize(mul(input.norm, (float3x3)worldMatrixInverseTranspose));
	output.Color = input.Color;
	for(int i = 0; i < NrOfLights; i++)
	{
		output.posLightH[i] = mul(input.Pos, mul(worldMatrix, lights[i].LightViewProj));
	}
	return output;
}

//-----------------------------------------------------------------------------------------
// PixelShader: PSSceneMain
//-----------------------------------------------------------------------------------------
float4 PSScene(PSSceneIn input) : SV_Target
{	
	float PCF_SIZE = 3.0f;								////// Not able to move this to cbuffer, why?
	float4 textureColor = float4(0.0, 0.0, 0.0, 1.0);
	if(textured)
	{
		textureColor = tex2D.Sample(linearSampler, input.tex);
	}

	float diffuseLighting = 0.0f;
	float specLighting = 0.0f;
	
	for(int i = 0; i < NrOfLights; i++)
	
	{
		float3 LightDirection = input.WorldPos - lights[i].LightPosition.xyz;
		float DistanceToLight = length(LightDirection);
		LightDirection = normalize(LightDirection);

		// Diff light
		float difflight = saturate(dot(input.norm, -LightDirection));

		// Spec Light
		float3 h = normalize(normalize(CameraPosition.xyz - input.WorldPos.xyz) - LightDirection);
		float speclight = pow(saturate(dot(h, input.norm)), SpecularPower);
		

		
		// Shadow Mappings
		// Project the texture coords and scale/offset to [0, 1].
		input.posLightH[i].xy /= input.posLightH[i].w;

		//Compute shadow map tex coord
		float2 smTex = float2(0.5f*input.posLightH[i].x, -0.5f*input.posLightH[i].y) + 0.5f;

		// Compute pixel depth for shadowing.
		float depth = input.posLightH[i].z / input.posLightH[i].w;

		float SHADOW_EPSILON = 0.01f;								////////////// PUT THIS WHERE?
		
		// PCF
		float shadow = 0.0f;
		if(smTex.x < 0 || smTex.x > 1 || smTex.y < 0 || smTex.y > 1)
			shadow = 1.0f;
		else if(depth > 1.0f)
			shadow = 1.0f;
		else
		{
			[unroll] for(float s = 0; s < PCF_SIZE; s++)
			{
				[unroll] for(float q = 0; q < PCF_SIZE; q++)
				{
					shadow += (ShadowMap[i].Sample(shadowMapSampler, smTex + float2(SMAP_DX * (s - PCF_SIZE/2) , SMAP_DX * (q - PCF_SIZE/2))).r + SHADOW_EPSILON < depth) ? 0.0f : 1.0f;
				}
			}
			shadow /= PCF_SIZE_SQUARED;
		}


		// Fall off test 3
		float coef = (lights[i].LightIntensity / 1000.0f) + (DistanceToLight * DistanceToLight) / (lights[i].LightIntensity * lights[i].LightIntensity);

		difflight /= coef;
		if(coef > 10.0f)
			speclight /= coef / 10.0f;

		difflight *= shadow;
		speclight *= shadow;

		diffuseLighting += difflight;
		specLighting += speclight;



		/*
		// Fall off test 2
		if(DistanceToLight > lights[i].LightIntensity * 1.1f)
		{
			DistanceToLight -= lights[i].LightIntensity;
			DistanceToLight /= lights[i].LightIntensity * 0.3f;
			if(DistanceToLight < 1.0f)
				DistanceToLight = 1.0f;

			difflight /= DistanceToLight;
			speclight /= DistanceToLight;

		}

		difflight *= shadow;
		speclight *= shadow;

		diffuseLighting += difflight;
		specLighting += speclight;
		*/

		/*
		// Fall off test 1:
		difflight *= lights[i].LightIntensity / (DistanceToLight * 2);
		speclight *= lights[i].LightIntensity / (DistanceToLight * 2);
		
		//shadow *= 1 / (DistanceToLight / 10);
		
		difflight *= shadow;
		speclight *= shadow;

		diffuseLighting += difflight;
		specLighting += speclight;
		*/


		/*
		//Without falloff and intensity
		difflight *= shadow;
		speclight *= shadow;

		diffuseLighting += difflight;
		specLighting += speclight;
		*/
	}
	
	diffuseLighting = saturate(diffuseLighting / NrOfLights);
	specLighting = saturate(specLighting / NrOfLights);

	float4 finalColor = float4((AmbientLight.xyz * (textureColor.xyz + input.Color.xyz)) + ((textureColor.xyz + input.Color.xyz) * DiffuseColor.xyz * diffuseLighting) + (SpecularColor.xyz * specLighting), textureColor.w + input.Color.w);
	
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
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PSScene() ) );
	    

		SetDepthStencilState( EnableDepth, 0 );
	    SetRasterizerState( NoCulling );
    }  
}