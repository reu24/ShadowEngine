#include "Light.h"
#include "core/Log.h"
#include "core/Camera.h"

std::vector<PointLight*> Light::mPointLights;
std::vector<DirLight*> Light::mDirLights;
std::vector<PointLight*> Light::mRenderedPointLights = std::vector<PointLight*>(MAX_POINT_LIGHTS);

void Light::addLight(PointLight& pointLight)
{
	mPointLights.push_back(&pointLight);
	if (mPointLights.size() > MAX_POINT_LIGHTS)
	{
		LOG_INFO("Max PointLights reached. Ignoring the most distant lights. MAX_POINT_LIGHTS: " + MAX_POINT_LIGHTS, false);
	}
}

void Light::addLight(DirLight& dirLight)
{
	if (mDirLights.size() >= MAX_DIR_LIGHTS)
	{
		LOG_WARNING("Max DirLights reached! MAX_DIR_LIGHTS:" + MAX_DIR_LIGHTS, false);
		return;
	}
	mDirLights.push_back(&dirLight);
}

void Light::removeLight(const PointLight& pointLight)
{
	for (int i = 0; i < mPointLights.size(); i++)
	{
		if ((mPointLights[i]) == &pointLight)
		{
			mPointLights.erase(mPointLights.begin() + i);
		}
	}
}

void Light::removeLight(const DirLight dirLight)
{
	for (int i = 0; i < mDirLights.size(); i++)
	{
		if ((mDirLights[i]) == &dirLight)
		{
			mDirLights.erase(mDirLights.begin() + i);
		}
	}
}

void Light::update()
{
	if (mPointLights.size() > MAX_POINT_LIGHTS)
	{
		updateRenderedPointLight();
	}
}

const std::vector<PointLight*>& Light::getPointLights()
{
	return mPointLights.size() <= MAX_POINT_LIGHTS ? mPointLights : mRenderedPointLights;
}

const std::vector<DirLight*>& Light::getDirLights()
{
	return mDirLights;
}

void Light::updateRenderedPointLight()
{
	// TODO Max Lights
	/*std::qsort(mPointLights.front(), mPointLights.size(), sizeof(PointLight), Light::compare);
	for (int i = 0; i < MAX_POINT_LIGHTS; i++)
	{
		mRenderedPointLights[i] = mPointLights[i];
	}*/
	mRenderedPointLights = mPointLights;
}

int Light::compare(const void* a, const void* b)
{
	float val1 = length(abs(((PointLight*)a)->position - Camera::getPos()));
	float val2 = length(abs(((PointLight*)b)->position - Camera::getPos()));
	float diff = val1 - val2;
	return (0 < diff) - (diff < 0);
}