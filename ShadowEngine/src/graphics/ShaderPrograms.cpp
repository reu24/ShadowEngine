#include "ShaderPrograms.h"
#include "ShaderStrings.h"

Shader ShaderPrograms::mLightingShader;
Shader ShaderPrograms::mLightingDepthShader;
Shader ShaderPrograms::mCubemapShader;
Shader ShaderPrograms::mModelShader;
Shader ShaderPrograms::mDepthModelShader;
Shader ShaderPrograms::mShadowDebugShader;

void ShaderPrograms::init()
{
	mLightingShader.initSource(LIGHTING_VERT, LIGHTING_FRAG);
	mLightingDepthShader.initSource(LIGHTING_DEPTH_VERT, LIGHTING_DEPTH_FRAG);
	mCubemapShader.initSource(CUBEMAP_VERT, CUBEMAP_FRAG);
	mModelShader.initSource(MODEL_VERT, MODEL_FRAG);
	mDepthModelShader.initSource(MODEL_DEPTH_VERT, MODEL_DEPTH_FRAG);
	mShadowDebugShader.initSource(SHADOW_DEBUG_VERT, SHADOW_DEBUG_FRAG);
}

Shader& ShaderPrograms::getLightingShader(bool shadowPass)
{
	if (shadowPass)
	{
		return mLightingDepthShader;
	}
	return mLightingShader;
}

Shader& ShaderPrograms::getCubemapShader()
{
	return mCubemapShader;
}


Shader& ShaderPrograms::getModelShader(bool shadowPass)
{
	if (shadowPass)
	{
		return mDepthModelShader;
	}
	return mModelShader;
}

Shader& ShaderPrograms::getShadowDebugShader()
{
	return mShadowDebugShader;
}
