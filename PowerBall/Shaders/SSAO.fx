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
float DEPTH_EPSILON = 0.001f;
Texture2D<float3> rndVectorsTex;
//------------------------------------------------------------------------------------------------------
//	Constant buffers
//------------------------------------------------------------------------------------------------------
cbuffer PerFrame
{
	uint		nrOfSamples;
	float		angleBias;				//angle in radian from plane of pixel to ignore samples
	float4x4	projMatrix;				//projection matrix of camera
	float4x4	viewMatrix;

	//**
	//float4x4	viewInverseTranspose;	//transposed inverse of of camera view-matrix
	//float4x4    invProjMatrix;			//inverse projection matrix
	//float4x4	viewToTex;	//projmatrix * texture
	//**
	//**
	// Coordinates given in view space.
	float   gOcclusionRadius    = 0.5f;
	float   gOcclusionFadeStart = 0.2f;
	float   gOcclusionFadeEnd   = 2.0f;
	//**

	float3	uniRndVectors[14];
};

float Occlusion(float distZ) //**
{
	float occlusion = 0.0f;
	if(distZ > DEPTH_EPSILON)
	{
		float fadeLength = gOcclusionFadeEnd - gOcclusionFadeStart;
		
		// Linearly decrease occlusion from 1 to 0 as distZ goes 
		// from gOcclusionFadeStart to gOcclusionFadeEnd.	
		occlusion = saturate( (gOcclusionFadeEnd-distZ)/fadeLength );
	}
	
	return occlusion;	
}
//------------------------------------------------------------------------------------------------------
//	SSAO Shader
//------------------------------------------------------------------------------------------------------
float4 SSAO(float2 texCoord, Texture2D normalAndDepthMap, Texture2D Position)
{
	float4 debugColor = float4(1,1,1,0);
	float occlusionFactor = 0.0f;

	//sample normal (world space) and depth of the pixel (normalized device coordinates [0,1])
	float4 normAndDepth = normalAndDepthMap.Sample(PointBorderSampler, texCoord); 

	//normAndDepth.xyz = mul(float4(normAndDepth.xyz, 1.0f), viewMatrix); **??

	//exclude pixels with no depth data (-1.0f) and skybox ( > 1.0f)
	if(normAndDepth.w > -DEPTH_EPSILON && normAndDepth.w < 1.0f + DEPTH_EPSILON) 
	{
		debugColor.w = 1;

		//1. get view position of the pixel
		float4 pixelPosV = float4(Position.Sample(PointClampSampler, texCoord).xyz, 1.0f); //position is in world space
		//convert to view space
		pixelPosV = mul(pixelPosV, viewMatrix);

		
		float3 offsetVector = rndVectorsTex.Sample(PointWrapSampler, texCoord).xyz; //sample offset vector
		float dist = 0.0f;
		float4 offsetPos = float4(0,0,0,0);
		[unroll]
		for(uint i = 0; i < nrOfSamples; i++) 
		{
			//2. add 3D-vectors to the view position of the pixel
			offsetVector = reflect(uniRndVectors[i], offsetVector);
			//check if offset vector is above surface of the pixel and within angle bias, if not, flip and hope for the best
			if(dot(offsetVector, normAndDepth.xyz) > angleBias)
			{
				offsetVector = -offsetVector;
			}
			offsetPos = float4(pixelPosV.xyz + offsetVector, 1.0f); 

			//3. convert offset xy-positions to texture space and z to normalized device coordinates [0,1]
			//convert to clip space xy[-w,w], z[0,w]
			offsetPos = mul(offsetPos, projMatrix); 
			//convert to normalized device coordinates xy[-1,1], z[0,1]
			offsetPos.xyz /= offsetPos.w; 
			//convert to texture space [0,1]
			offsetPos.xy = (offsetPos.xy + 1) * 0.5f;
			offsetPos.y = 1.0f - offsetPos.y; //invert direction of y-axis

			//4. get distance between the depth of the offset position and the depth of the pixel at that position
			dist = offsetPos.z - normalAndDepthMap.Sample(PointBorderSampler, offsetPos.xy).w;  
			
			//if distance is positive, then the offset position is behind the pixel sampled at that xy-location and therefore occlude the pixel being computed to some degree
			if(dist > DEPTH_EPSILON) //use epsilon to avoid using samples behind the pixel
			{
				//5. occlusion 
				occlusionFactor += Occlusion(dist); 

				//6. todo: blur (bilateral)**
			}
		}
	}
	
	occlusionFactor /= nrOfSamples;
	float access = 1.0f - occlusionFactor;
	debugColor.xyz = saturate(pow(access, 4.0f));
	debugColor.w = 1.0f;

	return debugColor;
	
}

