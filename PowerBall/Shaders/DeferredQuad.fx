
Texture2D Texture;
Texture2D NormalAndDepth;
Texture2D Position;
Texture2D Specular;

#define LightRadiusCoef 5.0f;

SamplerState linearSampler
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};

Texture2DArray ShadowMap;
SamplerState shadowMapSampler
{
	Filter = MIN_MAG_MIP_POINT;
	AddressU = Clamp;
	AddressV = Clamp;
};

DepthStencilState DisableDepthWrite
{
    DepthEnable = TRUE;
    DepthWriteMask = ZERO;
};

RasterizerState NoCulling
{
	CullMode = NONE;
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

struct Light
{
	float4 LightPosView;
	float4 LightPosition;
	float4 LightColor;
	float LightIntensity;
	matrix LightViewProj;
};

cbuffer EveryFrame
{
	matrix CameraVP;
	matrix CameraView;
	matrix CameraProj;
	float4 CameraPosition;
	float CameraFar;
	float CameraNear;

	float NrOfLights;
	Light lights[10];

	float SMAP_DX;
	//float PCF_SIZE;
	float PCF_SIZE_SQUARED;

	float ScreenWidth;
	float ScreenHeight;
};

// STOLEN FROM THE EXAMPLE
struct VSOut
{
    float4 coords : coords;         // [min.xy, max.xy] in clip space
    float quadZ : quadZ;
    uint lightIndex : lightIndex;
};

// STOLEN FROM THE EXAMPLE
struct GSOut
{
    float4 pos : SV_Position;
    uint lightIndex : Instance;
};


// STOLEN FROM THE EXAMPLE
void UpdateClipRegionRoot(float nc,          // Tangent plane x/y normal coordinate (view space)
                          float lc,          // Light x/y coordinate (view space)
                          float lz,          // Light z coordinate (view space)
                          float lightRadius,
                          float cameraScale, // Project scale for coordinate (_11 or _22 for x/y respectively)
                          inout float clipMin,
                          inout float clipMax)
{
    float nz = (lightRadius - nc * lc) / lz;
    float pz = (lc * lc + lz * lz - lightRadius * lightRadius) /
               (lz - (nz / nc) * lc);

    [flatten] if (pz > 0.0f) {
        float c = -nz * cameraScale / nc;
        [flatten] if (nc > 0.0f) {        // Left side boundary
            clipMin = max(clipMin, c);
        } else {                          // Right side boundary
            clipMax = min(clipMax, c);
        }
    }
}



// STOLEN FROM THE EXAMPLE
void UpdateClipRegion(float lc,          // Light x/y coordinate (view space)
                      float lz,          // Light z coordinate (view space)
                      float lightRadius,
                      float cameraScale, // Project scale for coordinate (_11 or _22 for x/y respectively)
                      inout float clipMin,
                      inout float clipMax)
{
    float rSq = lightRadius * lightRadius;
    float lcSqPluslzSq = lc * lc + lz * lz;
	float d = rSq * lc * lc - lcSqPluslzSq * (rSq - lz * lz);

    if (d > 0) {
        float a = lightRadius * lc;
        float b = sqrt(d);
        float nx0 = (a + b) / lcSqPluslzSq;
        float nx1 = (a - b) / lcSqPluslzSq;
        
        UpdateClipRegionRoot(nx0, lc, lz, lightRadius, cameraScale, clipMin, clipMax);
        UpdateClipRegionRoot(nx1, lc, lz, lightRadius, cameraScale, clipMin, clipMax);
    }
}


// STOLEN FROM THE EXAMPLE
float4 ComputeClipRegion(float3 lightPosView, float lightRadius)
{
    // Early out with empty rectangle if the light is too far behind the view frustum
    float4 clipRegion = float4(1, 1, 0, 0);
    if (lightPosView.z + lightRadius >= CameraNear) {
        float2 clipMin = float2(-1.0f, -1.0f);
        float2 clipMax = float2( 1.0f,  1.0f);
    
        UpdateClipRegion(lightPosView.x, lightPosView.z, lightRadius, CameraProj._11, clipMin.x, clipMax.x);
        UpdateClipRegion(lightPosView.y, lightPosView.z, lightRadius, CameraProj._22, clipMin.y, clipMax.y);

        clipRegion = float4(clipMin, clipMax);
    }

    return clipRegion;
}

VSOut VS(uint lightIndex : SV_VertexID)
{
	VSOut output;
	output.lightIndex = lightIndex;

	float lightRadius = lights[lightIndex].LightIntensity * LightRadiusCoef;

	output.coords = ComputeClipRegion(lights[lightIndex].LightPosView.xyz, lightRadius);	
	
	float quadDepth = max(CameraNear, lights[lightIndex].LightPosView.z - lightRadius);
	float4 quadClip = mul(float4(0.0f, 0.0f, quadDepth, 1.0f), CameraProj);
	output.quadZ = quadClip.z / quadClip.w;
	return output;
}

// STOLEN FROM THE EXAMPLE
[maxvertexcount(4)]
void GS(point VSOut input[1], inout TriangleStream<GSOut> quadStream)
{
    GSOut output;
    output.lightIndex = input[0].lightIndex;
    output.pos.zw = float2(input[0].quadZ, 1.0f);

    // Branch around empty regions (i.e. light entirely offscreen)
    if (all(input[0].coords.xy < input[0].coords.zw)) {
        output.pos.xy = input[0].coords.xw;      // [-1,  1]
        quadStream.Append(output);
        output.pos.xy = input[0].coords.zw;      // [ 1,  1]
        quadStream.Append(output);
        output.pos.xy = input[0].coords.xy;      // [-1, -1]
        quadStream.Append(output);
        output.pos.xy = input[0].coords.zy;      // [ 1, -1]
        quadStream.Append(output);
        quadStream.RestartStrip();
    }
}

float4 CalcLight(float2 tex, Light light, int lightIndex)
{
	float4 NormsAndDepth = NormalAndDepth.Sample(linearSampler, tex);
	float4 WorldPos = Position.Sample(linearSampler, tex);

	float SpecularPower = Specular.Sample(linearSampler, tex).w;
	float4 SpecularColor = float4(Specular.Sample(linearSampler, tex).xyz, 1.0f);


	float PCF_SIZE = 3.0f;								////// Not able to move this to cbuffer, why?

	float3 LightDirection = WorldPos.xyz - light.LightPosition.xyz;
	float DistanceToLight = length(LightDirection);
	LightDirection = normalize(LightDirection);

	// Diff light
	float difflight = saturate(dot(NormsAndDepth.xyz, -LightDirection));

	// Spec Light
	float3 h = normalize(normalize(CameraPosition.xyz - WorldPos.xyz) - LightDirection);
	float speclight = pow(saturate(dot(h, NormsAndDepth.xyz)), SpecularPower);

		
	// Shadow Mappings
	float4 posLight = mul(WorldPos, light.LightViewProj);
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
				//shadow += (ShadowMap[lightIndex].Sample(shadowMapSampler, smTex + float2(SMAP_DX * (s - PCF_SIZE/2) , SMAP_DX * (q - PCF_SIZE/2))).r + SHADOW_EPSILON < depth) ? 0.0f : 1.0f;
				shadow += (ShadowMap.Sample(shadowMapSampler, float3(smTex + float2(SMAP_DX * (s - PCF_SIZE/2) , SMAP_DX * (q - PCF_SIZE/2)), lightIndex)).r + SHADOW_EPSILON < depth) ? 0.0f : 1.0f;
			}
		}
		shadow /= PCF_SIZE_SQUARED;
	}
	
	// Fall off test 3
	float coef = (light.LightIntensity / 1000.0f) + (DistanceToLight * DistanceToLight) / (light.LightIntensity * light.LightIntensity);

	difflight /= coef;
	/*	/// For slower fall-off on spec light. Doesnt work with quad-based culling tho.
	if(coef > 10.0f)
		speclight /= coef / 10.0f;
		*/
	speclight /= coef;

	difflight *= shadow;
	speclight *= shadow;
		
	float4 finalColor = float4((float3(1.0f, 1.0f, 1.0f) * difflight + SpecularColor.xyz * speclight), 1.0f);
	finalColor = saturate(finalColor);
	finalColor.w = (finalColor.x + finalColor.y + finalColor.z) / 3.0f;

	return finalColor;
}

