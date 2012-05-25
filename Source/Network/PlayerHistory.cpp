#include "PlayerHistory.h"

PlayerHistory::PlayerHistory()
{
	for(int i = 0; i < this->CAP; i++)
	{
		this->mHistory[i].pos	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		this->mHistory[i].tempPos	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		this->mHistory[i].time	= 0.0f;
	}
	this->mHistoryIndex = 0;
	this->mInterpolationVector = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}
PlayerHistory::~PlayerHistory()
{

}

void PlayerHistory::AddSnapshot(D3DXVECTOR3 pos, D3DXVECTOR3 tempPos, float time)
{
	this->mHistory[this->mHistoryIndex].pos = pos;
	this->mHistory[this->mHistoryIndex].tempPos = tempPos;
	this->mHistory[this->mHistoryIndex++].time = time;
	this->mHistoryIndex = this->mHistoryIndex%CAP; //making the array circular
}
void PlayerHistory::Reset(D3DXVECTOR3 pos)
{
	for(int i = 0; i < this->CAP; i++)
	{
		this->mHistory[i].pos = pos;
		this->mHistory[i].tempPos = pos;
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
Snapshot PlayerHistory::GetSnapshot(float time)
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
	
	D3DXVECTOR3 lerpHistoryTempPos;
	D3DXVec3Lerp(&lerpHistoryTempPos, &this->mHistory[i].tempPos, &this->mHistory[i + 1].tempPos, lerpVal);

	Snapshot ret;
	ret.pos = lerpHistoryPos;
	ret.tempPos = lerpHistoryTempPos;

	return ret;
}
void PlayerHistory::MoveHistory(D3DXVECTOR3 offset)
{
	for(int i = 0; i < this->CAP; i++)
	{
		this->mHistory[i].pos += offset;
	}
}