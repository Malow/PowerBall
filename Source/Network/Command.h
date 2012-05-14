#pragma once
#include "..\stdafx.h"
#define INPUT_CAP 5
class Command
{
private:
	char		mInputs[INPUT_CAP];
	int			mNumInputs;
	float		mDuration;
	D3DXVECTOR3 mForward;
	Command*	mNext;
public:
				Command();
				Command(char inputs[], int numInputs, float duration, D3DXVECTOR3 forward);
	virtual		~Command();

	char*		GetInputs() { return this->mInputs; }
	char		GetInput(int index) { return this->mInputs[index]; }
	int			GetNumInputs() { return this->mNumInputs; }
	float		GetDuration() { return this->mDuration; }
	D3DXVECTOR3	GetForward() { return this->mForward; }
	Command*	GetNext() { return this->mNext; }
	
	void		SetInputs(char inputs[], int numInputs);
	void		SetDuration(float duration) { this->mDuration = duration; }
	void		ModifyDuration(float mod) { this->mDuration += mod; }
	void		SetForward(D3DXVECTOR3 forward) { this->mForward = forward; }
	void		SetNext(Command* command) { this->mNext = command; }
};