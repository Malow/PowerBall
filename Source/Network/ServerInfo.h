#pragma once

#include "..\stdafx.h"
class ServerInfo
{
private:
	string			mServerName;
	int				mNumPlayers;
	int				mMaxNumPlayers;
	int				mGameMode;
	string			mIP;
public:
	//constructors and destructors
				ServerInfo();
				ServerInfo(char* bufOut, int &offsetOut);
				ServerInfo(string serverName, int numPlayers, int maxNumPlayers, int mGameMode, string ip);
	virtual		~ServerInfo();

	int			GetNumPlayers() {return this->mNumPlayers;}
	int			GetMaxNumPlayers() {return this->mMaxNumPlayers;}
	int			GetGameMode() {return this->mGameMode;}
	string		GetIP() {return this->mIP;}
	string		GetServerName() {return this->mServerName;}

	void		GetBuffer(char* bufOut, int &offsetOut);

	
	void		SetNumPlayers(int n) {this->mNumPlayers = n;}
	void		SetMaxNumPlayers(int n) {this->mMaxNumPlayers = n;}
	void		SetGameMode(int gm) {this->mGameMode = gm;}
	void		SetIP(string ip) {this->mIP = ip;}
	void		SetServerName(string name) {this->mServerName = name;}
};