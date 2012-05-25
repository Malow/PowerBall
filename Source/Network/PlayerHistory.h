#pragma once
#include "..\stdafx.h"

struct Snapshot
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 tempPos;
	float		time;
};
class PlayerHistory
{
private:
	static const int CAP = 300;
	Snapshot		mHistory[CAP];
	int				mHistoryIndex;
	D3DXVECTOR3		mInterpolationVector;
public:
				PlayerHistory();
	virtual		~PlayerHistory();

	void		AddSnapshot(D3DXVECTOR3 pos, D3DXVECTOR3 tempPos, float time);
	void		Reset(D3DXVECTOR3 pos);
	D3DXVECTOR3	GetPos(float time);
	Snapshot	GetSnapshot(float time);
	D3DXVECTOR3	GetTempPos(float time);
	void		MoveHistory(D3DXVECTOR3 offset);
	

};