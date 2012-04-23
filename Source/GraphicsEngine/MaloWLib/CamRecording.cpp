#include "CamRecording.h"

//private
void CamRecording::DeletePreviousRecording()
{
	this->mCamAtSpline->Clear();
	this->mCamPosSpline->Clear();
}

//de/con-structors, init, other
CamRecording::CamRecording(int interval)
{
	this->gCamera = NULL;

	this->gDevice = NULL;
	this->gDeviceContext = NULL;
	this->gShader = NULL;

	this->mIsRecording = false;
	this->mHasRecorded = false;
	this->mIsPlaying = false;
	this->mInterval = interval;
	this->mPlayTime = 0.0f;
	this->mCurrentPlayTime = 0.0f;
	this->mPlaySpeed = 1.0f;
	this->mPathOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	this->mCamPosSpline = new TCBSpline();
	this->mCamAtSpline = new TCBSpline();
	
	this->mNrOfVertices = 0;
	this->mVertices = NULL;
	this->mVertexBuffer = NULL;
}
CamRecording::~CamRecording()
{
	SAFE_DELETE(this->mCamPosSpline);
	SAFE_DELETE(this->mCamAtSpline);

	if(this->mVertices)
	{
		for(int i = 0; i < this->mNrOfVertices; i++)
		{
			SAFE_DELETE(this->mVertices[i]);
		}
		SAFE_DELETE_ARRAY(this->mVertices);
	}
	SAFE_RELEASE(this->mVertexBuffer) ;
}

void CamRecording::Init(Camera* camera, ID3D11Device* device, ID3D11DeviceContext* deviceContext, Shader* shader)
{
	this->gCamera = camera;
	this->gDevice = device;
	this->gDeviceContext = deviceContext;
	this->gShader = shader;
}

//get
int CamRecording::GetInterval() const
{
	return this->mInterval;
}
bool CamRecording::IsRecording() const
{
	return this->mIsRecording;
}
bool CamRecording::HasRecorded() const
{
	return this->mHasRecorded;
}
bool CamRecording::IsPlaying() const
{
	return this->mIsPlaying;
}
float CamRecording::GetPlayTime() const
{
	return this->mPlayTime;
}
float CamRecording::GetCurrentPlayTime() const
{
	return this->mCurrentPlayTime;
}
float CamRecording::GetPlaySpeed() const
{
	return this->mPlaySpeed;
}
D3DXVECTOR3 CamRecording::GetPathOffset() const
{
	return this->mPathOffset;
}

//set
void CamRecording::SetInterval(int interval)
{
	this->mInterval = interval;
}
void CamRecording::SetPlaySpeed(float playSpeed)
{
	this->mPlaySpeed = playSpeed;
}
void CamRecording::SetPathOffset(D3DXVECTOR3 pathOffset)
{
	this->mPathOffset = pathOffset;
}

// other
void CamRecording::AddCameraWaypoint(D3DXVECTOR3 position, D3DXVECTOR3 lookAt)
{
	this->mCamPosSpline->AddControlPoint(position);
	this->mCamAtSpline->AddControlPoint(lookAt);
	this->mHasRecorded = true;
}

