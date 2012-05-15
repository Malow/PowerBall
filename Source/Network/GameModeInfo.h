#pragma once

#include "..\stdafx.h"
class GameModeInfo
{
protected:
	int mNumRounds;
	int mNumTeams; //0 = free for all
public:
	//constructors and destructors
				GameModeInfo();
				GameModeInfo(char* bufOut, int &offsetOut);
				GameModeInfo(int numRounds, int numTeams);
	virtual		~GameModeInfo();
	
	virtual void GetBuffer(char* bufOut, int &offsetOut);

	int			GetNumRounds() const { return this->mNumRounds; }
	int			GetNumTeams() const { return this->mNumTeams; }
	void		SetNumRounds(int numRounds) { this->mNumRounds = numRounds; }
	void		SetNumTeams(int numTeams) { this->mNumTeams = numTeams; }
};

class CTFInfo : public GameModeInfo
{
private:

public:
	//constructors and destructors
				CTFInfo();
				CTFInfo(char* bufOut, int &offsetOut);
				CTFInfo(int numRounds, int numTeams);
				~CTFInfo();
	void		GetBuffer(char* bufOut, int &offsetOut);
};

class KOTHInfo : public GameModeInfo
{
private:
	float	mMaxTimeContinuously;
	float	mMaxTimeAccumulated;
public:
	//constructors and destructors
				KOTHInfo();
				KOTHInfo(char* bufOut, int &offsetOut);
				KOTHInfo(int numRounds, int numTeams, float maxTimeContinuously, float maxTimeAccumulated);
				~KOTHInfo();

	void		GetBuffer(char* bufOut, int &offsetOut);

	float		GetMaxTimeContinuously() const { return this->mMaxTimeContinuously; }
	float		GetMaxTimeAccumulated() const { return this->mMaxTimeAccumulated; }
	void		SetMaxTimeContinuously(float val) { this->mMaxTimeContinuously = val; }
	void		SetMaxTimeAccumulated(float val) { this->mMaxTimeAccumulated = val; }
};

class WARLOCKInfo : public GameModeInfo
{
private:
public:
	//constructors and destructors
				WARLOCKInfo();
				WARLOCKInfo(char* bufOut, int &offsetOut);
				WARLOCKInfo(int numRounds, int numTeams);
				~WARLOCKInfo();

	void		GetBuffer(char* bufOut, int &offsetOut);
};