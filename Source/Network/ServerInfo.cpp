#include "ServerInfo.h"
#include "BufferFunctions.h"

ServerInfo::ServerInfo()
{
	this->mServerName		= "";
	this->mNumPlayers		= 0;
	this->mMaxNumPlayers	= 0;
	this->mGameMode			= int(0);
	this->mIP				= "";
	this->mPort				= 0;
	this->mGMI				= NULL;
	this->mID				= -1;
}
ServerInfo::ServerInfo(char* bufOut, int &offsetOut)
{
	this->mServerName		= GetFromBufS(bufOut, offsetOut);
	this->mNumPlayers		= (int)GetFromBufC(bufOut, offsetOut);
	this->mMaxNumPlayers	= (int)GetFromBufC(bufOut, offsetOut);
	this->mGameMode			= (int)GetFromBufC(bufOut, offsetOut);
	this->mIP				= GetFromBufS(bufOut, offsetOut);
	this->mPort				= GetFromBufferI(bufOut, offsetOut);
	if(this->mGameMode == GAMEMODE::CTF)
		this->mGMI = new CTFInfo(bufOut, offsetOut);
	else if(this->mGameMode == GAMEMODE::KOTH)
		this->mGMI = new KOTHInfo(bufOut, offsetOut);
	else if(this->mGameMode == GAMEMODE::WARLOCK)
		this->mGMI = new WARLOCKInfo(bufOut, offsetOut);
	this->mID				= -1;
}
ServerInfo::ServerInfo(string serverName, int numPlayers, int maxNumPlayers, int gameMode, string ip, int port, GameModeInfo* gmi)
{
	this->mServerName		= serverName;
	this->mNumPlayers		= numPlayers;
	this->mMaxNumPlayers	= maxNumPlayers;
	this->mGameMode			= gameMode;
	this->mIP				= ip;
	this->mPort				= port;
	this->mGMI				= gmi;
	this->mID				= -1;
}
ServerInfo::ServerInfo(const ServerInfo &source)
{
	this->mServerName		= source.mServerName;
	this->mNumPlayers		= source.mNumPlayers;
	this->mMaxNumPlayers	= source.mMaxNumPlayers;
	this->mGameMode			= source.mGameMode;
	this->mIP				= source.mIP;
	this->mPort				= source.mPort;
	this->mID				= source.mID;

	if(this->mGameMode == GAMEMODE::CTF)
		this->mGMI = new CTFInfo(*(CTFInfo*)source.mGMI);
	else if(this->mGameMode == GAMEMODE::KOTH)
		this->mGMI = new KOTHInfo(*(KOTHInfo*)source.mGMI);
	else if(this->mGameMode == GAMEMODE::WARLOCK)
		this->mGMI = new WARLOCKInfo(*(WARLOCKInfo*)source.mGMI);
	else this->mGMI = new GameModeInfo();
}
ServerInfo& ServerInfo::operator=(const ServerInfo &source)
{
	this->mServerName		= source.mServerName;
	this->mNumPlayers		= source.mNumPlayers;
	this->mMaxNumPlayers	= source.mMaxNumPlayers;
	this->mGameMode			= source.mGameMode;
	this->mIP				= source.mIP;
	this->mPort				= source.mPort;
	this->mID				= source.mID;

	if(this->mGMI != NULL)
		delete this->mGMI;
	
	if(this->mGameMode == GAMEMODE::CTF)
		this->mGMI = new CTFInfo(*(CTFInfo*)source.mGMI);
	else if(this->mGameMode == GAMEMODE::KOTH)
		this->mGMI = new KOTHInfo(*(KOTHInfo*)source.mGMI);
	else if(this->mGameMode == GAMEMODE::WARLOCK)
		this->mGMI = new WARLOCKInfo(*(WARLOCKInfo*)source.mGMI);
	else this->mGMI = new GameModeInfo();
	return *this;
}
ServerInfo::~ServerInfo()
{
	if(this->mGMI != NULL)
		delete this->mGMI;
}
void ServerInfo::GetBuffer(char* bufOut, int &offsetOut)
{
	AddToBuf(bufOut, offsetOut, this->mServerName);
	AddToBuf(bufOut, offsetOut, (char)this->mNumPlayers);
	AddToBuf(bufOut, offsetOut, (char)this->mMaxNumPlayers);
	AddToBuf(bufOut, offsetOut, (char)this->mGameMode);
	AddToBuf(bufOut, offsetOut, this->mIP);
	AddToBuf(bufOut, offsetOut, this->mPort);

	
	if(this->mGameMode == GAMEMODE::CTF)
		((CTFInfo*)this->mGMI)->GetBuffer(bufOut, offsetOut);
	else if(this->mGameMode == GAMEMODE::KOTH)
		((KOTHInfo*)this->mGMI)->GetBuffer(bufOut, offsetOut);
	else if(this->mGameMode == GAMEMODE::WARLOCK)
		((WARLOCKInfo*)this->mGMI)->GetBuffer(bufOut, offsetOut);
}