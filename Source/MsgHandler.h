#pragma once

#include "stdafx.h"
class GameNetwork;
class ServerConnection;
#define SEGMENT_END ';'
class MsgHandler
{
private:
	GameNetwork*		mNet;
	ServerConnection*	mConn;
	LARGE_INTEGER		mOldTick;

	MsgHandler() {};                               // Private constructor
	MsgHandler(const MsgHandler&);                 // Prevent copy-construction
	MsgHandler& operator=(const MsgHandler&);      // Prevent assignment

	/*! Adds a float to the char buffer and counts up the offset. */
	void		AddToBuffer(char* bufOut, int &offsetOut, float in);

	/*! Adds a char to the char buffer and counts up the offset. */
	void		AddToBuffer(char* bufOut, int &offsetOut, char in);

	/*! Adds a d3dxvector3 to the char buffer and counts up the offset. */
	void		AddToBuffer(char* bufOut, int &offsetOut, D3DXVECTOR3 in);
	
	/*! Retrieves a float from the char buffer and counts up the offset. */
	float		GetFromBufferF(char* buf, int &offsetOut);
	
	/*! Retrieves a char from the char buffer and counts up the offset. */
	char		GetFromBufferC(char* buf, int &offsetOut);
	
	/*! Retrieves a d3dxvector3 from the char buffer and counts up the offset. */
	D3DXVECTOR3	GetFromBufferD(char* buf, int &offsetOut);
public:
	//constructors and destructors
	virtual		~MsgHandler();
	static MsgHandler& GetInstance()
	{
		static MsgHandler singleton;
		return singleton;
	}
	void		Set(GameNetwork* gn, ServerConnection*	conn);
	void		ProcessMSG(char* buffer, int size, int index);

	
	void		Ping(int index);
	void		SendClose();
	void		SendClientData();
	void		SendServerData();



private:
	
	void		Close();
	void		ReturnPing(int index);
	void		ReceivePing(int index);

	void		ReceiveClientData(char* buf, int &offset, int index);
	void		ReceiveServerData(char* buf, int &offset);
};