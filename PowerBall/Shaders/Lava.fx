//------------------------------------------------------------------------------------------------------
//	Post-process effect: Lava with effects**
//	Todo: description
//	Requirements:	world positions of pixels**Todo: more**
//------------------------------------------------------------------------------------------------------

#include "stdafx.fx"

//------------------------------------------------------------------------------------------------------
//	Global variables (non-numeric values cannot be added to constant buffers)
//------------------------------------------------------------------------------------------------------
Texture2D LavaTexture;
Texture2D LavaHeightMap;
Texture2D LavaBumpMap;

//------------------------------------------------------------------------------------------------------
//	Constant buffers
//------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------
//	State structures
//------------------------------------------------------------------------------------------------------
SamplerState LinearWrapSampler 
{
	Filter = MIN_MAG_MIP_LINEAR;  
	AddressU = Wrap;
	AddressV = Wrap;
};


//------------------------------------------------------------------------------------------------------
//	Lava shader
//------------------------------------------------------------------------------------------------------
float4 Lava(float4 color, float4 pixelWorldPosition, float depth)
{
	float4 lavaColor = color;

	bool textureLava = true;

	float L = 10.0f;
	float H = 15.0f;
	/*
	if(depth > 1.0f)
	{
		L = 0.0f;
	}
	*/

	if(CameraPosition.y < L)
		return float4(0,0,0,0);

	if(pixelWorldPosition.y <= L + H)
	{
		float3 E = pixelWorldPosition.xyz - CameraPosition.xyz;
		E = normalize(E);

		float4 W = float4(0, 1, 0, -L);
		//float t = (L - dot(CameraPosition.xyz, W.xyz)) / dot(E, W.xyz);	// Kanske fel
		float t = (L - CameraPosition.y) / E.y;								// billigare s�tt att f� fram t
		float3 S = CameraPosition.xyz + t * E;

		float height = 0.0f;

		for(int i = 0; i < 1; i++)
		{
			/*
			float3 heightSample = LavaHeightMap.Sample(LinearWrapSampler, float2(S.x, S.z) / 100.0f).xyz;
			height = (heightSample.x + heightSample.y + heightSample.z) / 3.0f;			// Using RGB instead of just R when sampling height because working with a black/white heightmap is easier than a red/black.
			height *= 2;
			height -= 1;
			height *= HeightMultiplier;
			float B = height;
			B *= H;
			L += B;

			W = float4(0, 1, 0, -L);
			//t = (-L - dot(CameraPosition.xyz, W.xyz)) / dot(E, W.xyz);	/// Kanske fel
			t = (L - CameraPosition.y) / E.y;
			S = CameraPosition.xyz + t * E;*/





			//float bias = LavaHeightMap.Sample(LinearWrapSampler, (float2(S.x, S.z) + E.xz * 0.1f) * 0.01f).r;
			float bias = sin(timer * 0.2f) * sin(S.x * 0.2f) * sin(S.z * 0.2f);
			bias *= 1.2f - (length(CameraPosition.xyz - S) / 100.0f);
			float temp = bias;
			bias = bias - height;
			height = temp;

			bias *= 0.1f;
			L += bias * H;
			t = (L - CameraPosition.y) / E.y;
			S = CameraPosition.xyz + E * t;
		}

		//if(length(CameraPosition.xyz - S) < length(CameraPosition.xyz - pixelWorldPosition.xyz))
		
		lavaColor = LavaTexture.Sample(LinearWrapSampler, (float2(S.x, S.z) / 50.0f) + timer * 0.01f);
	}

	if(pixelWorldPosition.y > L)
		lavaColor = color;

	/*
	if(depth > 1.0f)
	{
		if(pixelWorldPosition.y < -0.2f)	// Means skybox is drawn on, need a lower number for it to be in appropriate height.
		{
			// right now sampling on both XZ and Y to not get fucked up looking textures
			lavaColor = LavaTexture.Sample(LinearWrapSampler, float2(pixelWorldPosition.x + pixelWorldPosition.y, pixelWorldPosition.z + pixelWorldPosition.y) / 10.0f);
		}
	}
	else
	{
		if(pixelWorldPosition.y < 10.0f)
		{
			lavaColor = LavaTexture.Sample(LinearWrapSampler, float2(pixelWorldPosition.x + pixelWorldPosition.y, pixelWorldPosition.z + pixelWorldPosition.y) / 10.0f);
		}
	}
	*/

	return lavaColor;
}

