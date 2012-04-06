//------------------------------------------------------------------------------------
//	Post-process effect: Screen space ambient occlusion (SSAO).
//	Approximates ambient occlusion by sampling the depth of 
//	neighboring pixels in screen space.
//	Requirements: Depth and normal for each pixel.
//------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------
//	Global variables (non-numeric values cannot be added to constant buffers
//------------------------------------------------------------------------------------
Texture1D rndTex;	//contains randomized 3D-vectors with values ranging from -1 to 1
Texture2D depthMap;	//contains the depth of each pixel

//------------------------------------------------------------------------------------
//	Constant buffers
//------------------------------------------------------------------------------------
cbuffer PerFrame
{
	uint		width;	//width of screen/depth map
	uint		height;	//height of screen/depth map
	float4x4	viewProjMatrix;
};

//-----------------------------------------------------------------------------------------
//	Shader input/ouput structures
//-----------------------------------------------------------------------------------------
struct VSIn
{
	uint vertexID : SV_VertexID; 
};
struct PSIn
{
	float pos : SV_Position;
};
//-----------------------------------------------------------------------------------------
//	State structures
//-----------------------------------------------------------------------------------------
DepthStencilState EnableDepth
{
    DepthEnable = TRUE;
    DepthWriteMask = ZERO;
};
SamplerState LinearSampler 
{
	Filter = MIN_MAG_MIP_LINEAR; 
	AddressU = Wrap;
	AddressV = Wrap;
};
RasterizerState BackCulling
{
	CullMode = BACK;
};
BlendState NoBlending
{
    AlphaToCoverageEnable = FALSE;
    BlendEnable[0] = FALSE;
    SrcBlend = SRC_COLOR;
    DestBlend = ONE;
    BlendOp = ADD;
    SrcBlendAlpha = ZERO;
    DestBlendAlpha = ZERO;
    BlendOpAlpha = ADD;
    RenderTargetWriteMask[0] = 0x0F;
};

//-----------------------------------------------------------------------------------------
//	Functions
//-----------------------------------------------------------------------------------------
float4 Sample()
{
	//pseudo-code:
	//compute 3d pos of pixel (view space)
	//sample n vectors from rndTex
	//compute 2d pos of vectors (screen space)
	//compare depth of each vector with the depth of the pixel
	//compute occlusionfactor

	//**blur - separate pass?**
}



//-----------------------------------------------------------------------------------------
//	Vertex shader
//-----------------------------------------------------------------------------------------
PSIn VS(VSIn input)
{
	PSIn output = (PSIn)0;

	//positions are in homogenous clip space [-1,1]
	if(input.vertexID == 0)
	{
		output.pos = float4(-1.0f, 1.0f, 0.0f, 1.0f);
	}
	else if(input.vertexID == 1)
	{
		output.pos = float4(-1.0f, -1.0f, 0.0f, 1.0f);
	}
	else if(input.vertexID == 2)
	{
		output.pos = float4(1.0f, 1.0f, 0.0f, 1.0f);
	}
	else if(input.vertexID == 3)
	{
		output.pos = float4(1.0f, -1.0f, 0.0f, 1.0f); 
	}

	return output;
}

//-----------------------------------------------------------------------------------------
//	Pixel shader
//-----------------------------------------------------------------------------------------
float4 VS(VSIn input)
{

	return float4(0,1,0,1);
}

