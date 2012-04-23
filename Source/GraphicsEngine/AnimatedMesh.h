#pragma once

#include "stdafx.h"
#include "Mesh.h"


class KeyFrame
{
public:
	int time;
	MaloW::Array<MeshStrip*>* strips;

	KeyFrame() { this->time = 0; this->strips = new MaloW::Array<MeshStrip*>(); }
	virtual ~KeyFrame()
	{
		if(this->strips)
		{
			while(this->strips->size() > 0)
				delete this->strips->getAndRemove(0);

			delete this->strips;
		}

	}
};


class AnimatedMesh : public Mesh
{
private:
	MaloW::Array<KeyFrame*>* keyframes;
	float timer;

public:
	AnimatedMesh(D3DXVECTOR3 pos);
	virtual ~AnimatedMesh();

	void GetCurrentKeyFrames(KeyFrame* one, KeyFrame* two, float* t, float time);

	virtual void LoadFromFile(string file);

	/* ! Doesnt work atm, let us know if u need it work. Allways returns NULL atm. */
	virtual MaloW::Array<MeshStrip*>* GetStrips();

	MaloW::Array<KeyFrame*>* GetKeyFrames() { return this->keyframes; }
};