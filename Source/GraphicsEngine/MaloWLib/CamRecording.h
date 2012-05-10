//Written by Markus Tillman.

/*!
	Class for recording and playing recorded camera movement.
	Note that time is handled in milliseconds.
	Todo: rendering and disable camera input when playing**
*/

#pragma once

#include "..\..\stdafx.h"
#include "Camera.h"
#include "Shader.h"
#include "..\..\MaloWLib\TCBSpline.h"

enum CAMERA_PATH
{
	SPIRAL_DOWN = 0,
	CIRCLE_AROUND = 1
};

class CamRecording
{
	private:
		//Global variables:
		//For recording/playing:
		Camera*					gCamera;	
		//For rendering:
		ID3D11Device*			gDevice;
		ID3D11DeviceContext*	gDeviceContext;
		Shader*					gShader; //**outside of class**

		//Local variables:
		//For recording/playing:
		bool						mIsLoopingSeamless;
		bool						mIsRecording;
		bool						mHasRecorded;
		bool						mIsPlaying;
		int							mInterval; //in milliseconds
		float						mPlayTime; //in seconds
		float						mCurrentPlayTime; //in seconds
		float						mPlaySpeed;
		D3DXVECTOR3					mPathOffset;
		TCBSpline*					mCamPosSpline;
		TCBSpline*					mCamAtSpline;
		//For rendering: use bufferclass?**
		int							mNrOfVertices;
		D3DXVECTOR3**				mVertices;
		ID3D11Buffer*				mVertexBuffer;

	private:
		void DeletePreviousRecording();
		//InitRendering()**

	public:
		CamRecording(int interval = 1000, bool seamLessPath = false);
		~CamRecording();
		/*! 
			camera - pointer to a Camera object. (Cannot be NULL).
			device, deviceContext and shader are used for rendering. Specify NULL for these parameters when rendering is not needed.
		*/
		void Init(Camera* camera, ID3D11Device* device = NULL, ID3D11DeviceContext* deviceContext = NULL, Shader* shader = NULL);
	
		bool IsLooping() const;
		bool IsRecording() const;
		bool HasRecorded() const;
		bool IsPlaying() const;
		int GetInterval() const;
		/*! Returns play time in seconds. */
		float GetPlayTime() const;
		/*! Returns current play time in seconds. */
		float GetCurrentPlayTime() const;
		float GetPlaySpeed() const;
		D3DXVECTOR3 GetPathOffset() const;

		void LoopSeamLess();
		void StopLooping();
		/*! Set the interval in milliseconds in which to save camera position and look-at position. */
		void SetInterval(int interval);
		/*! Set the play speed. Normal is 1. */
		void SetPlaySpeed(float playSpeed);
		/*! Set the offset to the camera path when playing. */
		void SetPathOffset(D3DXVECTOR3 pathOffset);

		/* ! Add position and lookat to path. */
		void AddCameraWaypoint(D3DXVECTOR3 position, D3DXVECTOR3 lookAt);
		/*! Start/end recording */
		void Record(bool record);
		/*! Start playing what has been recording. */
		void Play();
		/*! Save camera path (points) to file. */
		void Save(string fileName);
		/*! Open camera path (points) from file. */
		void Open(string fileName);
		/*! Set the camera to circle around a target. */
		/*! loop - set path to be looped. */
		/*! interval - number of milliseconds between each point */
		/*! nrOfPoints - the number of points (camera & lookAt positions) on the circle. */
		/*! nrOfRotations - the number of rotations to do if looping is set to false. */
		/*! startPos - start position of camera. */
		/*! lookAt - what the camera will look at during play. */
		void CircleAround(	bool loop, 
							unsigned int interval, 
							unsigned int nrOfPoints, 
							unsigned int nrOfRotations, 
							D3DXVECTOR3 startPos, 
							D3DXVECTOR3 lookAt);

		/*! Load predefined camera path. */
		//void Load(CAMERA_PATH camPath);

		/*! Used to update recording and playing. */
		void Update(float deltaTime);


		/*! Render the camera path (points). */
		//void Render(int nrOfPoints = -1);**
};