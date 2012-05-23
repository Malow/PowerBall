#pragma once
/**
* Author: Jerry Rahmqvist
* Create Date: 19/05/2012
* 
* 
**/

class GameTimerSimple
{
	private:
		__int64 start;
		float mTimeStepDt;
		float mCurrentTime;
		float mNewTime;
		float mDeltaTime;
		
		
		

	public:
		float mT;
		float mAccumulator;
		GameTimerSimple();
		GameTimerSimple(float timeStep);
		~GameTimerSimple();
		void Initialize();
		float GetDeltaTime() const;
		float GetMyCurrentTime() const;
		float GetNewTime() const;
		float GetTimeStepDt() const;
		void ResetTimers();
		bool Update();
		float GetTime();
};
