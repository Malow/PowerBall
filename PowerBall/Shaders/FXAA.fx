//-----------------------------------------------------------------------------------------
//	Post-process effect: Fast Approximate Anti-Aliasing (FXAA).
//	Reduces all aliasing while maintaining sharpness at a low cost.	
//	Requirements: Scene buffer as input texture.
//	Notes: This is a stripped and slightly modified version of the one provided by NVIDIA. **
//-----------------------------------------------------------------------------------------
#include "stdafx.fx"

//-----------------------------------------------------------------------------------------
//	Global variables (non-numeric values cannot be added to a constantbuffer.)
//-----------------------------------------------------------------------------------------
Texture2D sceneTex;

//-----------------------------------------------------------------------------------------
// Constant buffers
//-----------------------------------------------------------------------------------------
cbuffer Rarely
{
	float FXAA_EDGE_THRESHOLD;
	float FXAA_EDGE_THRESHOLD_MIN;
	int FXAA_SEARCH_STEPS;
	int FXAA_SEARCH_ACCELERATION;
	float FXAA_SEARCH_THRESHOLD;
	int FXAA_SUBPIX;
	int FXAA_SUBPIX_FASTER;
	float FXAA_SUBPIX_CAP;
	float FXAA_SUBPIX_TRIM;
	float FXAA_SUBPIX_TRIM_SCALE;
};

