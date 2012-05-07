#pragma once
#include "stdafx.h"
class PlayerHistory
{
private:
	struct Snapshot
	{
		D3DXVECTOR3 pos;
		float		time;
	};
	static const int CAP = 200;
	Snapshot		mHistory[CAP];
	int				mHistoryIndex;
	D3DXVECTOR3		mInterpolationVector;
public:
				PlayerHistory();
	virtual		~PlayerHistory();

	void		AddSnapshot(D3DXVECTOR3 pos, float time);
	void		Reset(D3DXVECTOR3 pos);
	D3DXVECTOR3	GetPos(float time);
	void		MoveHistory(D3DXVECTOR3 offset);
};