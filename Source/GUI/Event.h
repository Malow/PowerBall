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


class Event
{
private:
	char mEventMessage;
	char mEventInfo;

public:
	/*! Default constructor*/
	Event();
	/*! Constructor that takes a message and info about the event*/
	Event(char eventMessage, char eventInfo);
	virtual ~Event();
	
	/*! Set the event message.*/
	void SetEventMessage(char eventMessage) { this->mEventMessage = eventMessage; }
	/*! Set the event info.*/
	void SetEventInfo(char eventInfo) { this->mEventInfo = mEventInfo; }
	/*! Returns the event message*/
	char GetEventMessage(){ return this->mEventMessage; }
	/*! Returns the event info*/
	char GetEventInfo(){ return this->mEventInfo; }
	
};