//-----------------------------------------------------------------------------------------
// State structures 
//-----------------------------------------------------------------------------------------
DepthStencilState DisableDepth
{
    DepthEnable = FALSE;
    DepthWriteMask = ZERO;
};
RasterizerState NoCulling
{
	CullMode = NONE;
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
// Other structures 
//-----------------------------------------------------------------------------------------
struct FxaaTex 
{ 
	SamplerState smpl; 
	Texture2D tex; 
};

//-----------------------------------------------------------------------------------------
// Functions
//-----------------------------------------------------------------------------------------
float3 FxaaToFloat3(float a)
{
	return float3(a, a, a);
}
float4 FxaaTexLod0(FxaaTex tex, float2 pos) 
{
	return tex.tex.SampleLevel(tex.smpl, pos.xy, 0.0);
}
float4 FxaaTexGrad(FxaaTex tex, float2 pos, float2 grad) 
{
    return tex.tex.SampleGrad(tex.smpl, pos.xy, grad, grad);
}
float4 FxaaTexOff(FxaaTex tex, float2 pos, int2 off, float2 rcpFrame) 
{
    return tex.tex.SampleLevel(tex.smpl, pos.xy, 0.0, off.xy);
}
float FxaaLuma(float3 rgb) 
{
    return rgb.y * (0.587/0.299) + rgb.x; 
} 
float3 FxaaLerp3(float3 a, float3 b, float amountOfA) 
{
	return (FxaaToFloat3(-amountOfA) * b) + ((a * FxaaToFloat3(amountOfA)) + b); 
} 

//-----------------------------------------------------------------------------------------
// Vertex shader
//-----------------------------------------------------------------------------------------
float4 VSScene(uint vertexID : SV_VertexID) : SV_Position
{
	float4 pos = float4(0,0,0,0); //positions are in clip space [-1,1]
	
	if(vertexID == 0)
	{
		pos = float4(-1.0f, 1.0f, 0.0f, 1.0f);
	}
	else if(vertexID == 1)
	{
		pos = float4(-1.0f, -1.0f, 0.0f, 1.0f);
	}
	else if(vertexID == 2)
	{
		pos = float4(1.0f, 1.0f, 0.0f, 1.0f);
	}
	else if(vertexID == 3)
	{
		pos = float4(1.0f, -1.0f, 0.0f, 1.0f); 
	}

	return pos;
}

//------------------------------------------------------------------------------------------------------
//	Pixel Shader function
//------------------------------------------------------------------------------------------------------
float3 FxaaPixelShader(float2 pos, FxaaTex tex, float2 rcpFrame) 
{
    float3 rgbN = FxaaTexOff(tex, pos.xy, int2(0,-1), rcpFrame).xyz;
    float3 rgbW = FxaaTexOff(tex, pos.xy, int2(-1, 0), rcpFrame).xyz;
    float3 rgbM = FxaaTexOff(tex, pos.xy, int2( 0, 0), rcpFrame).xyz;
    float3 rgbE = FxaaTexOff(tex, pos.xy, int2( 1, 0), rcpFrame).xyz;
    float3 rgbS = FxaaTexOff(tex, pos.xy, int2( 0, 1), rcpFrame).xyz;
    float lumaN = FxaaLuma(rgbN);
    float lumaW = FxaaLuma(rgbW);
    float lumaM = FxaaLuma(rgbM);
    float lumaE = FxaaLuma(rgbE);
    float lumaS = FxaaLuma(rgbS);
    float rangeMin = min(lumaM, min(min(lumaN, lumaW), min(lumaS, lumaE)));
    float rangeMax = max(lumaM, max(max(lumaN, lumaW), max(lumaS, lumaE)));
    float range = rangeMax - rangeMin;
	float3 rgbL;
    if(range < max(FXAA_EDGE_THRESHOLD_MIN, rangeMax * FXAA_EDGE_THRESHOLD)) {
        return rgbM; }
    if (FXAA_SUBPIX > 0)
        if (FXAA_SUBPIX_FASTER)
            rgbL = (rgbN + rgbW + rgbE + rgbS + rgbM) * FxaaToFloat3(1.0/5.0);
        else
            rgbL = rgbN + rgbW + rgbM + rgbE + rgbS;
        //#endif
    //#endif        
    float rangeL;
    if( FXAA_SUBPIX != 0)
	{
        float lumaL = (lumaN + lumaW + lumaE + lumaS) * 0.25;
        rangeL = abs(lumaL - lumaM);
	}
    //#endif        
	float blendL;
    if( FXAA_SUBPIX == 1)
	{
        blendL = max(0.0, (rangeL / range) - FXAA_SUBPIX_TRIM) * FXAA_SUBPIX_TRIM_SCALE; 
        blendL = min(FXAA_SUBPIX_CAP, blendL);
	}
    //#endif
    if( FXAA_SUBPIX == 2)
        blendL = rangeL / range; 
    //#endif
    
    float3 rgbNW = FxaaTexOff(tex, pos.xy, int2(-1,-1), rcpFrame).xyz;
    float3 rgbNE = FxaaTexOff(tex, pos.xy, int2( 1,-1), rcpFrame).xyz;
    float3 rgbSW = FxaaTexOff(tex, pos.xy, int2(-1, 1), rcpFrame).xyz;
    float3 rgbSE = FxaaTexOff(tex, pos.xy, int2( 1, 1), rcpFrame).xyz;
    if( (FXAA_SUBPIX_FASTER == 0) && (FXAA_SUBPIX > 0))
	{
        rgbL += (rgbNW + rgbNE + rgbSW + rgbSE);
        rgbL *= FxaaToFloat3(1.0/9.0);
	}
    //#endif
    float lumaNW = FxaaLuma(rgbNW);
    float lumaNE = FxaaLuma(rgbNE);
    float lumaSW = FxaaLuma(rgbSW);
    float lumaSE = FxaaLuma(rgbSE);
    float edgeVert = 
        abs((0.25 * lumaNW) + (-0.5 * lumaN) + (0.25 * lumaNE)) +
        abs((0.50 * lumaW ) + (-1.0 * lumaM) + (0.50 * lumaE )) +
        abs((0.25 * lumaSW) + (-0.5 * lumaS) + (0.25 * lumaSE));
    float edgeHorz = 
        abs((0.25 * lumaNW) + (-0.5 * lumaW) + (0.25 * lumaSW)) +
        abs((0.50 * lumaN ) + (-1.0 * lumaM) + (0.50 * lumaS )) +
        abs((0.25 * lumaNE) + (-0.5 * lumaE) + (0.25 * lumaSE));
    bool horzSpan = edgeHorz >= edgeVert;

    float lengthSign = horzSpan ? -rcpFrame.y : -rcpFrame.x;
    if(!horzSpan) lumaN = lumaW;
    if(!horzSpan) lumaS = lumaE;
    float gradientN = abs(lumaN - lumaM);
    float gradientS = abs(lumaS - lumaM);
    lumaN = (lumaN + lumaM) * 0.5;
    lumaS = (lumaS + lumaM) * 0.5;
       
    bool pairN = gradientN >= gradientS;


    if(!pairN) lumaN = lumaS;
    if(!pairN) gradientN = gradientS;
    if(!pairN) lengthSign *= -1.0;
    float2 posN;
    posN.x = pos.x + (horzSpan ? 0.0 : lengthSign * 0.5);
    posN.y = pos.y + (horzSpan ? lengthSign * 0.5 : 0.0);
    
    gradientN *= FXAA_SEARCH_THRESHOLD;

    float2 posP = posN;
    float2 offNP = horzSpan ? 
        float2(rcpFrame.x, 0.0) :
        float2(0.0f, rcpFrame.y); 
    float lumaEndN = lumaN;
    float lumaEndP = lumaN;
    bool doneN = false;
    bool doneP = false;
    if( FXAA_SEARCH_ACCELERATION == 1)
	{
        posN += offNP * float2(-1.0, -1.0);
        posP += offNP * float2( 1.0,  1.0);
	}
    //#endif
    if( FXAA_SEARCH_ACCELERATION == 2)
	{
        posN += offNP * float2(-1.5, -1.5);
        posP += offNP * float2( 1.5,  1.5);
        offNP *= float2(2.0, 2.0);
	}
    //#endif
    if( FXAA_SEARCH_ACCELERATION == 3)
	{
        posN += offNP * float2(-2.0, -2.0);
        posP += offNP * float2( 2.0,  2.0);
        offNP *= float2(3.0, 3.0);
	}
    //#endif
    if( FXAA_SEARCH_ACCELERATION == 4)
	{
        posN += offNP * float2(-2.5, -2.5);
        posP += offNP * float2( 2.5,  2.5);
        offNP *= float2(4.0, 4.0);
	}
    //#endif
    for(int i = 0; i < FXAA_SEARCH_STEPS; i++) {
        if( FXAA_SEARCH_ACCELERATION == 1)
		{
            if(!doneN) lumaEndN = 
                FxaaLuma(FxaaTexLod0(tex, posN.xy).xyz);
            if(!doneP) lumaEndP = 
                FxaaLuma(FxaaTexLod0(tex, posP.xy).xyz);
		}
        else
		{
            if(!doneN) lumaEndN = 
                FxaaLuma(FxaaTexGrad(tex, posN.xy, offNP).xyz);
            if(!doneP) lumaEndP = 
                FxaaLuma(FxaaTexGrad(tex, posP.xy, offNP).xyz);
		}	
        //#endif
        doneN = doneN || (abs(lumaEndN - lumaN) >= gradientN);
        doneP = doneP || (abs(lumaEndP - lumaN) >= gradientN);
        if(doneN && doneP) break;
        if(!doneN) posN -= offNP;
        if(!doneP) posP += offNP; }
    

    float dstN = horzSpan ? pos.x - posN.x : pos.y - posN.y;
    float dstP = horzSpan ? posP.x - pos.x : posP.y - pos.y;
    bool directionN = dstN < dstP;

    lumaEndN = directionN ? lumaEndN : lumaEndP;
    

    if(((lumaM - lumaN) < 0.0) == ((lumaEndN - lumaN) < 0.0)) 
        lengthSign = 0.0;
 

    float spanLength = (dstP + dstN);
    dstN = directionN ? dstN : dstP;
    float subPixelOffset = (0.5 + (dstN * (-1.0/spanLength))) * lengthSign;


    float3 rgbF = FxaaTexLod0(tex, float2(
        pos.x + (horzSpan ? 0.0 : subPixelOffset),
        pos.y + (horzSpan ? subPixelOffset : 0.0))).xyz;
    if( FXAA_SUBPIX == 0)
        return rgbF; 
    else        
        return FxaaLerp3(rgbL, rgbF, blendL); 
    //#endif
}

//-----------------------------------------------------------------------------------------
// Pixel Shader
//-----------------------------------------------------------------------------------------
float4 PSScene(float4 pos : SV_Position) : SV_Target
{	
	//optimera:**
	uint width, height;
	sceneTex.GetDimensions(width, height);
	
	float2 rcpFrame = float2(1.0f / width, 1.0f / height);
	float2 texCoord = float2(pos.x * rcpFrame.x, pos.y * rcpFrame.y);
	FxaaTex tex = {AnisotropicClampSampler, sceneTex};
	float4 color = float4(FxaaPixelShader(texCoord, tex, rcpFrame), 1.0f);
	
	return color; 

	/**
	SetSettings(FXAAPreset); //optimera**

	FxaaTex tex = {AnisotropicSampler, sceneTex};
	float2 texCoord = float2(pos.x / width, pos.y / height);
	float2 dxdy = float2(1.0f / width, 1.0f / height);
	float4 color = float4(FxaaPixelShader(texCoord, tex, dxdy), 1.0f);
	*/
}

//-----------------------------------------------------------------------------------------
// Technique
//-----------------------------------------------------------------------------------------
technique10 FXAATech
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_4_0, VSScene()));
		SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0, PSScene()));
       
	    SetRasterizerState(NoCulling);
	    SetDepthStencilState(DisableDepth, 0);
		SetBlendState(NoBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);
    }  
}