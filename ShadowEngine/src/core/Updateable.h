#pragma once
#include "Application.h"
#include "UpdateableContainer.h"

class Updateable
{
public:
	Updateable(Application& application)
		: mApplication(application)
	{
	}

	virtual void update() = 0;

	virtual void added() 
	{
		mAdded = true;
	}
	virtual void removed() 
	{
		mAdded = false;
	}

private:
	std::string mName;

protected:
	Application& mApplication;
	bool mAdded = false;
	UpdateableContainer mUpdateableContainer = UpdateableContainer(mApplication);
};