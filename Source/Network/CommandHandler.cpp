#include "CommandHandler.h"

CommandHandler::CommandHandler()
{
	this->mEnd			= NULL;
	this->mStart		= NULL;
	this->mNumCommands	= 0;
}
CommandHandler::~CommandHandler()
{
	while(this->mStart != NULL)
	{
		Command* temp = this->mStart->GetNext();
		delete this->mStart;
		this->mStart = temp;
	}
}
void CommandHandler::Pop()
{
	if(this->mNumCommands > 0)
	{
		Command* temp = this->mStart->GetNext();
		delete this->mStart;
		this->mStart = temp;
		temp = NULL;
		this->mNumCommands--;
	}
}
void CommandHandler::Push(char inputs[], int numInputs, float duration, D3DXVECTOR3 forward)
{
	if(this->mNumCommands == 0)
	{
		this->mStart	= new Command(inputs, numInputs, duration, forward);
		this->mEnd		= this->mStart;
	}
	else
	{
		Command* temp = new Command(inputs, numInputs, duration, forward);
		this->mEnd->SetNext(temp);
		this->mEnd = temp;
		temp = NULL;
	}
	this->mNumCommands++;
}
Command* CommandHandler::Front()
{
	Command* temp = NULL;
	if(this->mNumCommands > 0)
		temp = this->mStart;
	return temp;
}
Command* CommandHandler::Back()
{
	Command* temp = NULL;
	if(this->mNumCommands > 0)
		temp = this->mEnd;
	return temp;
}
void CommandHandler::Clear()
{
	while(this->mStart != NULL)
	{
		Command* temp = this->mStart->GetNext();
		delete this->mStart;
		this->mStart  = temp;
	}
	this->mStart	= NULL;
	this->mEnd		= NULL;
}