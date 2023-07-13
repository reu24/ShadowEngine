#pragma once
#include "Texture2D.h"

// The properties of a object's material
struct LightingMaterial
{
	LightingMaterial() = default;

	LightingMaterial& operator=(const LightingMaterial& l)
	{
		color = l.color;
		texture = l.texture;
		usesTexture = l.usesTexture;
		shininess = l.shininess;
		specularMap = l.specularMap;
		usesSpecularMap = l.usesSpecularMap;
		return *this;
	}

	vec4 color = vec4(1.0f);

	Texture2D* texture = nullptr;
	bool usesTexture = false;

	float shininess = 32.0f;

	Texture2D* specularMap = nullptr;
	bool usesSpecularMap = false;
};