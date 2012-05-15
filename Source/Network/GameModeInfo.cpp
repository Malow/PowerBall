#include "GameModeInfo.h"
#include "BufferFunctions.h"

GameModeInfo::GameModeInfo()
{
	this->mNumRounds = 0;
	this->mNumTeams = 0;
}
GameModeInfo::GameModeInfo(char* bufOut, int &offsetOut)
{
	this->mNumRounds = (int)GetFromBufC(bufOut, offsetOut);
	this->mNumTeams  = (int)GetFromBufC(bufOut, offsetOut);
}
GameModeInfo::GameModeInfo(int numRounds, int numTeams)
{
	this->mNumRounds = numRounds;
	this->mNumTeams  = numTeams;
}
GameModeInfo::~GameModeInfo()
{
}
	
void GameModeInfo::GetBuffer(char* bufOut, int &offsetOut)
{
	AddToBuf(bufOut, offsetOut, (char)this->mNumRounds);
	AddToBuf(bufOut, offsetOut, (char)this->mNumTeams);
}


CTFInfo::CTFInfo()
	: GameModeInfo()
{
}
CTFInfo::CTFInfo(char* bufOut, int &offsetOut)
	: GameModeInfo(bufOut, offsetOut)
{
}
CTFInfo::CTFInfo(int numRounds, int numTeams)
	: GameModeInfo(numRounds, numTeams)
{
}
CTFInfo::~CTFInfo()
{
}
void CTFInfo::GetBuffer(char* bufOut, int &offsetOut)
{
	GameModeInfo::GetBuffer(bufOut, offsetOut);
}



KOTHInfo::KOTHInfo()
	: GameModeInfo()
{
}
KOTHInfo::KOTHInfo(char* bufOut, int &offsetOut)
	: GameModeInfo(bufOut, offsetOut)
{
	this->mMaxTimeContinuously = GetFromBufferF(bufOut, offsetOut);
	this->mMaxTimeAccumulated  = GetFromBufferF(bufOut, offsetOut);
}
KOTHInfo::KOTHInfo(int numRounds, int numTeams, float maxTimeContinuously, float maxTimeAccumulated)
	: GameModeInfo(numRounds, numTeams)
{
	this->mMaxTimeContinuously = maxTimeContinuously;
	this->mMaxTimeAccumulated  = maxTimeAccumulated;
}
KOTHInfo::~KOTHInfo()
{
}

void KOTHInfo::GetBuffer(char* bufOut, int &offsetOut)
{
	GameModeInfo::GetBuffer(bufOut, offsetOut);
	AddToBuf(bufOut, offsetOut, this->mMaxTimeContinuously);
	AddToBuf(bufOut, offsetOut, this->mMaxTimeAccumulated);
}




WARLOCKInfo::WARLOCKInfo()
	: GameModeInfo()
{
}
WARLOCKInfo::WARLOCKInfo(char* bufOut, int &offsetOut)
	: GameModeInfo(bufOut, offsetOut)
{
}
WARLOCKInfo::WARLOCKInfo(int numRounds, int numTeams)
	: GameModeInfo(numRounds, numTeams)
{
}
WARLOCKInfo::~WARLOCKInfo()
{
}

void WARLOCKInfo::GetBuffer(char* bufOut, int &offsetOut)
{
	GameModeInfo::GetBuffer(bufOut, offsetOut);
}