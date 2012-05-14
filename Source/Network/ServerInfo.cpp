#include "ServerInfo.h"

void AddToBuf(char* bufOut, int &offsetOut, char in)
{
	memmove(bufOut + offsetOut, &in, sizeof (in));
	offsetOut += sizeof (in);
}
void AddToBuf(char* bufOut, int &offsetOut, string in)
{
	for(int i = 0; i < in.size(); i++)
	{
		AddToBuf(bufOut, offsetOut, in[i]);
	}
	AddToBuf(bufOut, offsetOut, ';');
}
char GetFromBufC(char* buf, int &offsetOut)
{
	char out = '0';
	memmove(&out, &buf[offsetOut], sizeof (char));
	offsetOut += sizeof (char);
	return out;
}
string GetFromBufS(char* buf, int &offsetOut)
{
	string out = "";
	while(buf[offsetOut] != ';')
	{
		out += GetFromBufC(buf, offsetOut);
	}
	offsetOut += sizeof(char);
	return out;
}

ServerInfo::ServerInfo()
{
	this->mServerName		= "";
	this->mNumPlayers		= 0;
	this->mMaxNumPlayers	= 0;
	this->mGameMode			= int(0);
	this->mIP				= "";
}
ServerInfo::ServerInfo(char* bufOut, int &offsetOut)
{
	this->mServerName		= GetFromBufS(bufOut, offsetOut);
	this->mNumPlayers		= (int)GetFromBufC(bufOut, offsetOut);
	this->mMaxNumPlayers	= (int)GetFromBufC(bufOut, offsetOut);
	this->mGameMode			= (int)GetFromBufC(bufOut, offsetOut);
	this->mIP				= GetFromBufS(bufOut, offsetOut);
}
ServerInfo::ServerInfo(string serverName, int numPlayers, int maxNumPlayers, int gameMode, string ip)
{
	this->mServerName		= serverName;
	this->mNumPlayers		= numPlayers;
	this->mMaxNumPlayers	= maxNumPlayers;
	this->mGameMode			= gameMode;
	this->mIP				= ip;
}
ServerInfo::~ServerInfo()
{
}
void ServerInfo::GetBuffer(char* bufOut, int &offsetOut)
{
	AddToBuf(bufOut, offsetOut, this->mServerName);
	AddToBuf(bufOut, offsetOut, (char)this->mNumPlayers);
	AddToBuf(bufOut, offsetOut, (char)this->mMaxNumPlayers);
	AddToBuf(bufOut, offsetOut, (char)this->mGameMode);
	AddToBuf(bufOut, offsetOut, this->mIP);
}