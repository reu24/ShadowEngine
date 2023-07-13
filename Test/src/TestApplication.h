#pragma once
#include "DefaultLevel.h"
#include <main.h>
#include <core/UpdateableContainer.h>
#include <core/PhysicalBody.h>
#include <core/Vector.h>

class TestApplication : public Application
{
public:
	TestApplication();

	virtual void update() override;

private:
};

Application* createApplication()
{
	return new TestApplication();
}

Level* createDefaultLevel(Application& app)
{
	return new DefaultLevel(app);
}

ivec2 getWindowSize()
{
	return ivec2(1080, 720);
}