#pragma once
/**
* Auther: Rikard Johansson
* Create Date: 28/03/2012
*
* This is a abstract class that will have information
* about the diffrent events that can occure
* when you are clicking on a button on the GUI.
* Event Message is the message about whats going to happen.
* Event Info is the extra info that may be used.
**/
#include <string>

using namespace std;

class Event
{
private:
	string mEventMessage;
	string mEventInfo;

public:
	/*! Default constructor*/
	Event();
	/*! Constructor that takes a message and info about the event*/
	Event(string eventMessage, string eventInfo);
	virtual ~Event();
	
	/*! Set the event message.*/
	void SetEventMessage(string eventMessage) { this->mEventMessage = eventMessage; }
	/*! Set the event info.*/
	void SetEventInfo(string eventInfo) { this->mEventInfo = eventInfo; }
	/*! Returns the event message*/
	string GetEventMessage(){ return this->mEventMessage; }
	/*! Returns the event info*/
	string GetEventInfo(){ return this->mEventInfo; }
	
};