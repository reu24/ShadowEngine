#pragma once
#include "Vector.h"
#include <string>
#include "Updateable.h"

// The base class of every objects that can be drawn
class Drawable : public Updateable
{
public:
	using Updateable::Updateable;

	/*
		Draws the object.
		Get's automatically called by the RenderLayer.
		DON'T CALL MANUALLY!
	*/
	virtual void draw(bool shadowPass) = 0;

	/*
		Sets the position of the object.
		Note: Use this methode for 3D objecs only!
	*/
	virtual void setPos(const vec3& pos)
	{
		mPos = pos;
	}

	/*
		Sets the position of the object.
		Note: Use this methode for 2D objecs only!
	*/
	virtual void setPos(const vec2& pos)
	{
		mPos = vec3(pos.x, pos.y, 0);
	}

	// Sets the scale of each axis of the object
	virtual void setScale(float scale)
	{
		mScale = vec3(scale);
	}

	// Set the scale of the object
	virtual void setScale(const vec3& scale)
	{
		mScale = scale;
	}

	/*
		Sets the rotation of the object.
		Note: Use this methode for 2D objecs only!
	*/
	virtual void setRotation(float rotation)
	{
		mRotation = vec3(rotation);
	}

	/*
		Sets the rotation of the object.
		Note: Use this methode for 3D objecs only!
	*/
	virtual void setRotation(const vec3& rotation)
	{
		mRotation = rotation;
	}

	// Changes the visibility of the object
	void setVisible(bool visible)
	{
		mVisible = visible;
	}

	// Returns the direction in which the object is facing
	vec3 getForwardsVector()
	{
		vec3 forwardsVector;
		forwardsVector.x = cos(glm::radians(mRotation.x)) * cos(glm::radians(mRotation.y));
		forwardsVector.y = sin(glm::radians(mRotation.x));
		forwardsVector.z = cos(glm::radians(mRotation.x)) * sin(glm::radians(mRotation.y));
		forwardsVector = glm::normalize(forwardsVector);
		return forwardsVector;
	}

	// Returns the direction that is at the right of the object
	vec3 getRightVector()
	{
		return -normalize(cross(vec3(0.0f, 1.0f, 0.0f), getForwardsVector()));
	}

	// Returns the direction that is up the object
	vec3 getUpVector()
	{
		return normalize(cross(getForwardsVector(), getRightVector()));
	}

protected:
	vec3 mPos = vec3(0.0f);
	vec3 mScale = vec3(1.0f);
	vec3 mRotation = vec3(0.0f);
	
	bool mVisible = true;
};