void CamRecording::Record(bool record)
{
	this->mIsRecording = record;
	if(record) //check if something has already been recorded and delete is if that's the case
	{
		if(this->mHasRecorded)
		{
			CamRecording::DeletePreviousRecording();
		}
	}
	else 
		this->mHasRecorded = true;
}
void CamRecording::Play()
{
	if(this->mHasRecorded) 
	{
		//initialize the splines when starting play
		HRESULT hr = S_OK;
		hr = this->mCamPosSpline->Init();
		if(hr == S_OK) hr = this->mCamAtSpline->Init();
		if(FAILED(hr))
		{
			MaloW::Debug("CamRecording: Error: Failed to initialize spline(s)");
		}

		this->mIsPlaying = true;
		this->mPlayTime = (this->mCamPosSpline->GetNrOfControlPoints() - 1) * this->mInterval * 0.001f * this->mPlaySpeed;
	}
	else
	{
		MaloW::Debug("CamRecording: Warning: Tried to play when there's nothing to play.");
	}
}
void CamRecording::Save(string fileName)
{
	this->mCamPosSpline->WriteControlPointsToFile("Pos_" + fileName);
	this->mCamAtSpline->WriteControlPointsToFile("At_" + fileName);
}
void CamRecording::Open(string fileName)
{
	if(this->mHasRecorded)
	{
		CamRecording::DeletePreviousRecording();
	}
	bool success = false;
	success = this->mCamPosSpline->ReadControlPointsFromFile("Pos_" + fileName);
	if(!success)
	{
		MaloW::Debug("CamRecording: Warning: Failed to read control points from file.");
	}
	success = this->mCamAtSpline->ReadControlPointsFromFile("At_" + fileName);
	if(!success)
	{
		MaloW::Debug("CamRecording: Warning: Failed to read control points from file.");
	}
	else //if succesful read, initialize splines
	{
		HRESULT hr = S_OK;
		hr = this->mCamPosSpline->Init();
		if(FAILED(hr))
		{
			MaloW::Debug("CamRecording: Error: Failed to initialize splines from file.");
			return;
		}
		hr = this->mCamAtSpline->Init();
		if(FAILED(hr))
		{
			MaloW::Debug("CamRecording: Error: Failed to initialize splines from file.");
			return;
		}

		this->mHasRecorded = true;
	}
	

	if(this->gDevice) //if rendering is used, initialize rendering
	{
		//CamRecording::InitDrawing(this->gDevice); todo**
	}
}

void CamRecording::Update(float deltaTime)
{
	//milliseconds per frame for 10 fps (1 / 10fps). used for when deltatime > 1. 
	//Ex: interval = 1000. time = 999. (int)deltaTime = 2. time += (int)deltatime = 1001. 
	static int intervalEpsilon = 100; 
	static float time = 0.0f;

	if(this->mIsRecording)
	{
		time += deltaTime;
		int intervalTime = (int)time % (this->mInterval + intervalEpsilon); //+ intervalEpsilon because of "time += intervalEpsilon"

		if(intervalTime >= 0 && intervalTime < intervalEpsilon) 
		{
			this->mCamPosSpline->AddControlPoint(this->gCamera->getPosition());
			this->mCamAtSpline->AddControlPoint(this->gCamera->getPosition() + this->gCamera->getForward());
			time += intervalEpsilon;
		}
	}
	else if(this->mIsPlaying) //todo: disable camera input**
	{
		this->mCurrentPlayTime += deltaTime * 0.001f; 

		if(this->mCurrentPlayTime < this->mPlayTime)
		{
			float t = (float)this->mCurrentPlayTime / (float)this->mPlayTime;
			D3DXVECTOR3 pos, at;
			
			pos = this->mCamPosSpline->GetPoint(t) + this->mPathOffset;
			at = this->mCamAtSpline->GetPoint(t) + this->mPathOffset;
			
			this->gCamera->setPosition(pos);
			this->gCamera->LookAt(at);
		}
		else
		{
			this->mIsPlaying = false;
			this->mCurrentPlayTime = 0;
		}
	}
}

/*
void CamRecording::Render(int nrOfPoints = -1)
{
	if(this->mNrOfVertices > 1)
	{
		UINT stride = 12;
		UINT offset = 0;
		this->mEffect->GetConstantBufferByName("cbPerFrame")->GetMemberByName("pf_WVP")->AsMatrix()->SetMatrix(viewProj);
		this->gDevice->IASetInputLayout(this->mInputLayout);
		this->gDevice->IASetVertexBuffers(0, 1, &this->mVertexBuffer, &stride, &offset);
		this->gDevice->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
		this->mEffectTech->GetPassByIndex(0)->Apply(0);
		this->gDevice->Draw(this->mNrOfVertices, 0);
	}
}*/