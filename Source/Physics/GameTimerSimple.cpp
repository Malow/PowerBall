#include "GameTimerSimple.h"

/* for high def timers. */
#include <windows.h>
#include <mmsystem.h>

//static __int64 start = 0;

static __int64 frequency = 0;
GameTimerSimple::GameTimerSimple()
{
	this->mT = 0.0f;
	this->mAccumulator = 0.0f;
	this->mCurrentTime = 0.0f;
	this->mTimeStepDt = 10.0f; //10 for knockout mode
	
	this->mNewTime = 0.0f;
	this->mDeltaTime = 0.0f;
	this->start = 0;

}

GameTimerSimple::GameTimerSimple(float timeStep)
{
	this->mT = 0.0f;
	this->mAccumulator = 0.0f;
	this->mCurrentTime = 0.0f;
	this->mTimeStepDt = timeStep;
	
	this->mNewTime = 0.0f;
	this->mDeltaTime = 0.0f;
	this->start = 0;

}
GameTimerSimple::~GameTimerSimple()
{
	
}

void GameTimerSimple::Initialize()
{
	if (start==0)
    {
        QueryPerformanceCounter((LARGE_INTEGER*)&start);
        QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
    }
}

float GameTimerSimple::GetTime() 
{
    __int64 counter = 0;
    QueryPerformanceCounter((LARGE_INTEGER*)&counter);
    return (float) ((counter - start) / double(frequency))*1000.0f;
}


float GameTimerSimple::GetMyCurrentTime() const
{
	return this->mCurrentTime;
}

float GameTimerSimple::GetNewTime() const
{
	return this->mNewTime;
}

float GameTimerSimple::GetDeltaTime() const
{
	return this->mDeltaTime;
}

float GameTimerSimple::GetTimeStepDt() const
{
	return this->mTimeStepDt;
}

bool GameTimerSimple::Update()
{
	this->mNewTime = this->GetTime();
	this->mDeltaTime = this->mNewTime - this->mCurrentTime;
	this->mCurrentTime = this->mNewTime;
	if(this->mDeltaTime <= 0.0f)
		return false;
	else
		return true;
}

void GameTimerSimple::ResetTimers()
{
	this->mT = 0.0f;
	this->mAccumulator = 0.0f;
	this->mCurrentTime = 0.0f;
	this->mTimeStepDt = 5.0f;
	
	this->mNewTime = 0.0f;
	this->mDeltaTime = 0.0f;
	start = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&start);
}
