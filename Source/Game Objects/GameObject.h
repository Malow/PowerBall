#pragma once
/**
* Author: Jerry Rahmqvist
* Create Date: 09/05/2012
* 
* This Class is the abstract class for game objects.
*
**/

#include "GraphicsEngine.h"
#include "..\Physics\Matrix3.h"
#include "..\Physics\Matrix4.h"

using namespace MaloW;

class GameObject
{
	protected:
			StaticMesh*	mMesh;
			D3DXVECTOR3	mStartPosition;
	public:
			GameObject();
			virtual ~GameObject();
		
			/*! Returns the position of the game object in world space (retrieved from mMesh). */
			D3DXVECTOR3 GetPosition() const;

			/*! Returns the position of the game object in world space (retrieved from mMesh). */
			Vector3 GetPositionVector3() const;

			/*! Returns the mesh of the game object. */
			StaticMesh* GetMesh() const;
		
			/*! Sets the position of the game object in world space. */
			void SetPosition(const D3DXVECTOR3 position);
			
			/*! Sets the position of the game obejct in world space. */
			void SetPosition(const float x, const float y, const float z);

			/*! Sets the position of the game object in world space. */
			void SetPosition(const Vector3 position);

			/*! Sets the game object to its startposition. */
			virtual void SetToStartPosition() = 0;

			/*! Sets the mesh of the game object. */
			void SetMesh(StaticMesh* mesh);

};