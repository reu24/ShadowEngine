#pragma once
#include <vector>
#include "core/Vector.h"

#define MAX_POINT_LIGHTS 5
#define MAX_DIR_LIGHTS 2

// A light which sends light in all directions
struct PointLight
{
	PointLight() = default;

	PointLight(vec3 position)
	{
		this->position = position;
	}

	PointLight& operator=(const PointLight& p)
	{
		position = p.position;
		constant = p.constant;
		quadratic = p.quadratic;
		ambient = p.ambient;
		diffuse = p.diffuse;
		specular = p.specular;
		return *this;
	}

	vec3 position;

	float constant = 1.0f;
	float linear = 0.09f;
	float quadratic = 0.032f;

	// The color if no light is shining at it
	vec3 ambient = vec3(0.05f, 0.05f, 0.05f);
	// The color if the lights is shining at it
	vec3 diffuse = vec3(0.8f, 0.8f, 0.8f);
	// The shiny reflection color
	vec3 specular = vec3(1.0f, 1.0f, 1.0f);
};

// A light which sends light in a specific angle but everywhere
struct DirLight
{
	DirLight() = default;

	DirLight(vec3 direction)
	{
		this->direction = direction;
	}

	DirLight& operator=(DirLight& d)
	{
		direction = d.direction;
		ambient = d.ambient;
		diffuse = d.diffuse;
		specular = d.specular;
		return *this;
	}

	// The color if no light is shining at it
	vec3 direction;
	vec3 ambient = vec3(0.02f, 0.02f, 0.02f);
	// The color if the lights is shining at it
	vec3 diffuse = vec3(0.6f, 0.6f, 0.6f);
	// The shiny reflection color
	vec3 specular = vec3(0.5f, 0.5f, 0.5f);
};

// The class to add light to the scene
class Light
{
public:
	static void addLight(PointLight& pointLight);
	static void addLight(DirLight& dirLight);
	static void removeLight(const PointLight& pointLight);
	static void removeLight(const DirLight dirLight);
	
	// INTERNAL DON'T USE!
	static void update();

	static const std::vector<PointLight*>& getPointLights();
	static const std::vector<DirLight*>& getDirLights();

private:
	static void updateRenderedPointLight();
	static int compare(const void* a, const void* b);

	static std::vector<PointLight*> mPointLights;
	static std::vector<DirLight*> mDirLights;
	static std::vector<PointLight*> mRenderedPointLights;
};