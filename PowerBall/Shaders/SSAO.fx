//------------------------------------------------------------------------------------------------------
//	Post-process effect: Screen space ambient occlusion (SSAO).
//	Approximates ambient occlusion by sampling the depth of neighboring pixels in screen space.
//	Requirements:	Depth and normal in view space for each pixel (and randomized 3D-vectors).
//					Note that normals should be face normals to avoid false occlusion.
//------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------
//	Global variables (non-numeric values cannot be added to constant buffers)
//------------------------------------------------------------------------------------------------------
Texture1D<float3> gRndTex;	//used for sampling. contains randomized 3D-vectors x,y,z[-1,1] = length[0,3]
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
	AddressU = Clamp;
	AddressV = Clamp;
};

//------------------------------------------------------------------------------------------------------
//	Functions
//------------------------------------------------------------------------------------------------------
float3 ScreenSpaceToViewSpace(float2 screenPos, float zPos)
{
	//input position is in screen space x[0,width], y[0,height]
	float4 pixelPosV = float4(screenPos, zPos, 1.0f);

	//convert to texture space [0,1]
	pixelPosV.x /= width;
	pixelPosV.y /= height;
	//convert to normalized device coordinates [-1,1]
	pixelPosV.y = 1.0f - pixelPosV.y; //first invert direction of y-axis
	pixelPosV.xy = pixelPosV.xy * 2.0f - 1.0f;
	//transform by the inverse projection matrix; //**
	pixelPosV = mul(pixelPosV, invProjMatrix);
	//convert to view-space by dividing by w //**
	pixelPosV.xyz /= pixelPosV.w;

	return pixelPosV.xyz;
}

float2 ViewSpaceToScreenSpace(float4 viewPos)
{
	//convert to clip space xy[-w,w], z[0,w]
	viewPos = mul(viewPos, projMatrix); 
	//convert to normalized device coordinates xy[-1,1], z[0,1]
	viewPos.xy /= viewPos.w;
	//convert to texture space [0,1]
	viewPos.xy = (viewPos.xy + 1) * 0.5f;
	viewPos.y = 1.0f - viewPos.y; //invert direction of y-axis
	//convert to screen space x[0,width], y[0,height]
	viewPos.x *= width;
	viewPos.y *= height;

	return viewPos.xyz;
}/*
float3 TextureSpaceToViewSpace(float2 texCoord, float zPos)
{
	//input position is in texture space [0,1]
	float4 pixelPosV = float4(texCoord, zPos, 1.0f);

	//convert to normalized device coordinates [-1,1]
	pixelPosV.y = 1.0f - pixelPosV.y; //invert direction of y-axis
	pixelPosV.xy = pixelPosV.xy * 2.0f - 1.0f;
	//transform by the inverse projection matrix;
	pixelPosV = mul(pixelPosV, invProjMatrix);
	//convert to view-space by dividing by w
	pixelPosV.xyz /= pixelPosV.w;

	return pixelPosV.xyz;
}*/
float2 ViewSpaceToTextureSpace(float4 viewPos)
{
	//convert to clip space [-w,w]
	viewPos = mul(viewPos, projMatrix); 
	//convert to normalized device coordinates xy[-1,1], z[0,1]
	viewPos.xy /= viewPos.w; 
	//convert to texture space [0,1]
	viewPos.xy = (viewPos.xy + 1) * 0.5f;
	viewPos.y = 1.0f - viewPos.y; //invert direction of y-axis

	return viewPos.xy;
}

//------------------------------------------------------------------------------------------------------
//	SSAO Shader
//------------------------------------------------------------------------------------------------------
float4 SSAO(float2 pixel, Texture2D normalAndDepthMap)
{
	/**optimera:
	uint w = 0;
	uint h = 0;
	normalAndDepthMap.GetDimensions(w, h);

	texCoord
	**/
	
	//get normal (view space) and depth of the pixel [0,1]
	float2 texCoord = float2(pixel.x / width, pixel.y / height);
	float4 normAndDepth = normalAndDepthMap.Sample(LinearSampler, texCoord);

	float4 debugColor = float4(1,1,1,1);
	if(normAndDepth.w > -DEPTH_EPSILON)
	{
		//1. get view position of pixel
		float3 pixelPosV = ScreenSpaceToViewSpace(pixel, normAndDepth.w);

		float4 offsetPos[nrOfSamples];
		float3 offsetVector = float3(0.0f, 0.0f, 0.0f);
		float dist = 0.0f;
		float pixelDepth = -1.0f;
		float occlusionFactor = 0.0f;

		for(uint i = 0; i < nrOfSamples; i++)
		{
			//2. add 3D-vectors to this the view position of the pixel
			offsetVector = gRndTex.Sample(LinearSampler, (float)i);  //sample offset vector //**uniform rnd**
			//check if offset vector is above surface and within angle bias, if not, flip **flip med bias eller godta ev. förlust?**
			if(dot(offsetVector, normAndDepth.xyz) < angleBias)
			{
				offsetVector = -offsetVector;
			}
			offsetPos[i] = float4(pixelPosV + offsetVector, 1.0f);

			//3. convert offset positions to texture space **och z som NDC? beror på testet nedan**
			/*offsetPos[i].xy = ViewSpaceToScreenSpace(offsetPos[i]);
			offsetPos[i].x /= width;
			offsetPos[i].y /= height;*/
			offsetPos[i].xy = ViewSpaceToTextureSpace(offsetPos[i]);


			//4. compare depth of the offset position and pixel in the depth map at offset xy-position
			dist = normAndDepth.w - normalAndDepthMap.Sample(LinearSampler, offsetPos[i].xy).w; //**jämföra pixeldjup med pixeldjup vid offset pos?**
			//dist = offsetPos[i].z - normalAndDepthMap.Sample(LinearSampler, offsetPos[i].xy).w;  

			//5. **todo: occlusion function**
			/*
				Blockers closer to the sample should occlude more
				Blockers far from the sample don’t occlude at all
				Blockers behind don’t occlude at all
			*/
			//if distance is positive, then the offset position is infront of the pixel and therefore occlude the pixel to some degree
			if(dist > DEPTH_EPSILON) //use epsilon to avoid using samples behind the pixel
			{
				occlusionFactor = 1.0f / nrOfSamples;
				debugColor -= occlusionFactor * float4(1,1,1,1);
			}
		}
	}
	else
	{
		debugColor = float4(0,1,0,1);
	}

	//todo: occlusion function & blur
	//**blur - separate pass/shader**

	//debug
	return debugColor;
	//return occlusionFactor * float4(1,1,1,1);
	//return float4(-occlusionFactor,-occlusionFactor,-occlusionFactor,-occlusionFactor);
}

