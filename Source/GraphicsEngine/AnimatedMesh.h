/*!
	Class for doing morphanimation. Morphing is done for positions, 
	texture coordinates, normals and colors of the two keyframes.
	Requirements: 
		Atleast two keyframes to morph/interpolate.
		The number of vertices must match between all keyframes.
		Same goes with the number of faces, attributes, materials, textures and shaders.
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
			this->strips = new MaloW::Array<MeshStrip*>();
			for(int i = 0; i < origObj->strips->size(); i++)
			{
				this->strips->add(new MeshStrip(origObj->strips->get(i)));
			}
		}
};


class AnimatedMesh : public Mesh
{
	private:
		unsigned int				mNrOfTimesLooped;
		bool						mLoopNormal;
		bool						mLoopSeamless;
		float						mCurrentTime;
		MaloW::Array<KeyFrame*>*	mKeyFrames;

	public:
		AnimatedMesh(D3DXVECTOR3 pos);
		virtual ~AnimatedMesh();

		unsigned int				GetNrOfTimesLooped()	const { return this->mNrOfTimesLooped; }
		bool						IsLooping()				const { return this->mLoopNormal || this->mLoopSeamless; }
		bool						IsLoopingNormal()		const { return this->mLoopNormal; }
		bool						IsLoopingSeamless()		const { return this->mLoopSeamless; }
		MaloW::Array<KeyFrame*>*	GetKeyFrames()			const { return this->mKeyFrames; }

		/*! Returns the 2 keyframes to interpolate with value t[0,1] through the parameters depending on the current time. */
		/*!	Note that currentTime is expected to be in milliseconds. Returns NULL if there's no keyframes loaded. */
		/*! Also note that this function can most likely be optimized. */
		void GetCurrentKeyFrames(KeyFrame** one, KeyFrame** two, float& t);

		/* ! Returns the strips of the second mesh currently being used. */
		virtual MaloW::Array<MeshStrip*>* GetStrips();

		/*! Set the current time. This must be done every frame to for the animation to be animated. */
		void SetCurrentTime(float currentTime);

		/*! Prevents looping. Default. */
		void NoLooping();
		/*! Loops by returning to the first keyframe when last keyframe is reached. Note that this kind of looping is not seamless. */
		void LoopNormal();
		/*! Loops by adding the first keyframe as the last keyframe to prevent seamed(normal) looping */
		void LoopSeamless(); 

		/*! Load the keyframes from file. Input is exptected to be "'filename'.ani". */
		virtual void LoadFromFile(string file);

};