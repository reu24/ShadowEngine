#include "TestApplication.h"
#include <core/Log.h>
#include <core/Input.h>
#include <core/Camera.h>
#include <graphics/Light.h>
#include <core/Cube.h>
#include <core/Cubemap.h>
#include <core/Model.h>
#include <GLFW/glfw3.h>
#include <core/Physics.h>
#include <ui/Text.h>

TestApplication::TestApplication()
	: Super(ivec2(1080, 720), "Test", false)
{
	setLogFPS(true);
	start();
}

void TestApplication::update()
{
}
