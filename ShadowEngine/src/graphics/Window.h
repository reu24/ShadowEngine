#pragma once
#include "core/Vector.h"
#include <string>

struct GLFWwindow;

// Window stuff
class Window
{
public:
	static void init(vec2 size, std::string name, bool fullscreen);
	static void update();

	// if the game should stop
	static bool shouldClose();
	// if the game should stop
	static void setShouldClose(bool val);
	/*
		INTERNAL DON'T USE!
		The window of GLFW.
	*/
	static GLFWwindow* glfw();
	// the size of the window
	static glm::ivec2 getSize();
	// the size of the window
	static void setSize(const ivec2& size);
	// the position of the window
	static glm::ivec2 getPos();
	// the position of the window
	static void setPos(const ivec2& pos);

private:
	static GLFWwindow* mWindow;

	static void frameBufferSizeCallback(GLFWwindow* window, int width, int height);
};