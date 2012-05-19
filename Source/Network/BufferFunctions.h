#ifndef BUFFER_FUNCTIONS_H
#define BUFFER_FUNCTIONS_H
#include "..\stdafx.h"

static void AddToBuf(char* bufOut, int &offsetOut, char in)
{
	memmove(bufOut + offsetOut, &in, sizeof (in));
	offsetOut += sizeof (in);
}
static void AddToBuf(char* bufOut, int &offsetOut, string in)
{
	for(int i = 0; i < in.size(); i++)
	{
		AddToBuf(bufOut, offsetOut, in[i]);
	}
	AddToBuf(bufOut, offsetOut, ';'); //replace ';' with SEGMENT_ENd
}

static void AddToBuf(char* bufOut, int &offsetOut, float in)
{
	memmove(bufOut + offsetOut, &in, sizeof (in));
	offsetOut += sizeof (in);
}

static void AddToBuf(char* bufOut, int &offsetOut, int in)
{
	memmove(bufOut + offsetOut, &in, sizeof (in));
	offsetOut += sizeof (in);
}



static char GetFromBufC(char* buf, int &offsetOut)
{
	char out = '0';
	memmove(&out, &buf[offsetOut], sizeof (char));
	offsetOut += sizeof (char);
	return out;
}
static float GetFromBufferF(char* buf, int &offsetOut)
{
	float out = 0.0f;
	memmove(&out, &buf[offsetOut], sizeof (float));
	offsetOut += sizeof (float);
	return out;
}
static float GetFromBufferI(char* buf, int &offsetOut)
{
	int out = 0;
	memmove(&out, &buf[offsetOut], sizeof (int));
	offsetOut += sizeof (int);
	return out;
}
static string GetFromBufS(char* buf, int &offsetOut)
{
	string out = "";
	while(buf[offsetOut] != ';')
	{
		out += GetFromBufC(buf, offsetOut);
	}
	offsetOut += sizeof(char);
	return out;
}
#endif