float4 PS(GSOut input) : SV_Target
{    
    float4 result = float4(0.0f, 0.0f, 0.0f, 0.0f);

	float4 worldPos = Position.Sample(linearSampler, float2(input.pos.x / ScreenWidth, input.pos.y / ScreenHeight));
	float4 viewPos = mul(worldPos, CameraView);

    // Avoid shading skybox/background pixels
    // NOTE: Compiler doesn't quite seem to move all the unrelated surface computations inside here
    // We could force it to by restructuring the code a bit, but the "all skybox" case isn't useful for
    // our benchmarking anyways.
    float3 litDiffuse = float3(0.0f, 0.0f, 0.0f);
    float3 litSpecular = float3(0.0f, 0.0f, 0.0f);
    if (viewPos.z < CameraFar) 
	{
		int index = input.lightIndex;
		Light light = lights[index];
		float lightRadius = light.LightIntensity * LightRadiusCoef;
		
		float3 directionToLight = light.LightPosView.xyz - viewPos.xyz;
		float distanceToLight = length(directionToLight);
		
		if (distanceToLight < lightRadius) 
		{
			float2 tex = float2(input.pos.x / ScreenWidth, input.pos.y / ScreenHeight);
			result = CalcLight(tex, light, index);
		}
    }

    // Convert to monochromatic specular for accumulation
	/*
    float specularLum = dot(litSpecular, float3(0.2126f, 0.7152f, 0.0722f));
    result = float4(litDiffuse, specularLum);
	*/
	
    return result;
}



technique11 BasicTech
{
    pass p0
    {
		// Set VS, GS, and PS
        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( CompileShader( gs_4_0, GS() ) );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );
	    

		SetDepthStencilState( DisableDepthWrite, 0 );
	    SetRasterizerState( NoCulling );
		SetBlendState( SrcAlphaBlendingAdd, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF );
    }  
}


