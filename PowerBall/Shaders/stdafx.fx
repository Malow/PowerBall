/*
Include file for all samplers, rasterizer states, and blend states and constant buffers.

*/


#ifndef STDAFX_FX
#define STDAFX_FX

//------------------------------------------------------------------------------------------------------
//	Constants
//------------------------------------------------------------------------------------------------------
#define PI float(3.14159265358979323846f)
#define WHITE	float4(1.0f, 1.0f, 1.0f, 1.0f)
#define BLACK	float4(0.0f, 0.0f, 0.0f, 1.0f)
#define RED		float4(1.0f, 0.0f, 0.0f, 1.0f)
#define GREEN	float4(0.0f, 1.0f, 0.0f, 1.0f)
#define BLUE	float4(0.0f, 0.0f, 1.0f, 1.0f)
#define YELLOW	float4(1.0f, 1.0f, 0.0f, 1.0f)
#define CYAN	float4(0.0f, 1.0f, 1.0f, 1.0f)
#define MAGENTA float4(1.0f, 0.0f, 1.0f, 1.0f)


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

	uint windowWidth;
	uint windowHeight;
};

//------------------------------------------------------------------------------------------------------
//	State structures
//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------
//	SamplersStates
//------------------------------------------------------------------------------------------------------
SamplerState LinearWrapSampler
{
	Filter = MIN_MAG_MIP_LINEAR; 
	AddressU = Wrap;
	AddressV = Wrap;
};
SamplerState LinearClampSampler
{
	Filter = MIN_MAG_MIP_LINEAR; 
	AddressU = Clamp;
	AddressV = Clamp;
};
SamplerState PointWrapSampler
{
	Filter = MIN_MAG_MIP_POINT; 
	AddressU = Wrap;
	AddressV = Wrap;
};
SamplerState PointBorderSampler
{
	Filter = MIN_MAG_MIP_POINT; 
	AddressU = Border;
	AddressV = Border;
	BorderColor = float4(0.0f, 0.0f, 0.0f, 1e5f);
};
SamplerState PointClampSampler
{
	Filter = MIN_MAG_MIP_POINT; 
	AddressU = Clamp;
	AddressV = Clamp;
};
SamplerState AnisotropicClampSampler 
{
	Filter = ANISOTROPIC;
	AddressU = Clamp;
	AddressV = Clamp;
	AddressW = Clamp;
	MaxAnisotropy = 4;
	ComparisonFunc = ALWAYS;
	MaxLOD = 0.0f;
	MinLOD = 0.0f;
};
//------------------------------------------------------------------------------------------------------
//	RasterizerStates
//------------------------------------------------------------------------------------------------------
RasterizerState BackCulling
{
	CullMode = Back;
};

//------------------------------------------------------------------------------------------------------
//	DepthStencilStates
//------------------------------------------------------------------------------------------------------
DepthStencilState EnableDepth
{
    DepthEnable = TRUE;
    DepthWriteMask = ALL;
    DepthFunc = LESS_EQUAL;
};

#endif