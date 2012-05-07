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
//Texture1D<float3> rndTex;	//used for sampling. contains randomized 3D-vectors x,y,z[-1,1] = length[0,3]
float DEPTH_EPSILON = 0.001f;
static const uint		nrOfSamples = 8;	//temp**
Texture2D<float3> rndVectorsTex;
//------------------------------------------------------------------------------------------------------
//	Constant buffers
//------------------------------------------------------------------------------------------------------
cbuffer PerFrame
{
	float		angleBias;				//angle in radian from plane of pixel to ignore samples
	float4x4	viewInverseTranspose;	//transposed inverse of of camera view-matrix
	float4x4	projMatrix;				//projection matrix of camera
	float4x4    invProjMatrix;			//inverse projection matrix
	float4x4	viewToTex;	//projmatrix * texture

	float4x4	viewMatrix;

	//**
	// Coordinates given in view space.
	float   gOcclusionRadius    = 0.5f;
	float   gOcclusionFadeStart = 0.2f;
	float   gOcclusionFadeEnd   = 2.0f;
	float   gSurfaceEpsilon     = 0.05f;
	float3	uniRndVectors[14];
	//**

	//**blur
	float gWeights[11] = 
	{
		0.05f, 0.05f, 0.1f, 0.1f, 0.1f, 0.2f, 0.1f, 0.1f, 0.1f, 0.05f, 0.05f
	};
	//**
};
//**blur
static const int gBlurRadius = 5;

float Occlusion(float distZ) //***
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
float4 SSAO(float2 texCoord, Texture2D normalAndDepthMap, Texture2D Position, Texture2D Texture)
{
	float4 debugColor = float4(1,1,1,0);
	float occlusionFactor = 0.0f;

	//sample normal (world space) and depth of the pixel (normalized device coordinates [0,1])
	float4 normAndDepth = normalAndDepthMap.Sample(PointBorderSampler, texCoord); 
	//exclude pixels with no depth data (-1.0f) and skybox ( > 1.0f)
	if(normAndDepth.w > -DEPTH_EPSILON && normAndDepth.w < 1.0f + DEPTH_EPSILON) 
	{
		debugColor.w = 1;

		//1. get view position of the pixel
		float4 pixelPosV = float4(Position.Sample(PointClampSampler, texCoord).xyz, 1.0f); //position is in world space
		//convert to view space
		pixelPosV = mul(pixelPosV, viewMatrix);

		float dist = 0.0f;
		float4 offsetPos[nrOfSamples];
		for(uint i = 0; i < nrOfSamples; i++) 
		{
			//2. add 3D-vectors to the view position of the pixel
			float3 offsetVector = rndVectorsTex.Sample(PointWrapSampler, texCoord).xyz; //sample offset vector
			offsetVector = reflect(uniRndVectors[i], offsetVector);
			//check if offset vector is above surface of the pixel and within angle bias, if not, flip and hope for the best
			if(dot(offsetVector, normAndDepth.xyz) > angleBias)
			{
				offsetVector = -offsetVector;
			}
			offsetPos[i] = float4(pixelPosV.xyz + offsetVector, 1.0f); 

			//3. convert offset xy-positions to texture space and z to normalized device coordinates [0,1]
			//convert to clip space xy[-w,w], z[0,w]
			offsetPos[i] = mul(offsetPos[i], projMatrix); 
			//convert to normalized device coordinates xy[-1,1], z[0,1]
			offsetPos[i].xyz /= offsetPos[i].w; 
			//convert to texture space [0,1]
			offsetPos[i].xy = (offsetPos[i].xy + 1) * 0.5f;
			offsetPos[i].y = 1.0f - offsetPos[i].y; //invert direction of y-axis

			//4. get distance between the depth of the offset position and the depth of the pixel at that position
			dist = offsetPos[i].z - normalAndDepthMap.Sample(PointBorderSampler, offsetPos[i].xy).w;  
			
			//if distance is positive, then the offset position is behind the pixel sampled at that xy-location and therefore occlude the pixel being computed to some degree
			if(dist > DEPTH_EPSILON) //use epsilon to avoid using samples behind the pixel
			{
				//5. occlusion 
				occlusionFactor += Occlusion(dist); 

				//6. todo: blur (bilateral)
			}
		}
	}
	
	occlusionFactor /= nrOfSamples;
	float access = 1.0f - occlusionFactor;
	debugColor.xyz = saturate(pow(access, 4.0f));
	debugColor.w = 1.0f;

	return debugColor;
	
}

