/*
Include file for all samplers, rasterizer states, and blend states and constant buffers.

*/


#ifndef STDAFX_FX
#define STDAFX_FX


//------------------------------------------------------------------------------------------------------
//	Own structures
//------------------------------------------------------------------------------------------------------
struct Light
{
	float4 LightPosition;
	float4 LightColor;
	float LightIntensity;
	matrix LightViewProj;
};


//------------------------------------------------------------------------------------------------------
//	Constant buffers
//------------------------------------------------------------------------------------------------------
cbuffer EveryFrame
{
	matrix CameraVP;
	float4 CameraPosition;
	
	float NrOfLights;
	Light lights[10];
	//float SMAP_DX;
	//float PCF_SIZE;
	//float PCF_SIZE_SQUARED;

	float timerMillis;
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

#endif