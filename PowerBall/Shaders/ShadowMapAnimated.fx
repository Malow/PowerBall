cbuffer EveryObject
{
	float t;
	matrix LightWVP;
};

struct VSIn
{
	float3 pos		: POSITION;
	float3 posMorph : POSITION;
};

struct PSIn
{
	float4 pos : SV_POSITION;
};

float4 VS(VSIn input) : SV_POSITION
{
	return mul(lerp(float4(input.pos, 1.0f), float4(input.posMorph, 1.0f), t), LightWVP);
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