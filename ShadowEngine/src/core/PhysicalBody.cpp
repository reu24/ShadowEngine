#include "PhysicalBody.h"
#include "Physics.h"

PhysicalBody::PhysicalBody()
{
	mBody = Physics::getPhysicsWorld().createRigidBody(rp3d::Transform());
}

PhysicalBody::PhysicalBody(rp3d::RigidBody* body)
{
	mBody = body;
	mShouldDeleteBody = false;
}

PhysicalBody::~PhysicalBody()
{
	if (mShouldDeleteBody)
	{
		for (int i = 0; i < mCollisionShapes.size(); i++)
		{
			delete mCollisionShapes[i];
		}
		mCollisionShapes.clear();

		Physics::getPhysicsWorld().destroyRigidBody(mBody);
	}
}

rp3d::ProxyShape* PhysicalBody::addCollisionShape(rp3d::CollisionShape* collisionShape, rp3d::Transform transform, double mass)
{
	rp3d::ProxyShape* proxy = mBody->addCollisionShape(collisionShape, transform, mass);
	mCollisionShapes.push_back(collisionShape);
	return proxy;
}

std::vector<rp3d::CollisionShape*> PhysicalBody::getCollisionShapes()
{
	return mCollisionShapes;
}

void PhysicalBody::removeCollisionShape(const rp3d::ProxyShape* shape)
{
	mBody->removeCollisionShape(shape);
}

void PhysicalBody::setType(BodyType type)
{
	mBody->setType(type);
}

void PhysicalBody::setEnableGravity(bool enableGravity)
{
	mBody->enableGravity(enableGravity);
}

void PhysicalBody::applyForce(vec3 force, vec3 point)
{
	mBody->applyForce(toReact(force), toReact(point));
}

void PhysicalBody::applyForceCenter(vec3 force)
{
	mBody->applyForceToCenterOfMass(toReact(force));
}

void PhysicalBody::applyTorque(vec3 torque)
{
	mBody->applyTorque(toReact(torque));
}

void PhysicalBody::setLinearVelocity(vec3 velocity)
{
	mBody->setLinearVelocity(toReact(velocity));
}

void PhysicalBody::setAngularVelocity(vec3 velocity)
{
	mBody->setAngularVelocity(toReact(velocity));
}

void PhysicalBody::setTransform(vec3 position, vec3 rotation)
{
	rotation = radians(rotation);
	rp3d::Transform transform;
	transform.setPosition(toReact(position));
	rp3d::Quaternion quaternion = rp3d::Quaternion::fromEulerAngles(toReact(rotation));
	transform.setOrientation(quaternion);
	mBody->setTransform(transform);
}

void PhysicalBody::setBounciness(double bounciness)
{
	mBody->getMaterial().setBounciness(bounciness);
}

void PhysicalBody::setFrictionCoefficient(double frictionCoefficient)
{
	mBody->getMaterial().setFrictionCoefficient(frictionCoefficient);
}

void PhysicalBody::setRollingResistance(double rollingRestistance)
{
	mBody->getMaterial().setRollingResistance(rollingRestistance);
}

vec3 PhysicalBody::getLinearVelocity()
{
	return toGML(mBody->getLinearVelocity());
}

vec3 PhysicalBody::getAngularVelocity()
{
	return toGML(mBody->getAngularVelocity());
}

vec3 PhysicalBody::getPos()
{
	return toGML(mBody->getTransform().getPosition());
}

vec3 PhysicalBody::getRotation()
{
	rp3d::Quaternion orientation = mBody->getTransform().getOrientation();
	quat orientationGML;
	orientationGML.x = orientation.x;
	orientationGML.y = orientation.y;
	orientationGML.z = orientation.z;
	orientationGML.w = orientation.w;
	vec3 angles = eulerAngles(orientationGML);
	angles = degrees(angles);
	return angles;
}

rp3d::RigidBody* PhysicalBody::getBody() const
{
	return mBody;
}

bool PhysicalBody::raycast(vec3 beginningPoint, vec3 endPoint)
{
	rp3d::Ray ray(toReact(beginningPoint), toReact(endPoint));
	rp3d::RaycastInfo rayCastInfo;
	return mBody->raycast(ray, rayCastInfo);
}
