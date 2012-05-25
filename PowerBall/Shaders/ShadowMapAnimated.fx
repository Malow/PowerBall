cbuffer EveryObject
{
	float t;
	matrix LightWVP;
};

struct VSIn
{
	float4 Pos : POSITION;
	float2 tex : TEXCOORD;
	float3 norm : NORMAL;
	float4 Color : COLOR;

	float4 Pos_Morph : POSITION_MORPH;
	float2 tex_Morph : TEXCOORD_MORPH;
	float3 norm_Morph : NORMAL_MORPH;
	float4 Color_Morph : COLOR_MORPH;
};

struct PSIn
{
	float4 pos : SV_POSITION;
};

PSIn VS(VSIn input) : SV_POSITION
{
	PSIn output = (PSIn)0;
	output.pos = mul(lerp(input.Pos, input.Pos_Morph, t), LightWVP);
	return output;
}

BlendState NoBlend
{
	BlendEnable[0] = FALSE;
};

RasterizerState rs
{
	FillMode = Solid;
	CullMode = Front;
};

DepthStencilState EnableDepth
{
	DepthEnable = TRUE;
	DepthWriteMask = ALL;
	DepthFunc = LESS_EQUAL;
};

technique11 RenderShadowMap
{
	pass P0
	{
		SetVertexShader( CompileShader( vs_4_0, VS() ) );
		SetGeometryShader( NULL );
		SetPixelShader( NULL );
		SetDepthStencilState( EnableDepth, 0 );
		SetBlendState( NoBlend, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF );
		SetRasterizerState(rs);
	}
}