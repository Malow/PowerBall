#include "PlayerHistory.h"

PlayerHistory::PlayerHistory()
{
	for(int i = 0; i < this->CAP; i++)
	{
		this->mHistory[i].pos	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		this->mHistory[i].time	= 0.0f;
	}
	this->mHistoryIndex = 0;
	this->mInterpolationVector = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}
PlayerHistory::~PlayerHistory()
{

}

void PlayerHistory::AddSnapshot(D3DXVECTOR3 pos, float time)
{
	this->mHistory[this->mHistoryIndex].pos = pos;
	this->mHistory[this->mHistoryIndex++].time = time;
	this->mHistoryIndex = this->mHistoryIndex%CAP; //making the array circular
}
void PlayerHistory::Reset(D3DXVECTOR3 pos)
{
	for(int i = 0; i < this->CAP; i++)
	{
		this->mHistory[i].pos = pos;
	}
}
D3DXVECTOR3	PlayerHistory::GetPos(float time)
{
	int i = this->mHistoryIndex;
	if(i == 0)
		i = CAP;
	while(this->mHistory[--i].time > time && i != this->mHistoryIndex)
	{
		if(i == 0)
			i = CAP;
	}

	//linear interpolation between the two closest times.
	D3DXVECTOR3 lerpHistoryPos;
	float lerpVal = (float)(time - this->mHistory[i].time) / (float)(this->mHistory[i+1].time - this->mHistory[i].time);
	D3DXVec3Lerp(&lerpHistoryPos, &this->mHistory[i].pos, &this->mHistory[i + 1].pos, lerpVal);

	return lerpHistoryPos;
}
void PlayerHistory::MoveHistory(D3DXVECTOR3 offset)
{
	for(int i = 0; i < this->CAP; i++)
	{
		this->mHistory[i].pos += offset;
	}
}