//------------------------------------------------------------------------------------------------------
//	Post-process effect: Screen space ambient occlusion (SSAO).
//	Approximates ambient occlusion by sampling the depth of neighboring pixels in screen space.
//	Requirements:	Depth and normal in view space for each pixel (and randomized 3D-vectors).
//					Note that normals should be face normals to avoid false occlusion.
//------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------
//	Global variables (non-numeric values cannot be added to constant buffers)
//------------------------------------------------------------------------------------------------------
Texture1D gRndTex;	//used for sampling. contains randomized 3D-vectors x,y,z[-1,1] = length[0,3]
float DEPTH_EPSILON = 0.001f;
static const uint		nrOfSamples = 8;	//**temp.**

//------------------------------------------------------------------------------------------------------
//	Constant buffers
//------------------------------------------------------------------------------------------------------
cbuffer PerFrame
{
	//uint		nrOfSamples;	//size of gRndTex
	uint		width;			//width of depth map and normal map
	uint		height;			//height of depth map and normal map**
	float		radius;			//length multiplier of random-vectors**
	float		angleBias;		//angle in radian from plane of pixel to ignore samples
	float4x4	projMatrix;		//projection matrix
	float4x4    invProjMatrix;	//inverse projection matrix
};

//------------------------------------------------------------------------------------------------------
//	State structures
//------------------------------------------------------------------------------------------------------
SamplerState LinearSampler 
{
	Filter = MIN_MAG_MIP_LINEAR; 
	AddressU = Wrap;
	AddressV = Wrap;
};

//------------------------------------------------------------------------------------------------------
//	Functions
//------------------------------------------------------------------------------------------------------
/*float3 ScreenSpaceToViewSpace(float2 screenPos, float zPos)
{
	//input position is in screen space x[0,width], y[0,height]
	float4 pixelPos = float4(screenPos, zPos, 1.0f);

	//remap to view space
	//convert to texture space [0,1]
	pixelPos.x /= width;
	pixelPos.y /= height;
	pixelPos.y = 1.0f - pixelPos.y; //invert direction of y-axis
	//convert to normalized device coordinates [-1,1]
	pixelPos.xy = (pixelPos.xy * 2.0f) - 1.0f;
	//transform by the inverse projection matrix
	pixelPos = mul(pixelPos, invProjMatrix);
	//convert to view-space by dividing by w
	pixelPos.xyz / pixelPos.w;

	return pixelPos.xyz;
}*/
float2 ScreenSpaceToViewSpace(float2 screenPos)
{
	//input position is in screen space x[0,width], y[0,height]
	float4 pixelPos = float4(screenPos, 0.0f, 1.0f);

	//remap to view space
	//convert to texture space [0,1]
	pixelPos.x /= width;
	pixelPos.y /= height;
	pixelPos.y = 1.0f - pixelPos.y; //invert direction of y-axis
	//convert to normalized device coordinates [-1,1]
	pixelPos.xy = (pixelPos.xy * 2.0f) - 1.0f;
	//transform by the inverse projection matrix
	pixelPos = mul(pixelPos, invProjMatrix);
	//convert to view-space by dividing by w
	pixelPos.xy / pixelPos.w;

	return pixelPos.xy;
}
float2 ViewSpaceToScreenSpace(float4 viewPos)
{
	//convert to clip space [-w,w]
	viewPos = mul(viewPos, projMatrix); 
	//convert to normalized device coordinates xy[-1,1], z[0,1]
	viewPos.xyz /= viewPos.w;
	//convert to texture space [0,1]
	viewPos.xy = (viewPos.xy + 1) * 0.5f;
	viewPos.y = 1.0f - viewPos.y; //invert direction of y-axis
	//convert to screen space x[0,width], y[0,height]
	viewPos.x *= width;
	viewPos.y *= height;

	return viewPos.xy;
}

//------------------------------------------------------------------------------------------------------
//	SSAO Shader
//------------------------------------------------------------------------------------------------------
float4 SSAO(float2 pixel, Texture2D normalAndDepthMap)
{
	/**
	uint w = 0;
	uint h = 0;
	normalAndDepthMap.GetDimensions(w, h);
	**/

	
	//get normal and depth of pixel
	float4 normAndDepth = normalAndDepthMap.Sample(LinearSampler, pixel);
	//get view-position of pixel
	//float3 pixelPosV = ScreenSpaceToViewSpace(pixel, normAndDepth.w);**
	float3 pixelPosV = float3(ScreenSpaceToViewSpace(pixel), normAndDepth.w);
	//add 3D-vectors to this position
	float4 offsetPos[nrOfSamples];
	float3 offsetVector = float3(0.0f, 0.0f, 0.0f);
	[unroll]
	for(uint i = 0; i < nrOfSamples; i++)
	{
		offsetVector = gRndTex.Sample(LinearSampler, (float)i);  //**uniform rnd**
		//check if offsetVector is above surface and within angle bias, if not, flip **flip med bias**
		if(dot(normAndDepth.xyz, offsetVector) < angleBias)
		{
			offsetVector = -offsetVector;
		}
		offsetPos[i] = float4(pixelPosV + offsetVector, 1.0f);
	}

	//convert offset positions to screen space
	[unroll]
	for(uint i = 0; i < nrOfSamples; i++)
	{
		offsetPos[i].xy = ViewSpaceToScreenSpace(offsetPos[i]);
	}

	//compare depth of the pixel with the depth of pixels at the offset xy-position
	float dist = 0.0f;
	float pixelDepth = -1.0f;
	float occlusionFactor = 0.0f;
	float4 debugColor = float4(1,1,1,1);
	[unroll]
	for(uint i = 0; i < nrOfSamples; i++)
	{
		//get distance in depth between pixel and pixel at offset xy-position
		dist = normAndDepth.w - normalAndDepthMap.Sample(LinearSampler, offsetPos[i].xy).w; 
		//if distance is positive, then the offset position is infront of the pixel and therefore occlude the pixel to some degree
		if(dist > DEPTH_EPSILON) //** ? 0 : 1;** use epsilon to avoid using samples behind the pixel
		{
			//calculate occlusion factor by using distance
			//occlusion += pow(dist[i], 2.0f);**
			occlusionFactor = 1.0f;
			debugColor.xyzw = 0.0f;
		}
	}
	//occlusion /= nrOfSamples;

	//todo: occlusion function & blur
	//**blur - separate pass/shader**

	//**todo: ev. hantera då pixeldjup är -1**

	//debug
	return debugColor;
	//return occlusionFactor * float4(1,1,1,1);
	//return float4(-occlusionFactor,-occlusionFactor,-occlusionFactor,-occlusionFactor);
}

