#pragma once
#include "core/Vector.h"

// INTERNAL DON'T USE!
class Shadow
{
public:
	static void init();
	static void setupShadowDrawing();
	static void endShadowDrawing();
	static mat4 getLightSpaceMatrix();

private:
	static unsigned int mDepthmapFBO;
	static unsigned int mDepthmapTexture;
};