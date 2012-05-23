// Marcus Löwegren

// For textures
Texture2D tex2D;
SamplerState linearSampler
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};

//blend state
BlendState SrcAlphaBlendingAdd 
{ 
	BlendEnable[0] = TRUE; 
	SrcBlend = SRC_ALPHA; 
	DestBlend = INV_SRC_ALPHA; 
	BlendOp = ADD; 
	SrcBlendAlpha = ZERO; 
	DestBlendAlpha = ZERO; 
	BlendOpAlpha = ADD; 
	RenderTargetWriteMask[0] = 0x0F; 
}; 

DepthStencilState DisableDepthWrite
{
    DepthEnable = FALSE;
    DepthWriteMask = ZERO;
};

cbuffer EveryFrame
{
	float windowWidth;
	float windowHeight;

	float posx;
	float posy;
	float size;
}

cbuffer Once
{
	int charTex[70];
	int charWidth[70];
}

cbuffer EveryString
{
	int NrOfChars;
	int text[40];
};

//-----------------------------------------------------------------------------------------
// Input and Output Structures
//-----------------------------------------------------------------------------------------

struct VSIn
{
	float4 Pos : POSITION;
	float2 dimensions : DIMENSIONS;
	float opacity : OPACITY;
};

struct PSSceneIn
{
	float4 Pos : SV_POSITION;
	float2 tex : TEXCOORD;
};

//-----------------------------------------------------------------------------------------
// State Structures
//-----------------------------------------------------------------------------------------
RasterizerState NoCulling
{
	CullMode = NONE;
};


//-----------------------------------------------------------------------------------------
// VertexShader: VSScene
//-----------------------------------------------------------------------------------------
VSIn VSScene(VSIn input)
{
	return input;
}

// GS
[maxvertexcount(160)]
void GS(point VSIn input[1], inout TriangleStream<PSSceneIn> triStream)
{
	PSSceneIn output;
	float4 basepos = float4(posx, posy, 0, 1);


	float imgWidth = 0.0f;
	float imgHeight = 0.0f;
	tex2D.GetDimensions(imgWidth, imgHeight);

	
	float dimy = size * imgHeight;
	float height = (dimy / windowHeight);
	float scale = windowHeight / 950.0f; // Haxfix, text fits at 950 height
	height *= scale;


	float width = 0.0f;
	for(int i = 0; i < NrOfChars; i++)
	{
		float dimx = size * charWidth[text[i]];
		float thiswidth = (dimx / windowWidth) * scale;

		float texxwidth = (charWidth[text[i]] / imgWidth);
		float texx = charTex[text[i]] / imgWidth;

		//bottom left
		output.Pos = basepos + float4(width, -height, 0, 0);
		output.tex = float2(texx, 1);
		triStream.Append(output);
		
		//bottom right
		output.Pos = basepos + float4(width + thiswidth, -height, 0, 0);
		output.tex = float2(texx + texxwidth, 1);
		triStream.Append(output);

		//top left
		output.Pos = basepos + float4(width, 0, 0, 0);
		output.tex = float2(texx, 0);
		triStream.Append(output);

		//top right
		output.Pos = basepos + float4(width + thiswidth, 0, 0, 0);
		output.tex = float2(texx + texxwidth, 0);
		triStream.Append(output);
		triStream.RestartStrip();

		width += thiswidth;
	}
}



//-----------------------------------------------------------------------------------------
// PixelShader: PSSceneMain
//-----------------------------------------------------------------------------------------
float4 PSScene(PSSceneIn input) : SV_Target
{	
	float4 tex = tex2D.Sample(linearSampler, input.tex);
	return tex;
}

//-----------------------------------------------------------------------------------------
// Technique: RenderTextured  
//-----------------------------------------------------------------------------------------
technique11 BasicTech
{
    pass p0
    {
		// Set VS, GS, and PS
        SetVertexShader( CompileShader( vs_4_0, VSScene() ) );
        SetGeometryShader( CompileShader( gs_4_0, GS() ) );
        SetPixelShader( CompileShader( ps_4_0, PSScene() ) );
	    

		SetDepthStencilState( DisableDepthWrite, 0 );
	    SetRasterizerState( NoCulling );
		SetBlendState( SrcAlphaBlendingAdd, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF );
    }  
}