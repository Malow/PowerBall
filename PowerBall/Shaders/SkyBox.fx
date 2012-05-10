

cbuffer EveryFrame
{
	matrix gWVP;
	matrix worldMatrix;
}


struct VSIn
{
	float3 Pos : POSITION;
	float2 Texcoord : TEXCOORD;
	float3 Normal : NORMAL;
};

struct SKYMAP_VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float4 WorldPos : POSITION;
	float3 texCoord : TEXCOORD;
};

struct PSout
{
	float4 Texture : SV_TARGET0;
	float4 NormalAndDepth : SV_TARGET1;
	float4 Position : SV_TARGET2;
	float4 Specular : SV_TARGET3;
};

RasterizerState RastDesc
{
	CullMode = None;
};

DepthStencilState EnableDepth
{
	DepthEnable = TRUE;
	DepthWriteMask = ZERO;
	DepthFunc = LESS_EQUAL;
};

TextureCube SkyMap;

SamplerState linearSampler 
{
	Filter = MIN_MAG_MIP_LINEAR;
};

SKYMAP_VS_OUTPUT SKYMAP_VS(VSIn input)
{
	SKYMAP_VS_OUTPUT output = (SKYMAP_VS_OUTPUT)0;

	//Set Pos to xyww instead of xyzw, so that z will always be 1 (furthest from camera)
	output.Pos = mul(float4(input.Pos, 1.0f), gWVP).xyww;
	output.WorldPos = mul(float4(input.Pos, 1.0f), worldMatrix);

	output.texCoord = input.Pos;

	return output;
}

PSout SKYMAP_PS(SKYMAP_VS_OUTPUT input) : SV_Target
{
	PSout output;
	output.Texture = SkyMap.Sample(linearSampler, input.texCoord);
	output.NormalAndDepth = float4(0, 0, 0, 1.5f);
	output.Position = input.WorldPos;
	output.Specular = float4(0, 0, 0, 1.0f);
	return output;
}

technique11 BasicTech
{
    pass p0
    {
		// Set VS, GS, and PS
        SetVertexShader( CompileShader( vs_4_0, SKYMAP_VS() ) );

        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, SKYMAP_PS() ) );
		
		SetDepthStencilState( EnableDepth, 0 );
	    SetRasterizerState( RastDesc );
    } 
}