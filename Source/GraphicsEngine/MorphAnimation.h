#pragma once

/*
	Class for doing (linear) morph animation with keyframes.
	todo**: Requirements: (number of verts must be the same, 
	same with faces and attributes, material must be equal,
	textures must be the same, shaders, etc must be the same**
*/

#include "..\stdafx.h" //**
#include "MaloWLib\Mesh.h"
#include "..\GraphicsEngine\MaloWLib\Shader.h"

class MorphAnimation
{
	private:
		ID3D11Device*			gDevice;
		ID3D11DeviceContext*	gDeviceContext;
		
		float	mSpeedMultiplier;
		float	mTime; //in seconds
		UINT	mNrOfKeyFrames;
		UINT	mCapacity;
		float**	mTimeTable; //**
		Mesh**	mKeyFrames;
		Mesh*	mInterpolatedMesh;

	private:
		void Expand(); //**

	public:
		MorphAnimation();
		virtual ~MorphAnimation();

		float GetSpeedMultiplier() const;	
		float GetTime() const;				
		UINT GetNrOfKeyFrames() const;
		UINT GetCapacity() const;
		Mesh* GetInterpolatedMesh();

		void SetSpeedMultiplier(float speedMultiplier);

		void Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
		void Update(float deltaTime);

		//todo: add key frame
};
