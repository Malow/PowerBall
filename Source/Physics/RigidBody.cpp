#include "RigidBody.h"


RigidBody::RigidBody()
{

}

RigidBody::~RigidBody()
{

}

void RigidBody::CalculateNewValues()
{
	this->mVelocity = this->mMomentum * this->mInverseMass;
    this->mAngularVelocity = this->mAngularMomentum * this->mInverseInertiaTensor;
	this->mOrientation.Normalize();
	Quaternion q(this->mAngularVelocity.x, this->mAngularVelocity.y, this->mAngularVelocity.z, 0);
	//this->mSpin = 0.5f * q * this->mOrientation;
    Matrix4 translation;
	translation.SetTranslate(this->mPosition);
	
    this->mBodyToWorld = translation * this->mOrientation.GetMatrix();
	this->mWorldToBody = this->mBodyToWorld.GetInverse();
}