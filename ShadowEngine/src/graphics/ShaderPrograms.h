#pragma once
#include "Shader.h"

// INTERNAL DON?T USE!
class ShaderPrograms
{
public:
	static void init();

	static Shader& getLightingShader(bool shadowPass);
	static Shader& getCubemapShader();
	static Shader& getModelShader(bool shadowPass);
	static Shader& getShadowDebugShader();

private:
	static Shader mLightingShader;
	static Shader mLightingDepthShader;
	static Shader mCubemapShader;
	static Shader mModelShader;
	static Shader mDepthModelShader;
	static Shader mShadowDebugShader;
};