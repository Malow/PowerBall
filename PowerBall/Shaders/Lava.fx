//------------------------------------------------------------------------------------------------------
//	Post-process effect: Lava with effects**
//	Todo: description
//	Requirements:	world positions of pixels**Todo: more**
//------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------
//	Global variables (non-numeric values cannot be added to constant buffers)
//------------------------------------------------------------------------------------------------------

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
float4 Lava(float2 pixel, Texture2D pixelWorldPositions)
{
	float4 lavaColor = float4(1.0f, 0.5f, 0.0f, 1.0f);
	

	return lavaColor; 
}

