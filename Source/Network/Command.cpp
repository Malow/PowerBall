#include "Command.h"

Command::Command()
{
	this->mDuration		= 0.0f;
	this->mForward		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	this->mNumInputs	= 0;
	for(int i = 0; i < INPUT_CAP; i++)
		this->mInputs[i] = 0;
	this->mNext			= NULL;
}
Command::Command(char inputs[], int numInputs, float duration, D3DXVECTOR3 forward)
{
	this->mDuration		= duration;
	this->mForward		= forward;
	this->mNumInputs	= numInputs;
	for(int i = 0; i < numInputs; i++)
		this->mInputs[i] = inputs[i];
	this->mNext			= NULL;
}
Command::~Command()
{
}
void Command::SetInputs(char inputs[], int numInputs)
{
	this->mNumInputs = numInputs;
	for(int i = 0; i < numInputs; i++)
		this->mInputs[i] = inputs[i];
}