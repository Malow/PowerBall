#include "Platform.h"

Platform::Platform(const string meshFilePath, D3DXVECTOR3 position)
{
	this->mMesh		     = GetGraphicsEngine()->CreateStaticMesh(meshFilePath, position); //D3DXVECTOR3(10, 10, 10)
	this->mShrink	     = 0.03f;
	this->mRadius	     = 100000.0f;
	this->mScaledRadius  = this->mRadius;
	this->mMass			 = 100000.0f;
	this->mRestitution   = 0.60f;
	this->mIsRotating	 = false;
	this->mAngleX		 = 0.0f;
	this->mAngleZ		 = 0.0f;
	this->mMaxAngleX	 = 0.0f;
	this->mMaxAngleZ	 = 0.0f;
	this->mMeshHotZone = NULL;
	this->mAngleY = 0.0f;
	
}
Platform::~Platform()
{
	GetGraphicsEngine()->DeleteStaticMesh(this->mMesh);
	if(this->mMeshHotZone)
		GetGraphicsEngine()->DeleteStaticMesh(this->mMeshHotZone);
}

float Platform::GetY(const float x, const float z) const
{
	x;
	z;
	//either get vertex height from engine
	//or calc the distance of x,z to the center of platform and compare it to radius of platform. (this requires to have a specified Y-value of the platform)


	return -1.0f;
}

float Platform::GetYInterpolated(const float x, const float z) const
{
	x;
	z;
	//perform bilinear interpolation (this function is only needed if the platform surface have height differences.
	return -1.0f;
}
Vector3 Platform::GetPositionXZ() const
{
	D3DXVECTOR3 temp = this->mMesh->GetPosition();
	Vector3 temp2 = Vector3(temp.x,0,temp.z);
	return temp2;
}
void Platform::Update(const float dt)
{
	if(this->mIsRotating)
	{		
		float angle;
		float speed = PI/26.0f;
		if(this->mTargetAngleX <0)
			angle = -speed*dt*0.001f;
		else
			angle = speed*dt*0.001f;
		Matrix3 temp;
		temp.SetRotationZ(this->mAngleZ);
		Vector3 xA = Vector3(1,0,0);
		Vector3 xAnew = temp * xA;
		this->mAngleX += angle;
		this->mMesh->RotateAxis(xAnew.GetD3DVec(), angle);
		
		if(this->mTargetAngleZ <0)
			angle = -speed*dt*0.001f;
		else
			angle = speed*dt*0.001f;
		this->mAngleZ += angle;
		this->mMesh->RotateAxis(D3DXVECTOR3(0,0,1), angle);
	}
	if(this->mMeshHotZone)
	{
		
		Vector3 scalingMesh = this->mMesh->GetScaling();
		Vector3 pos = this->mMesh->GetPosition();
		D3DXMATRIX quat;
		D3DXMatrixRotationQuaternion(&quat, &this->mMesh->GetRotation()); 
		
		Matrix4 rotate(quat);
		rotate.TransposeThis();

		Matrix4 scaling;
		scaling.SetScale(scalingMesh);

		Matrix4 translate;
		translate.SetTranslate(pos);
	
		Matrix4 rotateY;
		rotateY.SetRotationY(dt*0.001f*(PI/2.0f));
		Matrix4 newRotate = rotateY*rotate;
		
		

		Matrix4 world = translate*rotate*scaling;
		Vector3 posHotZone = Vector3(13.5f,2.5,13.5f);
		Vector3 newPos4HotZoneMesh = world * posHotZone;
		
		
		Vector4 newYrot = rotate*Vector4(0,1,0,0);
		Vector3 newYrot2 = Vector3(newYrot.x, newYrot.y, newYrot.z);


		D3DXQUATERNION q = this->mMesh->GetRotation();
		

		this->mMeshHotZone->SetQuaternion(this->mMesh->GetRotation());
		this->mAngleY += dt*0.001f*(PI/2.0f);

		this->mMeshHotZone->SetQuaternion(q);
		this->mMeshHotZone->SetPosition(newPos4HotZoneMesh.GetD3DVec());
		this->mMeshHotZone->RotateAxis(newYrot2.GetD3DVec(), this->mAngleY);
		
		Vector3 hotPos = Vector3(13.5f,4.5,13.5f);
		Vector3 newHotPos = world * hotPos;
		/* test to see that the flag mesh is not straight and thats true. */
		//this->mMeshHotZone->RotateAxis(D3DXVECTOR3(0,1,0), dt*0.001f*(PI/2.0f));
		this->mHotZonePosition = newHotPos;
	}
	float newdt = dt * 0.001f;
	float fraction = 1.0f - this->mShrink * newdt;
	this->mMesh->Scale(D3DXVECTOR3(fraction,1,fraction));
	this->mScaledRadius *= fraction;//this->mScaledRadius/this->mRadius;
}
/*void Platform::Update(const float dt)
{
	
	float fraction = 1.0f-this->mShrink*dt;
	this->mScaledRadius *= fraction;//this->mScaledRadius/this->mRadius;
	this->mMesh->Scale(D3DXVECTOR3(fraction,1,fraction));
}*/

