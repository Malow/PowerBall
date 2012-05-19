#pragma once

#include "..\stdafx.h"
#include "Command.h"

class CommandHandler
{
private:
	Command*	mEnd;
	Command*	mStart;
	int			mNumCommands;
public:
			CommandHandler();
	virtual ~CommandHandler();

	void		Pop();
	void		Push(char inputs[], int numInputs, float duration, D3DXVECTOR3 forward);
	Command*	Front() const;
	Command*	Back() const;
	void		Clear();
	int			GetNumCommands() const { return this->mNumCommands; }
	bool		Empty() { return (this->mNumCommands == 0); }
};