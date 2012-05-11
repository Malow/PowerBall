//------------------------------------------------------------------------------------------------------
//	Post-process effect: Lava.
//	Todo: Creates a plane on the XZ-axes with "waves" at given height, size and interval.
//	Requirements: World positions of pixels, Texture for the lava and a timer for the waves.
//------------------------------------------------------------------------------------------------------

#include "stdafx.fx"

//------------------------------------------------------------------------------------------------------
//	Global variables (non-numeric values cannot be added to constant buffers)
//------------------------------------------------------------------------------------------------------
Texture2D LavaTexture;

//------------------------------------------------------------------------------------------------------
//	Lava shader
//------------------------------------------------------------------------------------------------------
float4 Lava(float4 color, float4 pixelWorldPosition)
{
	float4 lavaColor = color;

	bool textureLava = true;

	float L = 10.0f;
	float H = 15.0f;

	if(CameraPosition.y < L)
		return float4(0,0,0,0);

	if(pixelWorldPosition.y <= L + H)
	{
		float timer = timerMillis * 0.001f;
		float3 E = pixelWorldPosition.xyz - CameraPosition.xyz;
		E = normalize(E);

		float4 W = float4(0, 1, 0, -L);
		float t = (L - CameraPosition.y) / E.y;	
		
		float3 S = CameraPosition.xyz + t * E;
		float height = 0.0f;

		float bias = sin(timer * 0.2f) * sin(S.x * 0.2f) * sin(S.z * 0.2f);
		bias *= 1.2f - (length(CameraPosition.xyz - S) / 100.0f);
		float temp = bias;
		bias = bias - height;
		height = temp;

		bias *= 0.1f;
		if(t < 1000.0f) //for higher angles/height
		{
			L += bias * H;
		}	
		t = (L - CameraPosition.y) / E.y;
		S = CameraPosition.xyz + E * t;
		if(CameraPosition.y > L) //for low angles/height
		{
			lavaColor = LavaTexture.Sample(LinearWrapSampler, (float2(S.x, S.z) / 50.0f) + timer * 0.01f);
		}
	}

	if(pixelWorldPosition.y > L)
		lavaColor = color;

	return lavaColor;
}

