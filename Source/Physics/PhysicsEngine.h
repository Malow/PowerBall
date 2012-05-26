#pragma once
/**
* Author: Jerry Rahmqvist
* Create Date: 19/05/2012
* 
* 
**/



#define CAPACITY 10
class PowerBall;
class Map;
class GameTimerSimple;
class GraphicsEngine;
#include "..\GraphicsEngine\MaloWLib\Vector.h"
using MaloW::Vector3;
class SoundEffect;
class Text;
class GameNetwork;
#include "..\Network\ServerInfo.h"
class PhysicsEngine
{
	private:
			GameTimerSimple* mGameTimer;
			int mSize;
			int mCapacity;
			PowerBall** mPowerBalls;
			PowerBall* shadow;
			Map* mMap;
			GraphicsEngine* mGe;
			SoundEffect*	mCollisionWithWall;
			SoundEffect*	mCollisionWithBall;
			bool CollisionWithMapSimple(PowerBall* b, Map* map, Vector3 &normalPlane);
			void CollisionMapResponse(PowerBall* b, Map* map, Vector3 normalPlane, float dt);
			bool CollisionWithSphereSimple(PowerBall* b, PowerBall* b1);
			void CollisionSphereResponse(PowerBall* b, PowerBall* b1);
			bool RayTriIntersect(Vector3 origin, Vector3 direction, Vector3 p0, Vector3 p1, Vector3 p2, float &u, float &v, float &t);
			Text* mHud;
			GameNetwork* mNet;
			ServerInfo mServerInfo;
			bool mZoomOutPressed;
			bool mZoomInPressed;

	public:
			PhysicsEngine(GraphicsEngine* ge);
			PhysicsEngine(GraphicsEngine* ge, GameNetwork* net, ServerInfo info);
			~PhysicsEngine();
			void Initialize();
			void AddBody(PowerBall* body);
			void AddMap(Map* map);
			bool RemoveBody(PowerBall* body);
			bool RemoveMap(Map* map);
			void Simulate(bool clientBall = false);
			void SimulateServer();
			void SimulateClient();
			
			void ResetTimers();
			void InputKnockout(float timeStep);
			void HandleClientKeyInputs(const int clientIndex, float diff);
			void ClientKeyPress(float diff, const int index, char key);
			void InputKeysPressedSelf(float diff, int index);
			void SendKeyInputs(const int clientIndex, float diff);
			int Size() const;
};