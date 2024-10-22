#pragma once

#include "..\stdafx.h"
//class Lobby;
class GameNetwork;
class NetworkHandler;
#define SEGMENT_END ';'
class MsgHandler
{
private:
	//Lobby*				mLobby;
	GameNetwork*		mNet;
	NetworkHandler*	mConn;
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
	void		Set(GameNetwork* gn, NetworkHandler* conn);//, Lobby* lobby = NULL);
	void		ProcessMSG(char* buffer, int size, int index);

	
	void		Ping(int index);
	void		SendClose();
	void		SendClose(int index);
	void		SendClientData();
	void		SendServerData();
	void		SendCastSpell(int casterIndex, int spellIndex);
	void		JoinTeam(TEAM id);

	void		PlayerReconnect(int index);
	//lobby
	void		SendIdentifyYourself();
	void		SendPlayerInfos();
	void		SendPlayerProfile();
	void		SendDropPlayer(int index);


private:
	
	void		Close();
	void		ReturnPing(int index);
	void		ReceivePing(int index);

	void		ReceiveClientData(char* buf, int &offset, int index);
	void		ReceiveServerData(char* buf, int &offset);

	void		ReceiveTeamChange(char* buf, int &offset, int index);
	void		ReceiveCastSpell(char* buf, int &offset, int index);
	void		ReceiveDropPlayer(char* buf, int &offset);

	void		ReceiveIdentification(char* buf, int &offset, int index);
	void		ReceivePlayerInfos(char* buf, int &offset);
};