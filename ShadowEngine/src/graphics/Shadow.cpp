#include "Shadow.h"
#include "Light.h"
#include <GL/glew.h>
#include <core/Camera.cpp>

unsigned int Shadow::mDepthmapFBO = 0;
unsigned int Shadow::mDepthmapTexture = 0;

constexpr unsigned int SHADOW_SIZE = 8192;
constexpr float NEAR_PLANE = 1.0f;
constexpr float FAR_PLANE = 100.0f;
constexpr float DIR_LIGHT_DISTANCE = 20.0f;
constexpr float ORTHO_SIZE = 40.0f;

void Shadow::init()
{
	// -- create and setup framebuffer and texture for depth rendering --
	// create
	glGenFramebuffers(1, &mDepthmapFBO);
	glGenTextures(1, &mDepthmapTexture);

	// setup texture
	glBindTexture(GL_TEXTURE_2D, mDepthmapTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		SHADOW_SIZE, SHADOW_SIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	// setup framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, mDepthmapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mDepthmapTexture, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Shadow::setupShadowDrawing()
{
	glViewport(0, 0, SHADOW_SIZE, SHADOW_SIZE);
	glBindFramebuffer(GL_FRAMEBUFFER, mDepthmapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
}

void Shadow::endShadowDrawing()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, mDepthmapTexture);
	glActiveTexture(GL_TEXTURE0);
}

mat4 Shadow::getLightSpaceMatrix()
{
	if (Light::getDirLights().size() == 0)
	{
		return identity<mat4>();
	}
	mat4 lightProjection = ortho(-ORTHO_SIZE, ORTHO_SIZE, -ORTHO_SIZE, ORTHO_SIZE, NEAR_PLANE, FAR_PLANE);
	vec3 direction = Light::getDirLights()[0]->direction;
	mat4 lightView = lookAt(Camera::getPos() + -direction * DIR_LIGHT_DISTANCE, Camera::getPos() + -direction, vec3(0.0f, 1.0f, 0.0f));
	return lightProjection * lightView;
}
