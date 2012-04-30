
#include "stdafx.fx"
#include "SSAO.fx"
#include "Lava.fx"

// Marcus Löwegren

// For textures
Texture2D Texture;
Texture2D NormalAndDepth;
Texture2D Position;
Texture2D Specular;
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
BlendState SrcAlphaBlendingAdd 
{ 
	BlendEnable[0] = TRUE; 
	SrcBlend = SRC_ALPHA; 
	DestBlend = INV_SRC_ALPHA; 
	BlendOp = ADD; 
	SrcBlendAlpha = ZERO; 
	DestBlendAlpha = ZERO; 
	BlendOpAlpha = ADD; 
	RenderTargetWriteMask[0] = 0x0F; 
}; 
//-----------------------------------------------------------------------------------------
// Input and Output Structures
//-----------------------------------------------------------------------------------------
cbuffer ef
{
	float SMAP_DX;
	float PCF_SIZE;
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
	float4 NormsAndDepth = NormalAndDepth.Sample(linearSampler, input.tex);
	
	float4 WorldPos = Position.Sample(linearSampler, input.tex);

	//DiffuseColor.w = 1.0f; **

	float4 AmbientLight = float4(DiffuseColor.xyz * 0.5f, 1.0f);
	float SpecularPower = Specular.Sample(linearSampler, input.tex).w;
	float4 SpecularColor = float4(Specular.Sample(linearSampler, input.tex).xyz, 1.0f);

	float diffuseLighting = 0.0f;
	float specLighting = 0.0f;
	
	for(int i = 0; i < NrOfLights; i++)
	{
		float3 LightDirection = WorldPos.xyz - lights[i].LightPosition.xyz;
		float DistanceToLight = length(LightDirection);
		LightDirection = normalize(LightDirection);

		// Diff light
		float difflight = saturate(dot(NormsAndDepth.xyz, -LightDirection));

		// Spec Light
		float3 h = normalize(normalize(CameraPosition.xyz - WorldPos.xyz) - LightDirection);
		float speclight = pow(saturate(dot(h, NormsAndDepth.xyz)), SpecularPower);


		// Shadow Mappings
		float4 posLight = mul(WorldPos, lights[i].LightViewProj);
		posLight.xy /= posLight.w;


		//Compute shadow map tex coord
		float2 smTex = float2(0.5f*posLight.x, -0.5f*posLight.y) + 0.5f;

		// Compute pixel depth for shadowing.
		float depth = posLight.z / posLight.w;

		float SHADOW_EPSILON = 0.00001f;			////////////// PUT THIS WHERE?


		//float PCF_SIZE = 3.0f;								////// Not able to move this to cbuffer, why?


		// PCF
		float shadow = 0.0f;
		if(smTex.x < 0 || smTex.x > 1 || smTex.y < 0 || smTex.y > 1)
			shadow = 1.0f;
		else if(depth > 1.0f)
			shadow = 1.0f;
		else
		{
			for(float s = 0; s < PCF_SIZE; s++)
			{
				for(float q = 0; q < PCF_SIZE; q++)
				{
					shadow += (ShadowMap[i].SampleLevel(shadowMapSampler, smTex + float2(SMAP_DX * (s - PCF_SIZE/2) , SMAP_DX * (q - PCF_SIZE/2)), 0).r + SHADOW_EPSILON < depth) ? 0.0f : 1.0f;
				}
			}
			shadow *= PCF_SIZE_SQUARED;
		}


		// Fall off test 3
		float coef = (lights[i].LightIntensity / 1000.0f) + (DistanceToLight * DistanceToLight) / (lights[i].LightIntensity * lights[i].LightIntensity);

		difflight /= coef;
		/*	/// For slower fall-off on spec light. Doesnt work with quad-based culling tho.
		if(coef > 10.0f)
			speclight /= coef / 10.0f;
			*/
		speclight /= coef;
		
		difflight *= shadow;
		speclight *= shadow;
		
		diffuseLighting += difflight;
		specLighting += speclight;
	}
	
	diffuseLighting = saturate(diffuseLighting / NrOfLights);
	specLighting = saturate(specLighting / NrOfLights);

	//float4 finalColor = float4((AmbientLight.xyz * DiffuseColor.xyz + DiffuseColor.xyz * diffuseLighting + SpecularColor.xyz * specLighting), DiffuseColor.w);
	float4 finalColor = float4((AmbientLight.xyz * DiffuseColor.xyz + DiffuseColor.xyz * diffuseLighting + SpecularColor.xyz * specLighting), 1.0f);

	



	// Haxfix, want it above but I lose 75% of my FPS then (??!?!? :S:S:S:S:S)
	if(NormsAndDepth.w < -0.5f)		// All pixels that has a negative depth means that there is no geometry, therefor return without lightcalcs.
		finalColor = DiffuseColor;

	if(NormsAndDepth.w > 1.0f)		// All pixels that has a greater than 1 depth means that there is no geometry and there is skybox, therefor return without lightcalcs.
		finalColor = DiffuseColor;
	
	
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

	//**temp:**
	//todo: if player is on red team, reduce redness and increase blueness**

	//Exlude skybox
	if(NormsAndDepth.w < 1.00001f)
	{
		//Exclude nullColor
		if((uint)DiffuseColor.w != 0)
		{
			switch((uint)DiffuseColor.w)
			{
				case 1: (finalColor += WHITE) * 0.5f; break;
				case 2: (finalColor += BLACK) * 0.5f; break;
				case 3: (finalColor += RED) * 0.5f; break;
				case 4: (finalColor += GREEN) * 0.5f; break;
				case 5: (finalColor += BLUE) * 0.5f; break;
				case 6: (finalColor += YELLOW) * 0.5f; break;
				case 7: (finalColor += CYAN) * 0.5f; break;
				case 8: (finalColor += MAGENTA) * 0.5f; break;
			}
		}
	}
	
	
	//finalColor = SSAO(input.Pos.xy, NormalAndDepth);

	finalColor = Lava(finalColor, WorldPos, NormsAndDepth.w);

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
		
		SetBlendState( SrcAlphaBlendingAdd, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF );
    }  
}