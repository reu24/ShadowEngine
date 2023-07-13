#pragma once
#include <vector>
#include "core/Updateable.h"
#include "Application.h"
#include <graphics\Light.h>

class UpdateableContainer
{
public:
	UpdateableContainer(Application& application)
		: mApplication(application)
	{
	}

	template<class T>
	T& createUpdateable()
	{
		T* updateable = new T(mApplication);
		mUpdateables.push_back(updateable);
		return *updateable;
	}

	template<class T>
	T& createLight(vec3 arg)
	{
		T* light = new T(arg);
		mUpdateables.push_back(light);
		return *light;
	}

	void removeUpdateable(const void* updateable)
	{
		for (int i = 0; i < mUpdateables.size(); i++)
		{
			if (&updateable == mUpdateables[i])
			{
				mUpdateables.erase(mUpdateables.begin() + i);
			}
		}
	}

	~UpdateableContainer()
	{
		for (int i = 0; i < mUpdateables.size(); i++)
		{
			delete mUpdateables[i];
		}
	}

private:
	Application& mApplication;
	std::vector<void*> mUpdateables;
};