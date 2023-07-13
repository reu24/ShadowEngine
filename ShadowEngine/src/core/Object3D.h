#pragma once
#include "Drawable.h"
#include "graphics/LightingMaterial.h"
#include "PhysicalBody.h"

// The base class of every 3D Object except the Model.
class Object3D : public Drawable
{
public:
	using Drawable::Drawable;

	/*
		Returns the material used for lighting calculations. 
		Use it to change the lighting properties.
	*/
	LightingMaterial& getLightingMaterial()
	{
		return mLightingMaterial;
	}
	/*
		Returns the physical body of the object.
		Use this body to perform physical actions
		and to change the physical properties of the object.
	*/
	PhysicalBody& getPhysicalBody()
	{
		return mPhysicalBody;
	}

	/*
		Sets the material used for lighting calculations.
		With the lighting material you can change lighting properties.
	*/
	void setLightingMaterial(const LightingMaterial& lightingMaterial)
	{
		mLightingMaterial = lightingMaterial;
	}

	/*
		Changes the physical body of the object.
		The physical body is used to perform physical actions
		and to change the physical properties of the object.
	*/
	void setPhysicalBody(const PhysicalBody& physicalBody)
	{
		mPhysicalBody = physicalBody;
	}

	void setPos(const vec3& pos) override
	{
		Drawable::setPos(pos);
		mPhysicalBody.setTransform(mPos, mPhysicalBody.getRotation());
	}
	void setRotation(const vec3& rotation) override
	{
		Drawable::setRotation(rotation);
		mPhysicalBody.setTransform(mPhysicalBody.getPos(), mRotation);
	}

protected:
	LightingMaterial mLightingMaterial;
	
	PhysicalBody mPhysicalBody;
};