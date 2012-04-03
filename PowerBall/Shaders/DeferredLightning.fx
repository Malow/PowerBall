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

cbuffer EveryFrame
{
	matrix CameraVP;
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
	
	
	if(NormsAndDepth.w < -0.5f)
	{
		//return DiffuseColor;			// Fråga stefan! Decreasar FPS, ska increasa för att den ska skippa ljusberäkningar! Den returnar inte här utan den gör allt under också, fast den returnar colorn här
	}
	if(NormsAndDepth.w < -0.5f)
	{
		//float4 asd = ShadowMap[500].Sample(linearSampler, input.tex);		// Garantued crash if it comes here
	}


	float4 WorldPos = Position.Sample(linearSampler, input.tex);

	DiffuseColor.w = 1.0f;

	float4 AmbientLight = float4(DiffuseColor.xyz * 0.5f, 1.0f);
	float SpecularPower = Specular.Sample(linearSampler, input.tex).w;
	float4 SpecularColor = float4(Specular.Sample(linearSampler, input.tex).xyz, 1.0f);


	float PCF_SIZE = 3.0f;								////// Not able to move this to cbuffer, why?

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
	}
	
	diffuseLighting = saturate(diffuseLighting / NrOfLights);
	specLighting = saturate(specLighting / NrOfLights);

	float4 finalColor = float4((AmbientLight.xyz * DiffuseColor.xyz + DiffuseColor.xyz * diffuseLighting + SpecularColor.xyz * specLighting), DiffuseColor.w);
	
	
	

	/*
	//Basic Screen Space Ambient Occlussion:
	// Need to rework to use random vectors / kernel with random rotation.
	float AO_SIZE = 7.0f;
	float AO_SCALE = 5.0f;
	float Total_AO = 0.0f;
	finalColor = saturate(finalColor);
	float myDepth = NormsAndDepth.w;

	//if(myDepth == 1.0f)	
		//return finalColor;

	float depth = 0;
	[unroll] for(float s = 0; s < AO_SIZE; s++)
	{
		[unroll] for(float q = 0; q < AO_SIZE; q++)
		{
			depth = NormalAndDepth.Sample(linearSampler, input.tex + float2((1.0f/1920.0f)*(s-AO_SIZE/2) * AO_SCALE, (1.0f/1080.0f)*(q-AO_SIZE/2) * AO_SCALE)).w;
			
			
			float depthDiff = myDepth - depth;
			if(depthDiff > 0.005f)		// Avoid self-occlusion
				Total_AO += 0.0000003f / (depthDiff * depthDiff);
				
			
			
			//if(depth < myDepth)
				//if(depth + 0.005f > myDepth)		// To avoid objects from Ambient Occlude each other if they are not even close to eachother.
					//Total_AO += (myDepth - depth) * 5.0f;
					
		}
	}

	finalColor = float4(1,1,1,1);		// For debugging, shows only AO

	finalColor *= (1.0f - Total_AO);
	*/

	/*
	// Better SSAO:
	finalColor = saturate(finalColor);
	float myDepth = NormsAndDepth.w;
	float Total_AO = 0.0f;
	float3 rand[10] = {
		float3(-0.010735935, 0.01647018, 0.0062425877),
		float3(-0.06533369, 0.3647007, -0.13746321),
		float3(-0.6539235, -0.016726388, -0.53000957),
		float3(0.40958285, 0.0052428036, -0.5591124),
		float3(-0.1465366, 0.09899267, 0.15571679),
		float3(-0.44122112, -0.5458797, 0.04912532),
		float3(0.03755566, -0.10961345, -0.33040273),
		float3(0.019100213, 0.29652783, 0.066237666),
		float3(0.8765323, 0.011236004, 0.28265962),
		float3(0.29264435, -0.40794238, 0.15964167)
		};
	for(int s = 0; s < 10; s++)
	{
		if(dot(NormsAndDepth.xyz, rand[s]) < 0.0f)
			rand[s] *= -1;
		rand[s] = float3(input.Pos.xy, myDepth) + rand[s] * 5;
	}
	for(int t = 0; t < 10; t++)
	{
		float depth = NormalAndDepth.Sample(linearSampler, float2(rand[t].x / 1920.0f, rand[t].y / 1080.0f)).w;
		float depthdiff = rand[t].z - depth;
		if(depthdiff > 0.02f)
			Total_AO += (1.0f / (depthdiff * depthdiff)) / 10000.0f;
	}
	finalColor = float4(1,1,1,1);		// For debugging, shows only AO

	finalColor *= (1.0f - Total_AO);
	*/

	
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

	if(finalColor.x < 0.0f)		// Haxfix, want it above but I lose 75% of my FPS then (??!?!? :S:S:S:S:S)
		return DiffuseColor;

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