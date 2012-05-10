#include "GUIEvent.h"

class ChangeOptionEvent : public GUIEvent
{
private:
	string mOption;
	string mValue;
public:
	ChangeOptionEvent();
	ChangeOptionEvent(string mOption, string value);
	virtual ~ChangeOptionEvent();

	string GetOption(){ return this->mOption; }
	void SetOption(string option){ this->mOption = option; }

	string GetValue(){ return this->mValue; }
	void SetValue(string value){ this->mValue = value; }
};