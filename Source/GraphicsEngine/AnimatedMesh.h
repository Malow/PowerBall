/*
	Class for doing (linear) morph animation with keyframes.
	todo**: Requirements: (number of verts must be the same, 
	same with faces and attributes, material must be equal,
	textures must be the same, shaders, etc must be the same**
*/

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
	KeyFrame(const KeyFrame* origObj)
	{
		this->time = origObj->time;
		for(int i = 0; i < origObj->strips->size(); i++)
		{
			this->strips->add(new MeshStrip(origObj->strips->get(i)));
		}
	}
};


class AnimatedMesh : public Mesh
{
	private:
		UINT						mNrOfTimesLooped;
		bool						mLoopNormal;
		bool						mLoopSeamless;
		//loopa växelström**
		float						mTimer;
		MaloW::Array<KeyFrame*>*	mKeyFrames;
		
		//Loop/noloop
		//loop Seamless (använda keyframe första som sista keyframe)
		
		//spela fram & tillbaka**

	public:
		AnimatedMesh(D3DXVECTOR3 pos);
		virtual ~AnimatedMesh();

		UINT GetNrOfTimesLooped() const;
		bool IsLooping() const;
		/*! Returns the 2 keyframes to interpolate with value t[0,1] through the parameters depending on the current time. Note that currentTime is expected to be in milliseconds. */
		void GetCurrentKeyFrames(KeyFrame** one, KeyFrame** two, float& t, float currentTime);

		/*! Prevents looping. Default. */
		void NoLooping();
		/*! Loops by returning to the first keyframe when last keyframe is reached. Note that this kind of looping is not seamless. */
		void LoopNormal();
		/*! Loops by adding the first keyframe as the last keyframe to prevent seamed(normal) looping */
		void LoopSeamless(); 

		virtual void LoadFromFile(string file);

		/* ! Doesnt work atm, let us know if u need it work. Allways returns NULL atm. */
		virtual MaloW::Array<MeshStrip*>* GetStrips();

		MaloW::Array<KeyFrame*>* GetKeyFrames() { return this->mKeyFrames; }
};