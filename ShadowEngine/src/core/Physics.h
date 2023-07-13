#pragma once
#include <reactphysics3d.h>
#include "Vector.h"
#include "PhysicalBody.h"
#include "Log.h"

class CollisionListener;

class Physics
{
public:
	static void update();

	static void setGravity(vec3 gravity);
	static void setIsGravityEnabled(bool isGravityEnabled);

	static void setPhysicsWorld(rp3d::DynamicsWorld& physicsWorld);
	static rp3d::DynamicsWorld& getPhysicsWorld();

	static bool collision(PhysicalBody& body1, PhysicalBody& body2);
	static void addCollisionListener(PhysicalBody& body, std::function<void(PhysicalBody& body1, PhysicalBody& body2, vec3 body1LocalHitPoint, vec3 body2LocalHitPoint)> f);
	static void addCollisionListener(PhysicalBody& body1, PhysicalBody& body2, std::function<void(PhysicalBody& body1, PhysicalBody& body2, vec3 body1LocalHitPoint, vec3 body2LocalHitPoint)> f);
	static void addCanJumpListener(PhysicalBody& body, std::function<void(PhysicalBody& body1, PhysicalBody& body2, vec3 body1LocalHitPoint, vec3 body2LocalHitPoint)> f);

private:
	static rp3d::DynamicsWorld* mWorld;
	static std::list<CollisionListener*> mCollisionListeners;
};

class CollisionListener : public rp3d::CollisionCallback
{
public:
	CollisionListener(PhysicalBody& body, std::function<void(PhysicalBody& body1, PhysicalBody& body2, vec3 body1LocalHitPoint, vec3 body2LocalHitPoint)> f)
		: mFunction(f), mBody1(&body)
	{
	}

	CollisionListener(PhysicalBody& body1, PhysicalBody& body2, std::function<void(PhysicalBody & body1, PhysicalBody & body2, vec3 body1LocalHitPoint, vec3 body2LocalHitPoint)> f)
		: mFunction(f), mBody1(&body1), mBody2(&body2)
	{
	}

	PhysicalBody* getBody1()
	{
		return mBody1;
	}

	PhysicalBody* getBody2()
	{
		return mBody2;
	}

	virtual void notifyContact(const CollisionCallbackInfo& collisionCallbackInfo) override;

private:
	std::function<void(PhysicalBody& body1, PhysicalBody& body2, vec3 body1LocalHitPoint, vec3 body2LocalHitPoint)> mFunction;
	PhysicalBody* mBody1 = nullptr;
	PhysicalBody* mBody2 = nullptr;
};

inline rp3d::Vector3 toReact(const vec3& vec)
{
	return reactphysics3d::Vector3(vec.x, vec.y, vec.z);
}

inline vec3 toGML(const rp3d::Vector3& vec)
{
	return vec3(vec.x, vec.y, vec.z);
}