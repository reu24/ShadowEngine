#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <graphics/Window.h>
#include "Texture2D.h"
#include <graphics/Shader.h>
#include <vector>
#include "Light.h"
#include "LightingMaterial.h"
#include <graphics/VertexArray.h>

/*
	INTERNAL DON'T USE!
	Draws objects in the 3d world
*/
class Renderer
{
public:
	static void init();

	static void draw(VertexArray& vertexArray, LightingMaterial material, const glm::mat4& model, bool shadowPass);

private:
	static void setupUniforms(Shader& shader, vec3 cameraPos, std::vector<DirLight*> dirLights, std::vector<PointLight*> pointLights, LightingMaterial material);
};

// INTERNAL DON'T USE!
static void setupDrawing()
{
	ivec2 size = Window::getSize();
	glViewport(0, 0, size.x, size.y);
	glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}