#pragma once
/**
* Auther: Rikard Johansson
* Create Date: 28/03/2012
*
* This is a abstract class that will have information
* about the diffrent events that can occure
* when you are clicking on a button on the GUI.
* Event Message is the message about whats going to happen.
**/
#include <string>

using namespace std;

class Event
{
private:
	string mEventMessage;

public:
	/*! Default constructor*/
	Event();
	/*! Constructor that takes a message about the event*/
	Event(string eventMessage);
	virtual ~Event();
	
	/*! Set the event message.*/
	void SetEventMessage(string eventMessage) { this->mEventMessage = eventMessage; }
	/*! Returns the event message*/
	string GetEventMessage(){ return this->mEventMessage; }
	
};