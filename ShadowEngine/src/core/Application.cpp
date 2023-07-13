#include "Application.h"
#include "core/Level.h"
#include <graphics/Window.h>
#include <core/Log.h>
#include "core/Input.h"
#include <graphics/Renderer.h>
#include <core/Camera.h>
#include <core/Physics.h>
#include <ImGui/ImGuiWrapper.h>

Application::Application(ivec2 windowSize, std::string windowName, bool fullscreen)
{
	mActiveLevel = createDefaultLevel(*this);
	Physics::setPhysicsWorld(mActiveLevel->physicsWorld);
	Window::init(windowSize, windowName, fullscreen);
	Light::update();
	Renderer::init();
	Camera::init();
	ImGuiWrapper::init();
	Shadow::init();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glDepthFunc(GL_LESS);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
}

Application::~Application()
{
	delete mActiveLevel;
}

RenderLayer& Application::getWorldLayer()
{
	return mActiveLevel->worldLayer;
}

RenderLayer& Application::getUILayer()
{
	return mActiveLevel->UILayer;
}

RenderLayer& Application::getDebugLayer()
{
	return mActiveLevel->debugLayer;
}

void Application::logFPS()
{
	if (mLogFPS)
	{
		static double lastTime = glfwGetTime();

		double thisTime = glfwGetTime();
		double deltaTime = thisTime - lastTime;
		double fps = 1 / deltaTime;
		LOG_INFO("FPS: " + std::to_string(fps), false);
		lastTime = glfwGetTime();
	}
}

void Application::start()
{
	mActiveLevel->loadLevel();
	while (!Window::shouldClose())
	{
		logFPS();
		if (mEnableImGui) ImGuiWrapper::newFrame();
		Physics::update();
		Light::update();
		mActiveLevel->update();
		update();
		mActiveLevel->draw();
		if (mEnableImGui) ImGuiWrapper::render();
		Window::update();
	}
}

void Application::changeLevel(Level* level)
{
	delete mActiveLevel;
	mActiveLevel = level;
	Physics::setPhysicsWorld(mActiveLevel->physicsWorld);
}

void Application::setLogFPS(bool logFPS)
{
	mLogFPS = logFPS;
}

void Application::setEnableImGui(bool enableImGui)
{
	mEnableImGui = enableImGui;
}
