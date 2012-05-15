#pragma once

#include "..\stdafx.h"
#include "GameModeInfo.h"
class ServerInfo
{
private:
	string			mServerName;
	int				mNumPlayers;
	int				mMaxNumPlayers;
	int				mGameMode;
	string			mIP;
	GameModeInfo*	mGMI;
public:
	//constructors and destructors
				ServerInfo();
				ServerInfo(char* bufOut, int &offsetOut);
				ServerInfo(string serverName, int numPlayers, int maxNumPlayers, int mGameMode, string ip, GameModeInfo* gmi);
	virtual		~ServerInfo();

	ServerInfo(const ServerInfo &source);
	ServerInfo& ServerInfo::operator=(const ServerInfo &source);

	int			GetNumPlayers() {return this->mNumPlayers;}
	int			GetMaxNumPlayers() {return this->mMaxNumPlayers;}
	int			GetGameMode() {return this->mGameMode;}
	string		GetIP() {return this->mIP;}
	string		GetServerName() {return this->mServerName;}
	GameModeInfo* GetGameModeInfo() { return this->mGMI; }
	void		GetBuffer(char* bufOut, int &offsetOut);

	
	void		SetNumPlayers(int n) {this->mNumPlayers = n;}
	void		SetMaxNumPlayers(int n) {this->mMaxNumPlayers = n;}
	void		SetGameMode(int gm) {this->mGameMode = gm;}
	void		SetIP(string ip) {this->mIP = ip;}
	void		SetServerName(string name) {this->mServerName = name;}
	//void		SetGameModeInfo(GameModeInfo* gmi) { this->mGMI = &gmi; }
};