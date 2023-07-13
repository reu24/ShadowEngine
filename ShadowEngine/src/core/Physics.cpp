#include "Physics.h"
#include "Log.h"
#include <collision/ContactManifold.h>
#include <constraint/ContactPoint.h>

rp3d::DynamicsWorld* Physics::mWorld = nullptr;
std::list<CollisionListener*> Physics::mCollisionListeners;

void Physics::update()
{
	for (auto collisionListenerPtr : mCollisionListeners)
	{
		CollisionListener* collisionListener = &*collisionListenerPtr;
		if (collisionListener->getBody2() == nullptr)
		{
			mWorld->testCollision(collisionListener->getBody1()->getBody(), collisionListener);
		}
		else
		{
			mWorld->testCollision(collisionListener->getBody1()->getBody(), collisionListener->getBody2()->getBody(), collisionListener);
		}
	}

	mWorld->update(1.0f / 60.0f);
}

void Physics::setGravity(vec3 gravity)
{
	mWorld->setGravity(toReact(gravity));
}

void Physics::setIsGravityEnabled(bool isGravityEnabled)
{
	mWorld->setIsGratityEnabled(isGravityEnabled);
}

void Physics::setPhysicsWorld(rp3d::DynamicsWorld& physicsWorld)
{
	// remove old collision listeners
	for (auto listener : mCollisionListeners)
	{
		delete listener;
	}
	mCollisionListeners.clear();

	// change world
	mWorld = &physicsWorld;
}

rp3d::DynamicsWorld& Physics::getPhysicsWorld()
{
	return *mWorld;
}

bool Physics::collision(PhysicalBody& body1, PhysicalBody& body2)
{
	return mWorld->testOverlap(body1.getBody(), body2.getBody());
}
// TODO canJump()
void Physics::addCollisionListener(PhysicalBody& body, std::function<void(PhysicalBody& body1, PhysicalBody& body2, vec3 body1LocalHitPoint, vec3 body2LocalHitPoint)> f)
{
	CollisionListener* collisionListener = new CollisionListener(body, f);
	mCollisionListeners.push_back(collisionListener);
	mWorld->testCollision(body.getBody(), collisionListener);
}

void Physics::addCollisionListener(PhysicalBody& body1, PhysicalBody& body2, std::function<void(PhysicalBody& body1, PhysicalBody& body2, vec3 body1LocalHitPoint, vec3 body2LocalHitPoint)> f)
{
	CollisionListener* collisionListener = new CollisionListener(body1, body2, f);
	mCollisionListeners.push_back(collisionListener);
	mWorld->testCollision(body1.getBody(), body2.getBody(), collisionListener);
}

void Physics::addCanJumpListener(PhysicalBody& body, std::function<void(PhysicalBody& body1, PhysicalBody& body2, vec3 body1LocalHitPoint, vec3 body2LocalHitPoint)> f)
{
	addCollisionListener(body, [f](PhysicalBody body1, PhysicalBody& body2, vec3 body1LocalHitPoint, vec3 body2LocalHitPoint)
		{
			if (body1LocalHitPoint.y == 0.5)
			{
				f(body1, body2, body1LocalHitPoint, body2LocalHitPoint);
			}
		});
}

void CollisionListener::notifyContact(const CollisionCallbackInfo& collisionCallbackInfo)
{
	rp3d::ContactPoint* contactPoint = collisionCallbackInfo.contactManifoldElements->getContactManifold()->getContactPoints();
	rp3d::RigidBody* body1 = dynamic_cast<rp3d::RigidBody*>(collisionCallbackInfo.body1);
	rp3d::RigidBody* body2 = dynamic_cast<rp3d::RigidBody*>(collisionCallbackInfo.body2);
	mFunction(PhysicalBody(body1), PhysicalBody(body2), toGML(contactPoint->getLocalPointOnShape1()), toGML(contactPoint->getLocalPointOnShape2()));
}
