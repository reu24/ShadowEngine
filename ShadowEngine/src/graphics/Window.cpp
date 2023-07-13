#include "Window.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "core/Log.h"
#include <core/Input.h>

GLFWwindow* Window::mWindow = nullptr;

void Window::init(vec2 size, std::string name, bool fullscreen)
{
	// create window
	if (glfwInit() == GLFW_FALSE)
	{
		LOG_FATAL("Failed to initialize GLFW!", true);
	}
	GLFWmonitor* monitor = (fullscreen ? glfwGetPrimaryMonitor() : nullptr);
	mWindow = glfwCreateWindow((int)size.x, (int)size.y, name.c_str(), monitor, nullptr);
	if (!mWindow)
	{
		LOG_FATAL("Failed to create Window!", true);
	}
	glfwMakeContextCurrent(mWindow);
	glfwSwapInterval(1);

	// set frame buffer callback
	glfwSetFramebufferSizeCallback(mWindow, frameBufferSizeCallback);
	glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// viewport
	glViewport(0, 0, (GLsizei) size.x, (GLsizei) size.y);

	Input::init(mWindow);
}

void Window::update()
{
	glfwPollEvents();
	glfwSwapBuffers(mWindow);
	Input::update();
}

bool Window::shouldClose()
{
	return glfwWindowShouldClose(mWindow);
}

void Window::setShouldClose(bool val)
{
	glfwSetWindowShouldClose(mWindow, val);
}

GLFWwindow* Window::glfw()
{
	return mWindow;
}

glm::ivec2 Window::getSize()
{
	int x, y;
	glfwGetWindowSize(mWindow, &x, &y);
	return ivec2(x, y);
}

void Window::setSize(const ivec2& size)
{
	glfwSetWindowSize(mWindow, size.x, size.y);
}

glm::ivec2 Window::getPos()
{
	int x, y;
	glfwGetWindowPos(mWindow, &x, &y);
	return ivec2(x, y);
}

void Window::setPos(const ivec2& pos)
{
	glfwSetWindowPos(mWindow, pos.x, pos.y);
}

void Window::frameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
