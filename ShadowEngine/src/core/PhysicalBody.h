#pragma once
#include <reactphysics3d.h>
#include "Vector.h"


using rp3d::BodyType;

/*
	This class is used to perform physical actions
	and to change the physical properties of an object.
*/
class PhysicalBody
{
public:
	// INTERNAL! DON'T USE!
	PhysicalBody();
	// INTERNAL! DON'T USE!
	PhysicalBody(rp3d::RigidBody* body);
	~PhysicalBody();

	/*
		INTERNAL! DON'T USE!
		Adds a React Physics 3D Collision Shape to the body.
	*/
	rp3d::ProxyShape* addCollisionShape(rp3d::CollisionShape* collisionShape, rp3d::Transform transform, double mass);
	/*
		INTERNAL! DON'T USE!
		Gets the React Physics 3D Collision Shape from the body.
	*/
	std::vector<rp3d::CollisionShape*> getCollisionShapes();
	/*
		INTERNAL! DON'T USE!
		Removes the React Physics 3D Collision Shape from the body.
	*/
	void removeCollisionShape(const rp3d::ProxyShape* shape);
	/*
		Sets the type of the Body.
		Options: STATIC: If the object shouldn't move at all. 
		DYNAMIC: If the object should behave normally to the laws of physic.
		KINEMATIC: If the object shouldn't collide but use the given velocity.
	*/
	void setType(BodyType type);
	/*
		Changes if the object should be affected by gravity. 
		Note that even if this is off it can still collide. 
		In order to change that use setType.
	*/
	void setEnableGravity(bool enableGravity);
	/*
		Applies Force at a specific position.
		@param force: How strong and in which direction it should move.
		@param point: Where it should be moved at.
	*/
	void applyForce(vec3 force, vec3 point);
	/*
		Applies Force at the center of mass.
		@param force: How strong and in which direction it should move.
	*/
	void applyForceCenter(vec3 force);
	/*
		Applies Torque at the object. Torque changes the angular (rotational) movement.
		@param torque: How strong and in which direction it should be rotated.
	*/
	void applyTorque(vec3 torque);
	// Changes the velocity of the object.
	void setLinearVelocity(vec3 velocity);
	// Changes the angular (rotational) velocity of the object.
	void setAngularVelocity(vec3 velocity);
	// Changes the body's position and rotation.
	void setTransform(vec3 position, vec3 rotation);
	// Changes how bouncy the object is.
	void setBounciness(double bounciness);
	/*
		Changes the friction coefficent.
		@param frictionCoefficent: The friction coefficent is how good objects slip on this object. 
		If it is low objects easily slip away while if it's high the objects on it barely move.
	*/
	void setFrictionCoefficient(double frictionCoefficient);
	// Changes the rolling restistance.
	void setRollingResistance(double rollingRestistance);

	// Returns the velocity of the body.
	vec3 getLinearVelocity();
	// Returns the angular (rotational) velocity of the body.
	vec3 getAngularVelocity();
	// Returns the position of the body.
	vec3 getPos();
	// Returns the rotation of the body.
	vec3 getRotation();
	/*
		INTERNAL! DON'T USE!
		The React Physics 3D Body.
	*/
	rp3d::RigidBody* getBody() const;

	/*
		Tests if this object is within a specific line.
		@param beginningPoint: Where the line starts.
		@param endPoint: Where the line ends.
	*/
	bool raycast(vec3 beginningPoint, vec3 endPoint);
private:
	rp3d::RigidBody* mBody;
	std::vector<rp3d::CollisionShape*> mCollisionShapes;
	bool mShouldDeleteBody = true;
};