bool Platform::IsTargetAngleReachedX() const
{
	if(this->mTargetAngleX < 0)
	{
		if( this->mAngleX > this->mTargetAngleX )
			return false;
		else
			return true;
	}
	else
	{
		if( this->mAngleX < this->mTargetAngleX)
			return false;
		else
			return true;
	}
}

bool Platform::IsTargetAngleReachedZ() const
{
	if(this->mTargetAngleZ < 0)
	{
		if( this->mAngleZ > this->mTargetAngleZ )
			return false;
		else
			return true;
	}
	else
	{
		if( this->mAngleZ < this->mTargetAngleZ)
			return false;
		else
			return true;
	}
}


bool Platform::IsOnPlatform(const float x, const float z) const
{
	D3DXVECTOR3 pos = this->mMesh->GetPosition();

	//calculate the distance from the XZ-center of the platform to the XZ-position
	float distance = sqrt(pow(pos.x - x, 2) + pow(pos.z - z, 2));

	bool isOn = false;
	if(distance < this->mScaledRadius)
		isOn = true;

	return isOn;
}

bool Platform::IsInHotZone(Vector3 positionOfBall, float radiusBall)
{
	float distance = (this->mHotZonePosition - positionOfBall).GetLength();
	if(distance <= (this->mHotZoneRadius - radiusBall))
		return true;
	return false;
}

void Platform::RotateX(float dt)
{
	float angle = (PI/8.0f)*dt*0.001f;
	if((this->mAngleX+angle) > -this->mMaxAngleX && (this->mAngleX+angle) < this->mMaxAngleX)
	{
		Matrix3 temp;
		temp.SetRotationZ(this->mAngleZ);
		Vector3 xA = Vector3(1,0,0);
		Vector3 xAnew = temp * xA;
		this->mMesh->RotateAxis(xAnew.GetD3DVec(), angle);
		this->mAngleX +=angle;
	}
}

void Platform::RotateZ(float dt)
{
	float angle = (PI/8.0f)*dt*0.001f;
	if((this->mAngleZ+angle) > -this->mMaxAngleZ && (this->mAngleZ+angle) < this->mMaxAngleZ)
	{
		/* we don't need the same proc. as for the RotateX 
		   because we don't want to change the rotation axis.
		   if we do change the mesh would rotate around the y-axis from the
		   original base.
		*/
		Vector3 zA = Vector3(0,0,1);
		this->mMesh->RotateAxis(zA.GetD3DVec(), angle);
		this->mAngleZ +=angle;
	}
}

void Platform::Reset()
{
	Vector3 theScale = Vector3(this->mMesh->GetScaling());
	Vector3 inverseScale = theScale.GetInverseComponents();
	this->SetScale(inverseScale);
}