#pragma once

#include "stdafx.h"

struct Font
{
	ID3D11ShaderResourceView* texture;
	int charTexCoords[256];
	int charWidth[256];

	Font()
	{
		for(int i = 0; i < 256; i++)
		{
			this->charTexCoords[i] = 0;
			this->charWidth[i] = 0;
		}
	}
};

class Text
{
private:
	string text;
	D3DXVECTOR2 position;
	float size;
	Font* font;

	void ToUpper();

public:
	Text(string text, D3DXVECTOR2 position, float size);
	virtual ~Text();
	
	void SetText(string newText) { this->text = newText; this->ToUpper(); }
	string GetText() const { return this->text; }
	void AppendText(string app) { this->text += app; this->ToUpper(); }
	void DeleteFromEnd(int CharsToDel);
	void SetPosition(D3DXVECTOR2 pos) { this->position = pos; }
	D3DXVECTOR2 GetPosition() const { return this->position; }
	void SetSize(float size) { this->size = size; }
	float GetSize() const { return this->size; }
	void SetFont(Font* newFont) { this->font = newFont; }
	Font* GetFont() const { return this->font; }
	
};


/*
#define FONTBUFFER_SIZE (64*1024)

struct FontVertex
{
	float x, y, x2, y2, w, h;
	FontVertex()
	{
		x = y = x2 = y2 = w = h = 0.0f;
	};
	FontVertex(float _x, float _y, float _x2, float _y2, float _w, float _h)
	{
		x = _x;
		y = _y;
		x2 = _x2;
		y2 = _y2;
		w = _w;
		h = _h;
	};
};

struct FontBuffer
{
	D3DXVECTOR2 pos;
	D3DXVECTOR2 texC;

	FontBuffer()
	{
	};
	FontBuffer(D3DXVECTOR2 _pos, D3DXVECTOR2 _texC)
	{
		pos = _pos;
		texC = _texC;
	};
};

struct Font
{
	FontVertex *chars[256];
	float pre[256];
	float post[256];
	int texture;

	float height;
};

const char FNTHEADERTAG[] = "[HGEFONT]";
const char FNTBITMAPTAG[] = "Bitmap";
const char FNTCHARTAG[]   = "Char";

//Cpp

UINT GetTextQuads(const char *str)
{
	UINT n = 0;
	while(*str)
	{
		if(*str != '\n')
			n++;
		str++;
	}
	return n;
}

char *_get_line(char *file, char *line)
{
	int i=0;

	if(!file[i]) return 0;

	while(file[i] && file[i]!='\n' && file[i]!='\r')
	{
		line[i]=file[i];
		i++;
	}
	line[i]=0;

	while(file[i] && (file[i]=='\n' || file[i]=='\r')) i++;

	return file + i;
}

bool VCE_DX11::LoadFont(const char *fontname, Font *font)
{
	DWORD	size;
	char	*desc, *pdesc;
	char	linebuf[256];
	char	buf[MAX_PATH], *pbuf;
	char	chr;
	int		i, x, y, w, h, a, c;

	float xTemp;
	float yTemp;

	float x2Temp;
	float y2Temp;
	
	//Load font description
	FILE *file = NULL;	
	fopen_s(&file, fontname, "rb");

	if(file == NULL)
		return false;

	fseek (file , 0 , SEEK_END);
	size = ftell (file);
	rewind (file);

	//Allocate memory to contain the whole file
	desc = new char[size+1];

	size_t result;
	result = fread(desc, 1, size, file);
	if(result != size)
		return false;

	desc[size]=0;

	fclose(file);

	pdesc=_get_line(desc,linebuf);
	if(strcmp(linebuf, FNTHEADERTAG))
	{
		delete[] desc;	
		return false;
	}

	//Parse font description
	POINT texSize;
	while(pdesc = _get_line(pdesc,linebuf))
	{
		if(!strncmp(linebuf, FNTBITMAPTAG, sizeof(FNTBITMAPTAG)-1 ))
		{
			strcpy_s(buf, sizeof(buf), fontname);
			pbuf=strrchr(buf,'\\');
			if(!pbuf) pbuf=strrchr(buf,'/');
			if(!pbuf) pbuf=buf;
			else pbuf++;
			if(!sscanf(linebuf, "Bitmap = %s", pbuf))
				continue;

			font->texture = AddTexture(buf);
			GetTextureSize(font->texture, &texSize);
		}
		else if(!strncmp(linebuf, FNTCHARTAG, sizeof(FNTCHARTAG)-1 ))
		{
			pbuf=strchr(linebuf,'=');
			if(!pbuf) continue;
			pbuf++;
			while(*pbuf==' ') pbuf++;
			if(*pbuf=='\"')
			{
				pbuf++;
				i=(unsigned char)*pbuf++;
				pbuf++; // skip "
			}
			else
			{
				i=0;
				while((*pbuf>='0' && *pbuf<='9') || (*pbuf>='A' && *pbuf<='F') || (*pbuf>='a' && *pbuf<='f'))
				{
					chr=*pbuf;
					if(chr >= 'a') chr-='a'-':';
					if(chr >= 'A') chr-='A'-':';
					chr-='0';
					if(chr>0xF) chr=0xF;
					i=(i << 4) | chr;
					pbuf++;
				}
				if(i<0 || i>255) continue;
			}
			sscanf(pbuf, " , %d , %d , %d , %d , %d , %d", &x, &y, &w, &h, &a, &c);

			xTemp = ((float)x)/((float)texSize.x);
			yTemp = ((float)y)/((float)texSize.y);

			x2Temp = ((float)(x+w))/((float)texSize.x);
			y2Temp = ((float)(y+h))/((float)texSize.y);

			font->chars[i] = new FontVertex(xTemp, yTemp, x2Temp, y2Temp, (float)w, (float)h);
			font->pre[i]=(float)a;
			font->post[i]=(float)c;
			if(h>font->height)
				font->height=(float)h;
		}
	}
	delete [] desc;

	return true;
}

const char *texDefs =
"struct VsIn\n"
"{\n"
"	float4 position : POSITION;\n"
"	float2 texCoord : TEXCOORD;\n"
"};\n"
"struct PsIn\n"
"{\n"
"	float4 position: SV_Position;\n"
"	float2 texCoord: TEXCOORD;\n"
"};\n";

const char *texVS =
"float4 scaleBias;\n"
"PsIn VS(VsIn In)\n"
"{\n"
"	PsIn Out;\n"
"	Out.position = In.position;\n"
"	Out.position.xy = Out.position.xy * scaleBias.xy + scaleBias.zw;\n"
"	Out.texCoord = In.texCoord;\n"
"	return Out;\n"
"}\n";

const char *texPS =
"Texture2D Diffuse: register(t0);\n"
"SamplerState linearSample : register(s0);\n"
"float4 color;\n"
"float xStartMask = 0;\n"
"float yStartMask = 0;\n"
"float xEndMask = 2000;\n"
"float yEndMask = 2000;\n"
"float4 PS(PsIn In) : SV_Target0\n"
"{\n"
"	if(In.position.x < xStartMask || In.position.y < yStartMask || In.position.x > xEndMask || In.position.y > yEndMask)\n"
"		return float4(1,1,1,0);\n"
"	return Diffuse.Sample(linearSample, In.texCoord) * color;\n"
"}";

FontID VCE_DX11::AddFont(const char *fontname)
{
	Font font;
	memset(&font, 0, sizeof(Font));
	memset(&font.chars, 0, sizeof(font.chars));
	memset(&font.pre, 0, sizeof(font.pre));
	memset(&font.post, 0, sizeof(font.post));

	bool finished = LoadFont(fontname, &font);

	if(!finished)
	{
		Log("[VCERenderer] Failed: AddFont\n");
		return FONT_NONE;
	}
	if(fontVB == -2)
	{
		fontVB = AddVertexBuffer(FONTBUFFER_SIZE, DYNAMIC);
	}
	if(fontShader == -2)
	{
		fontShader = AddShader("Font", texVS, NULL, NULL, NULL, texPS, NULL, 0, 0, 0, 0, 0, 0, texDefs);
	}
	if(sampler == -2)
	{
		sampler = AddSamplerState(VCE_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR, VCE_TEXTURE_CLAMP, VCE_TEXTURE_CLAMP, VCE_TEXTURE_CLAMP);
	}
	if(fontIL == -2)
	{
		InputDesc format[] =
		{
			0, TYPE_VERTEX,   FORMAT_FLOAT, 2,
			0, TYPE_TEXCOORD, FORMAT_FLOAT, 2,
		};

		fontIL = AddInputLayout(format, 2, fontShader);
	}
	if(depthState == -2)
	{
		depthState = AddDepthState(false);
	}
	if(blendState == -2)
	{
		VCE_BLEND_DESC blendDesc;
		memset(&blendDesc, 0, sizeof(VCE_BLEND_DESC));

		blendDesc.RenderTargetDesc[0].BlendEnable = TRUE;
		blendDesc.RenderTargetDesc[0].SrcBlend = VCE_BLEND_SRC_ALPHA;
		blendDesc.RenderTargetDesc[0].DestBlend = VCE_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTargetDesc[0].BlendOp = VCE_BLEND_OP_ADD;
		blendDesc.RenderTargetDesc[0].SrcBlendAlpha = VCE_BLEND_ZERO;
		blendDesc.RenderTargetDesc[0].DestBlendAlpha = VCE_BLEND_ZERO;
		blendDesc.RenderTargetDesc[0].BlendOpAlpha = VCE_BLEND_OP_ADD;
		blendDesc.RenderTargetDesc[0].RenderTargetWriteMask = 0x0F;

		blendState = AddBlendState(&blendDesc);
	}

	return m_Fonts.add(font);
}

void VCE_DX11::SetBias2D(float x, float y, float w, float h)
{
	m_ScaleBias2D.x = 2.0f / (w - x);
	m_ScaleBias2D.y = 2.0f / (y - h);
	m_ScaleBias2D.z = -1.0f;
	m_ScaleBias2D.w =  1.0f;
}
void VCE_DX11::DrawText(FontID font, float x, float y, VCE_TEXT_ALIGN align, D3DXVECTOR4 *color, D3DXVECTOR4 *mask, float scale, const char* text)
{
	UINT nrOf = 6*GetTextQuads(text);
	if(nrOf == 0)
		return;

	int size = nrOf * sizeof(FontBuffer);
	if(size >= FONTBUFFER_SIZE)
		return;

	if(nrOf > m_BufferCount)
	{
		DeletePointer(m_Buffer);
		m_Buffer = new FontBuffer[nrOf];
		m_BufferCount = nrOf;
	}

	FillBuffer(font, m_Buffer, text, x, y, align, scale);

	CopyToDynamicBuffer(BUFFER_VERTEX, fontVB, m_Buffer, sizeof(FontBuffer)*nrOf);

	D3DXVECTOR4 l_Col(1, 1, 1, 1);
	D3DXVECTOR4 l_Mask(0, 0, 2000, 2000);

	if(color)
		l_Col = D3DXVECTOR4(color->x, color->y, color->z, color->w);

	if(mask)
		l_Mask = D3DXVECTOR4(mask->x, mask->y, mask->z, mask->w);

	Reset();

	SetShader(fontShader);
	SetTexture("Diffuse", m_Fonts[font].texture);
	SetSamplerState("linearSample", sampler);

	SetShaderVec4("scaleBias", m_ScaleBias2D);
	SetShaderVec4("color", l_Col);
	
	SetShaderFloat("xStartMask", l_Mask.x);
	SetShaderFloat("yStartMask", l_Mask.y);
	SetShaderFloat("xEndMask", l_Mask.z);
	SetShaderFloat("yEndMask", l_Mask.w);

	SetBlendState(blendState);
	SetDepthState(depthState);
	SetInputLayout(fontIL);
	SetVertexBuffer(0, fontVB);
	Apply();

	Draw(PRIMITIVES_TRIANGLES, nrOf, 0);
}

float VCE_DX11::GetStringWidth(FontID font, const char *string, float scale, bool multiline) const
{
	int i;
	float linew, width = 0;

	while(*string)
	{
		linew = 0;

		while(*string && *string != '\n')
		{
			i=(unsigned char)*string;
			if(!m_Fonts[font].chars[i]) i='?';
			if(m_Fonts[font].chars[i])
				linew += m_Fonts[font].chars[i]->w + m_Fonts[font].pre[i] + m_Fonts[font].post[i];

			string++;
		}

		if(!multiline) return linew * scale;

		if(linew > width)
			width = linew;

		while (*string == '\n' || *string == '\r')
			string++;
	}
	return width * scale;
}

float VCE_DX11::GetFontHeight(FontID font, float scale)
{
	return m_Fonts[font].height * scale;
}

void VCE_DX11::FillBuffer(FontID font, FontBuffer *buffer, const char *text, float x, float y, VCE_TEXT_ALIGN align, float scale)
{
	float startx = x;
	int i;
	
	if(align == VCE_CENTER)
		startx -= int(GetStringWidth(font, text, scale, false)/2.0f);

	if(align == VCE_RIGHT)
		startx -= GetStringWidth(font, text, scale, false);

	while(*text)
	{
		if(*text == '\n')
		{
			y += m_Fonts[font].height * scale;
			startx = x;
			
			if(align == VCE_CENTER)
				startx -= int(GetStringWidth(font, text+1, scale, false)/2.0f);

			if(align == VCE_RIGHT)
				startx -= GetStringWidth(font, text+1, scale, false);
		}
		else
		{
			i = (UCHAR)*text;

			if(!m_Fonts[font].chars[i])
				i = '?';
			if(m_Fonts[font].chars[i])
			{

				//First triangle
				buffer[0].pos = D3DXVECTOR2(startx, y);
				buffer[0].texC = D3DXVECTOR2(m_Fonts[font].chars[i]->x, m_Fonts[font].chars[i]->y);

				buffer[1].pos = D3DXVECTOR2(startx+(m_Fonts[font].chars[i]->w*scale), y);
				buffer[1].texC = D3DXVECTOR2(m_Fonts[font].chars[i]->x2, m_Fonts[font].chars[i]->y);

				buffer[2].pos = D3DXVECTOR2(startx, y+(m_Fonts[font].height*scale));
				buffer[2].texC = D3DXVECTOR2(m_Fonts[font].chars[i]->x, m_Fonts[font].chars[i]->y2);

				//Second triangle
				buffer[3].pos = D3DXVECTOR2(startx, y+(m_Fonts[font].height*scale));
				buffer[3].texC = D3DXVECTOR2(m_Fonts[font].chars[i]->x, m_Fonts[font].chars[i]->y2);

				buffer[4].pos = D3DXVECTOR2(startx+(m_Fonts[font].chars[i]->w*scale), y);
				buffer[4].texC = D3DXVECTOR2(m_Fonts[font].chars[i]->x2, m_Fonts[font].chars[i]->y);

				buffer[5].pos = D3DXVECTOR2(startx+(m_Fonts[font].chars[i]->w*scale), y+(m_Fonts[font].height*scale));
				buffer[5].texC = D3DXVECTOR2(m_Fonts[font].chars[i]->x2, m_Fonts[font].chars[i]->y2);

				buffer += 6;

				startx += (m_Fonts[font].chars[i]->w + m_Fonts[font].post[i]) * scale;
				startx += m_Fonts[font].pre[i] * scale;
			}
		}
		text++;
	}
}

bool VCE_DX11::IsRunning()
{
	return m_bRunning;
}

bool VCE_DX11::IsWindowed()
{
	return m_bWindowed;
}

void VCE_DX11::ChangeVerticalSync(bool vsync)
{
	m_vSync = (vsync ? 1 : 0);
}
HRESULT VCE_DX11::ChangeWindowState(bool windowed)
{
	if(m_NrOfHwnd > 0)
		return VCE_NOTSUPPORTED;

	HRESULT hr = m_SwapChain[0]->SetFullscreenState( (windowed ? 0 : 1), NULL);
	if(FAILED(hr))
	{
		Log("[VCERenderer] Error: Something went wrong when fullscreen state was changed\n");
		return VCE_FAIL;
	}

	m_bWindowed = windowed;

	return VCE_OK;
}
HRESULT VCE_DX11::GetDisplayModes(UINT *pNumModes, DISPLAY_MODE_DESC *pDesc)
{
	if(pDesc == NULL)
	{
		*pNumModes = m_DisplayModes.getCount();
	}
	else
	{
		if(*pNumModes < m_DisplayModes.getCount())
			return VCE_BUFFERSIZE;

		memcpy(&pDesc[0], &m_DisplayModes[0], sizeof(DISPLAY_MODE_DESC)*(m_DisplayModes.getCount()));

		*pNumModes = m_DisplayModes.getCount();
	}

	return VCE_OK;
*/