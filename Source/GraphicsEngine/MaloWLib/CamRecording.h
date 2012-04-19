/*!
	Class for recording and playing recorded camera movement.
	Note that time is handled in milliseconds.
*/

#pragma once

#include "..\..\stdafx.h"
#include "Camera.h"
#include "Shader.h"
#include "..\..\MaloWLib\TCBSpline.h"


class CamRecording
{
	private:
		//Global variables:
		//For recording/playing:
		Camera*					gCamera;	
		//For rendering:
		ID3D11Device*			gDevice;
		ID3D11DeviceContext*	gDeviceContext;
		Shader*					gShader;

		//Local variables:
		//For recording/playing:
		bool						mIsRecording;
		bool						mHasRecorded;
		bool						mIsPlaying;
		int							mInterval;
		int							mPlayTime; 
		int							mCurrentPlayTime;
		float						mPlaySpeed;
		D3DXVECTOR3					mPathOffset;
		TCBSpline*					mCamPosSpline;
		TCBSpline*					mCamAtSpline;
		//For rendering: **bufferklass?**
		int							mNrOfVertices;
		D3DXVECTOR3**				mVertices;
		ID3D11Buffer*				mVertexBuffer;

	private:
		void DeletePreviousRecording();
		//**InitRendering()**

	public:
		CamRecording(int interval = 1000);
		~CamRecording();
		/*! 
			camera - pointer to a Camera object. (Cannot be NULL).
			device, deviceContext and shader are used for rendering. Specify NULL for these parameters when rendering is not needed.
		*/
		void Init(Camera* camera, ID3D11Device* device = NULL, ID3D11DeviceContext* deviceContext = NULL, Shader* shader = NULL);
	
		int GetInterval() const;
		bool IsRecording() const;
		bool HasRecorded() const;
		bool IsPlaying() const;
		/*! Returns play time in milliseconds. */
		int GetPlayTime() const;
		/*! Returns current play time in milliseconds. */
		int GetCurrentPlayTime() const;
		float GetPlaySpeed() const;
		D3DXVECTOR3 GetPathOffset() const;

		/*! Set the interval in milliseconds in which to save camera position and look-at position. */
		void SetInterval(int interval);
		/*! Set the play speed. Normal is 1. */
		void SetPlaySpeed(float playSpeed);
		void SetPathOffset(D3DXVECTOR3 pathOffset);

		/*! Start/end recording */
		void Record(bool record);
		/*! Start playing what has been recording. */
		void Play();
		/*! Save camera path (points) to file. */
		void Save(string fileName);
		/*! Open camera path (points from file. */
		void Open(string fileName);

		
		void Update(float deltaTime);



		//** D3DXVECTOR3 GetCamPos(float t) const;
		//** D3DXVECTOR3 GetCamAt(float t) const;
		//** sitta "attached" på camera klassen? **
		//void AddPosControlPoint(D3DXVECTOR3 pos);
		//void AddAtControlPoint(D3DXVECTOR3 at);
		//**update internt innehålla/hålla reda på addpos & play?**
		
		

		/*! Render the camera path (points). */
		//void Render(D3DXMATRIX worldViewProj, int nrOfPoints = -1);
};