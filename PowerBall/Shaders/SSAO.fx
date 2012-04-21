//------------------------------------------------------------------------------------------------------
//	Post-process effect: Screen space ambient occlusion (SSAO).
//	Approximates ambient occlusion by sampling the depth of neighboring pixels with 3D-offset vectors.
//	Requirements:	Normal in view space and depth in normalized device coordinates for each pixel.
//					Note that normals should be face normals to avoid false occlusion.
//------------------------------------------------------------------------------------------------------

#include "stdafx.fx"

//------------------------------------------------------------------------------------------------------
//	Global variables (non-numeric values cannot be added to constant buffers)
//------------------------------------------------------------------------------------------------------
Texture1D<float3> rndTex;	//used for sampling. contains randomized 3D-vectors x,y,z[-1,1] = length[0,3]
float DEPTH_EPSILON = 0.001f;
static const uint		nrOfSamples = 8;	//**temp.**

//------------------------------------------------------------------------------------------------------
//	Constant buffers
//------------------------------------------------------------------------------------------------------
cbuffer PerFrame
{
	//uint		nrOfSamples;	//size of rndTex
	//uint		width;			//width of depth map and normal map
	//uint		height;			//height of depth map and normal map**
	//float		radius;			//length multiplier of random-vectors**
	float		angleBias;		//angle in radian from plane of pixel to ignore samples
	float4x4	projMatrix;		//projection matrix
	float4x4    invProjMatrix;	//inverse projection matrix
};




//------------------------------------------------------------------------------------------------------
//	SSAO Shader
//------------------------------------------------------------------------------------------------------
float4 SSAO(float2 pixel, Texture2D normalAndDepthMap)
{
	//optimera:**
	uint width = 0;
	uint height = 0;
	normalAndDepthMap.GetDimensions(width, height);

	

	//get normal (view space) and depth of the pixel (normalized device coordinates [0,1])
	float2 texCoord = float2(pixel.x / width, pixel.y / height); 
	float4 normAndDepth = normalAndDepthMap.Sample(LinearSampler, texCoord);
	
	float4 debugColor;
	if(normAndDepth.w > -DEPTH_EPSILON) //exclude pixels with no depth data (-1.0f)
	{
		debugColor = float4(1,1,1,1);

		//1. get view position of the pixel
		//x[0,width], y[0,height], z[0,1]
		float4 pixelPosV = float4(pixel.x, pixel.y, normAndDepth.w, 1.0f);
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

		float4 offsetPos[nrOfSamples];
		float3 offsetVector = float3(0.0f, 0.0f, 0.0f);
		float dist = 0.0f;
		float pixelDepth = -1.0f;
		float occlusionFactor = 0.0f;

		for(uint i = 0; i < nrOfSamples; i++) //**
		{
			//2. add 3D-vectors to this the view position of the pixel
			offsetVector = rndTex.Sample(LinearSampler, (float)i);  //sample offset vector
			//check if offset vector is above surface of the pixel and within angle bias, if not, flip **flip med bias eller godta ev. förlust?**
			if(dot(offsetVector, normAndDepth.xyz) > 0.0f)
			{
				offsetVector = -offsetVector;
			}
			offsetPos[i] = float4(pixelPosV.xyz + offsetVector, 1.0f); //**konvertera vector bara, addera sen på pixel.xy??**

			//3. convert offset xy-positions to texture space and z to normalized device coordinates [0,1]
			//convert to clip space xy[-w,w], z[0,w]
			offsetPos[i] = mul(offsetPos[i], projMatrix); 
			//convert to normalized device coordinates xy[-1,1], z[0,1]
			offsetPos[i].xyz /= offsetPos[i].w; 
			//convert to texture space [0,1]
			offsetPos[i].xy = (offsetPos[i].xy + 1) * 0.5f;
			offsetPos[i].y = 1.0f - offsetPos[i].y; //invert direction of y-axis

			//**debug**		
			/*if(offsetPos[i].x >= 0.0f && offsetPos[i].x <= 1.0f 
			&& offsetPos[i].y >= 0.0f && offsetPos[i].y <= 1.0f)
			{
				debugColor = float4(1,1,1,1);
			}
			else
			{
				debugColor = float4(1,0,1,1);
			}*/

			//4. get distance between the depth of the sample (offset position) and the depth sampled at that point
			dist = offsetPos[i].z - normalAndDepthMap.Sample(LinearSampler, offsetPos[i].xy).w;  
			
			//if distance is positive, then the offset position is infront of the pixel and therefore occlude the pixel to some degree
			if(dist > DEPTH_EPSILON) //use epsilon to avoid using samples behind the pixel
			{
				//5. **todo: occlusion function - quadratic**
				//
				//	negative depth deltas give zero occlusion
				//	smaller depth deltas give higher occlusion 
				//	the occlusion value falls to zero again beyond a certain depth delta value
				//

				debugColor -= (1.0f / nrOfSamples) * float4(1,1,1,0); //**

				//6. **todo: blur (bilateral) - separate pass/shader?**
			}
		}
	}
	else
	{
		debugColor = float4(0,1,0,1);
	}

	return debugColor; //**
}

