#pragma once
#include "Vector.h"

#define NEAR 0.1f
#define FAR 100.0f

// Defines where you look at, what's your rotation is, ...
class Camera
{
public:
	// INTERNAL! DON'T USE!
	static void init();

	static void setPos(const vec3& pos);
	static void changePos(vec3 pos);
	static vec3 getPos();

	static void setRotation(const vec2& rotation);
	static void changeRotation(vec2 rotation);
	static vec2 getRotation();

	// Sets the Field of View
	static void setFOV(float fov);
	// Gets the Field of View
	static float getFOV();

	// Locked removes the ability to change the position or rotation
	static void setLocked(bool locked);
	// Locked removes the ability to change the position or rotation
	static bool getLocked();

	// Returns the direction in which the camera is facing
	static vec3 getForwardsVector();
	// Returns the direction that is at the right of the camera
	static vec3 getRightVector();
	// Returns the direction that is up the camera
	static vec3 getUpVector();

	// INTERNAL! DON'T USE!
	static const mat4& getView();
	// INTERNAL! DON'T USE!
	static const mat4& getProjection();
private:
	static void update();

	static vec3 mPos;
	static vec2 mRotation;
	static float mFov;
	static bool mLocked;

	static vec3 mCameraFront;
	static const vec3 mUp;

	static mat4 mView;
	static mat4 mProjection;
};

extern void setupFirstPersonCamera(float sensitivity);