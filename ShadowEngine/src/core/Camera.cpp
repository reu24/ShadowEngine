#include "Camera.h"
#include <glm/ext/matrix_transform.hpp>
#include <graphics/Window.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <core/Input.h>

vec3 Camera::mPos;
vec2 Camera::mRotation;
float Camera::mFov = 45.0f;
bool Camera::mLocked = false;

vec3 Camera::mCameraFront = vec3(0.0f, 0.0f, -1.0f);
const vec3 Camera::mUp = vec3(0.0f, 1.0f, 0.0f);

mat4 Camera::mView;
mat4 Camera::mProjection;

void Camera::init()
{
	update();
}

void Camera::setPos(const vec3& pos)
{
	if (!mLocked)
	{
		mPos = pos;
		update();
	}
}

vec3 Camera::getPos()
{
	return mPos;
}

void Camera::changePos(vec3 pos)
{
	if (!mLocked)
	{
		mPos += pos;
		update();
	}
}

void Camera::setRotation(const vec2& rotation)
{
	if (!mLocked)
	{
		mRotation = rotation;
		update();
	}
}

vec2 Camera::getRotation()
{
	return mRotation;
}

void Camera::setFOV(float fov)
{
	if (!mLocked)
	{
		mFov = fov;
		update();
	}
}

float Camera::getFOV()
{
	return mFov;
}

void Camera::setLocked(bool locked)
{
	mLocked = locked;
}

bool Camera::getLocked()
{
	return mLocked;
}

vec3 Camera::getForwardsVector()
{
	return mCameraFront;
}

vec3 Camera::getRightVector()
{
	return -normalize(cross(mUp, getForwardsVector()));
}

vec3 Camera::getUpVector()
{
	return normalize(cross(getForwardsVector(), getRightVector()));
}

void Camera::changeRotation(vec2 rotation)
{
	if (!mLocked)
	{
		mRotation += rotation;
		update();
	}
}

const mat4& Camera::getView()
{
	return mView;
}

const mat4& Camera::getProjection()
{
	return mProjection;
}

void Camera::update()
{
	// view matrix
	if (mRotation.x > 89.0f)
		mRotation.x = 89.0f;
	if (mRotation.x < -89.0f)
		mRotation.x = -89.0f;

	mCameraFront.x = cos(glm::radians(mRotation.x)) * cos(glm::radians(mRotation.y));
	mCameraFront.y = sin(glm::radians(mRotation.x));
	mCameraFront.z = cos(glm::radians(mRotation.x)) * sin(glm::radians(mRotation.y));
	mCameraFront = glm::normalize(mCameraFront);

	mView = glm::lookAt(mPos, mPos + mCameraFront, mUp);

	// projection matrix
	vec2 windowSize = Window::getSize();
	float aspect = windowSize.x / windowSize.y;
	mProjection = glm::perspective(glm::radians(mFov), aspect, NEAR, FAR);
}

void setupFirstPersonCamera(float sensitivity)
{
	Input::addCursorListener([sensitivity](glm::vec2 cursor)
		{
			static float lastX = cursor.x;
			static float lastY = cursor.y;

			float xOffset = cursor.x - lastX;
			float yOffset = lastY - cursor.y;
			lastX = cursor.x;
			lastY = cursor.y;

			xOffset *= sensitivity;
			yOffset *= sensitivity;

			static float pitch = 0.0f, yaw = -90.0f;

			yaw += xOffset;
			pitch += yOffset;

			Camera::setRotation(vec2(pitch, yaw));
		});
}
