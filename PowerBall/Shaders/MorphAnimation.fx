//-----------------------------------------------------------------------------------------
//	Todo: description & implementation**
//-----------------------------------------------------------------------------------------


struct VS_IN
{
	float3 posL          : POSITION;
	float3 normalL	     : NORMAL;
	float2 texC			 : TEXCOORD;

	float3 posL_Morph    : POSITION_MORPH;
	float3 normalL_Morph : NORMAL_MORPH;
	float2 texC_Morph    : TEXCOORD_MORPH;
};