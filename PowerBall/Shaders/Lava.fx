//------------------------------------------------------------------------------------------------------
//	Post-process effect: Lava with effects**
//	Todo: description
//	Requirements:	world positions of pixels**Todo: more**
//------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------
//	Global variables (non-numeric values cannot be added to constant buffers)
//------------------------------------------------------------------------------------------------------
Texture2D LavaTexture;

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

	return lavaColor;
}

