//------------------------------------------------------------------------------------------------------
//	Post-process effect: Screen space ambient occlusion (SSAO).
//	Approximates ambient occlusion by sampling the depth of neighboring pixels in screen space.
//	Requirements: Depth and normal for each pixel (and randomized 3D-vectors).
//------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------
//	Global variables (non-numeric values cannot be added to constant buffers)
//------------------------------------------------------------------------------------------------------
Texture1D rndTex;	//used for sampling. contains randomized 3D-vectors x,y,z[-1,1] = length[0,3]
Texture2D depthMap;	//contains the depth of each pixel
Texture2D normalMap;//contains the normal of each pixel
float DEPTH_EPSILON = 0.001f;

//------------------------------------------------------------------------------------------------------
//	Constant buffers
//------------------------------------------------------------------------------------------------------
cbuffer PerFrame
{
	uint		nrOfSamples;	//size of rndTex
	uint		width;			//width of depth map and normal map
	uint		height;			//height of depth map and normal map
	float		radius;			//length multiplier of random-vectors
	float		angleBias;		//angle in radian from plane of pixel to ignore samples
	float4x4	projMatrix;		//projection matrix
	float4x4    invProjMatrix;	//inverse projection matrix
};

//------------------------------------------------------------------------------------------------------
//	Shader input/ouput structures
//------------------------------------------------------------------------------------------------------
struct VSIn
{
	uint vertexID : SV_VertexID; 
};
struct PSIn
{
	float4 pos : SV_Position;
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
DepthStencilState DepthDisabled
{
    DepthEnable = FALSE;
    DepthWriteMask = ZERO;
};
RasterizerState BackCulling
{
	CullMode = BACK;
};
BlendState NoBlending //**
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

//------------------------------------------------------------------------------------------------------
//	Functions
//------------------------------------------------------------------------------------------------------
float3 ScreenSpaceToViewSpace(float2 screenPos)
{
	//input position is in screen space x[0,width], y[0,height]
	float4 pixelPos = float4(screenPos, 0.0f, 1.0f);
	//remap to view space
	//convert to texture space [0,1]
	pixelPos.x /= width;
	pixelPos.y /= height;
	//invert direction of y-axis
	pixelPos.y = 1.0f - pixelPos.y;
	//convert to normalized device coordinates [-1,1]
	pixelPos.xy = (pixelPos.xy * 2) - 1;
	//get the depth value for this pixel
	float z = depthMap.Sample(LinearSampler, float2(pixelPos.x, pixelPos.y))
	pixelPos.z = z;
	//transform by the inverse projection matrix
	pixelPos = mul(pixelPos, invProj);
	//convert to view-space by dividing by w
	pixelPos.xyz / pixelPos.w;

	return pixelPos.xyz;
}
float2 ViewSpaceToScreenSpace(float4 viewPos)
{
	//convert to clip space [-w,w]
	viewPos = mul(float4(offsetPos[i], 1.0f), projMatrix); 
	//convert to normalized device coordinates [-1,1]
	viewPos.xyz /= viewPos.w;
	//convert to texture space [0,1]
	viewPos.xy = (viewPos.xy + 1 * 0.5f);
	viewPos.y = 1.0f - viewPos.y; //invert direction of y-axis
	//convert to screen space x[0,width], y[0,height]
	viewPos.x *= width;
	viewPos.y *= height;

	return viewPos.xy;
}
//------------------------------------------------------------------------------------------------------
//	Vertex shader
//------------------------------------------------------------------------------------------------------
PSIn VS(VSIn input)
{
	PSIn output = (PSIn)0;

	//make quad in homogenous clip space [-1,1] (topology: trianglestrip)
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

//------------------------------------------------------------------------------------------------------
//	Pixel shader
//------------------------------------------------------------------------------------------------------
//float4 PS(VSIn input)
float4 SSAO(VSIn input)
{
	//Get view-position of pixel
	float3 pixelPosV = ScreenSpaceToViewSpace(input.pos.xy);

	//add 3D-vectors to this position
	float4 offsetPos[nrOfSamples];
	[unroll]
	for(uint i = 0; i < nrOfSamples; i++)
	{
		float3 offsetVector = rndTex.Sample(LinearSampler, (float)i);  //**uniform rnd**
		float3 pixelNormalV = normalMap.Sample(LinearSampler, input.pos.xy);
		//check if offsetVector is above surface and within angle bias, if not, flip
		if(dot(pixelNormalV, offsetVector) < angleBias)
		{
			offsetVector = -offsetVector;
		}
		offsetPos[i] = float3(viewPos + offsetVector, 1.0f));
	}

	//remap to screen space
	[unroll]
	for(uint i = 0; i < nrOfSamples; i++)
	{
		offsetPos.xy = viewSpaceToScreenSpace(offsetPos[i]);
	}

	//compare depth of offset positions with the depth of the pixel on that xy-position
	float dist[nrOfSamples];
	float occlusion = 0.0f;
	[unroll]
	for(uint i = 0; i < nrOfSamples; i++)
	{
		pixelDepth = depthMap.Sample(LinearSampler, float2(offsetPos[i].x, offsetPos[i].y));
		dist[i] = pixelDepth - offsetPos.z; //get depth-distance between pixel position and offset position
		//if distance is positive, then the offset position is infront of the pixel
		if(dist[i] > DEPTH_EPSILON) //** ? 0 : 1;** use epsilon to avoid using samples behind the pixel
		{
			occlusion += pow(dist[i], 2.0f);
		}
	}
	occlusion /= nrOfSamples;
	
	//todo: occlusion function & blur
	//**blur - separate pass/shader**

	return float4(0,1,0,1) - occlusion * float4(1,1,1,1); //debug, **neg, värden**
}

/*
//-----------------------------------------------------------------------------------------
//	Technique
//-----------------------------------------------------------------------------------------
technique11 Tech
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_4_0, VSScene()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0, PSScene()));
	    
		SetDepthStencilState(DepthDisabled, 0);
	    SetRasterizerState(NoCulling);
		SetBlendState(NoBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff); //**
    }  
}
*/