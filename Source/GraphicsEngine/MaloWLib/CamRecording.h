/*!
	Class for recording and playing recorded camera movement.
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
		TCBSpline*					mCamPosSpline;
		TCBSpline*					mCamAtSpline;
		//For rendering: **bufferklass?**
		int							mNrOfVertices;
		D3DXVECTOR3**				mVertices;
		ID3D11Buffer*				mVertexBuffer;

	private:
		void DeletePreviousRecording();

	public:
		CamRecording(int interval);
		~CamRecording();
		/*! 
		*	Params: **derp**
		*	camera - pointer to a Camera object.
		*	device - pointer to an ID3D11Device interface to use for rendering.** Specify NULL for this parameter when rendering is not needed.
		*	deviceContext - pointer to an ID3D11DeviceContext interface to use for rendering. Specify NULL for this parameter when rendering is not needed.
			shader - pointer to a Shader object to use for rendering. Specify NULL for this parameter when rendering is not needed.
		*/
		void Init(Camera* camera, ID3D11Device* device, ID3D11DeviceContext* deviceContext, Shader* shader);
	
		int GetInterval() const;
		bool IsRecording() const;
		bool HasRecorded() const;
		bool IsPlaying() const;
		/*! Returns play time in milliseconds. */
		int GetPlayTime() const;
		/*! Returns current play time in milliseconds. */
		int GetCurrentPlayTime() const;
		float GetPlaySpeed() const;

		/*! Set the interval in milliseconds in which to save camera position and look-at position. */
		void SetInterval(int interval);
		/*! Set the play speed. Normal is 1. */
		void SetPlaySpeed(float playSpeed);

		/*! Start/end recording */
		void Record(bool record);
		/*! Start playing what has been recording. */
		void Play();
		/*! Save camera path (points) to file. */
		void Save(string fileName);
		/*! Open camera path (points from file. */
		void Open(string fileName);

		



		//** D3DXVECTOR3 GetCamPos(float t) const;
		//** D3DXVECTOR3 GetCamAt(float t) const;
		//** sitta "attached" på camera klassen? **
		//void AddPosControlPoint(D3DXVECTOR3 pos);
		//void AddAtControlPoint(D3DXVECTOR3 at);
		//**update internt innehålla/hålla reda på addpos & play?**
		//void Update(float deltaTime, D3DXVECTOR3 offset = D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		

		/*! Draw the camera path (points). */
		//void DrawPath(D3DXMATRIX worldViewProj, int nrOfPoints = -1);
};