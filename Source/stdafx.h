//
// NEEDS REWRITE, HAVN'T WRITTEN MYSELF / MaloW
//

#ifndef _STDAFX__H
#define _STDAFX__H

enum GAMEMODE{
	NONE,
	CTF,
	DM,
	KOTH,
	WARLOCK,
	CREDITS,
	CREDITS2
};
enum TEAM{
	NOTEAM = 0,
	BLUETEAM = 1,
	REDTEAM = 2
};

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <D3D11.h>
#include <D3DX11.h>
#include <D3DX10math.h>
#include <d3dx11effect.h>
#include <d3dCompiler.h>

// Helpfiles from MaloW
#include "MaloW.h"
#include "MaloWFileDebug.h"
#include "Array.h"

#include <string>
#include <vector>
#include <map>
#include <fstream>

/*! constants */
const D3DXCOLOR WHITE(1.0f, 1.0f, 1.0f, 1.0f);
const D3DXCOLOR BLACK(0.0f, 0.0f, 0.0f, 1.0f);
const D3DXCOLOR RED(1.0f, 0.0f, 0.0f, 1.0f);
const D3DXCOLOR GREEN(0.0f, 1.0f, 0.0f, 1.0f);
const D3DXCOLOR BLUE(0.0f, 0.0f, 1.0f, 1.0f);
const D3DXCOLOR YELLOW(1.0f, 1.0f, 0.0f, 1.0f);
const D3DXCOLOR CYAN(0.0f, 1.0f, 1.0f, 1.0f);
const D3DXCOLOR MAGENTA(1.0f, 0.0f, 1.0f, 1.0f);
#define PI (3.14159265358979323846f)

enum COLOR
{
	NULL_COLOR = 0,
	WHITE_COLOR = 1,
	BLACK_COLOR = 2,
	RED_COLOR = 3,
	GREEN_COLOR = 4,
	BLUE_COLOR = 5,
	YELLOW_COLOR = 6,
	CYAN_COLOR = 7,
	MAGENTA_COLOR = 8
};

/*! functions */
#define SAFE_RELEASE(x) { if(x) { (x)->Release(); (x) = NULL; } }
#define SAFE_DELETE(x) { if(x) { delete(x); (x) = NULL; } }
#define SAFE_DELETE_ARRAY(x) if( x ) { delete[](x); (x) = NULL; }
/*! returns random float between [0,1] */
inline float RndFloat() 
{
	//rand() = between 0 and RAND_MAX (32767)
	return (float)(rand()) / (float)RAND_MAX;
}
/*! returns random float between [a,b] */
inline float RndFloat(float a, float b)
{
	return a + RndFloat() * (b - a);
}
/*! returns random D3DXVECTOR3 with x,y,z between [-1,1] where x^2 + y^2 + z^2 = 1 */
inline D3DXVECTOR3 RndUnitVec3()
{
	D3DXVECTOR3 v = D3DXVECTOR3(RndFloat(-1.0f, 1.0f), RndFloat(-1.0f, 1.0f), RndFloat(-1.0f, 1.0f));
	D3DXVec3Normalize(&v, &v);

	return v;
}
/*! returns the angle in radian*/
inline float DegreesToRadian(float degrees)
{
	return (PI / 180) * degrees;
}
/*! returns the angle in degrees*/
inline float RadianToDegrees(float radian)
{
	return (180 / PI) * radian;
}
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment (lib,"dxerr.lib")

#ifdef _DEBUG
	/*! Graphic debug libs */
	#pragma comment(lib, "d3dx11d.lib")
	#pragma comment(lib, "Effects11D.lib")
	#pragma comment(lib, "d3dx10d.lib")
	/*! Sound debug libs */
	/*! 64 bit */
	#if _WIN64 || __amd64__ //**
		#pragma comment(lib, "fmodexL64_vc.lib")
	/*! 32 bit */
	#else
		#pragma comment(lib, "fmodexL_vc.lib") 
	#endif
#else
	/*! Graphic libs */
	#pragma comment(lib, "d3dx11.lib")
	#pragma comment(lib, "Effects11.lib")
	#pragma comment(lib, "d3dx10.lib")
	/*! Sound libs */
	/*! 64 bit */
	#if _WIN64 || __amd64__ //**
		#pragma comment(lib, "fmodex64_vc.lib")
	/*! 32 bit */
	#else
		#pragma comment(lib, "fmodex_vc.lib")
	#endif
#endif

//////////////////////////////////////////////////////////////////////////
// to find memory leaks
//////////////////////////////////////////////////////////////////////////
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifdef _DEBUG
#define myMalloc(s)       _malloc_dbg(s, _NORMAL_BLOCK, __FILE__, __LINE__)
#define myCalloc(c, s)    _calloc_dbg(c, s, _NORMAL_BLOCK, __FILE__, __LINE__)
#define myRealloc(p, s)   _realloc_dbg(p, s, _NORMAL_BLOCK, __FILE__, __LINE__)
#define myExpand(p, s)    _expand_dbg(p, s, _NORMAL_BLOCK, __FILE__, __LINE__)
#define myFree(p)         _free_dbg(p, _NORMAL_BLOCK)
#define myMemSize(p)      _msize_dbg(p, _NORMAL_BLOCK)
#define myNew new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define myDelete delete  // Set to dump leaks at the program exit.
#define myInitMemoryCheck() \
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF)
#define myDumpMemoryLeaks() \
	_CrtDumpMemoryLeaks()
#else
#define myMalloc malloc
#define myCalloc calloc
#define myRealloc realloc
#define myExpand _expand
#define myFree free
#define myMemSize _msize
#define myNew new
#define myDelete delete
#define myInitMemoryCheck()
#define myDumpMemoryLeaks()
#endif 
//////////////////////////////////////////////////////////////////////////



#endif
