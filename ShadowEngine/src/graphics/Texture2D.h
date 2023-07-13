#pragma once
#include <string>
#include <glm/glm.hpp>

// INTERNAL DON'T USE!
class Texture2D
{
public:
	Texture2D();
	Texture2D(const std::string& path, bool pixelArtStyle = true);
	Texture2D(unsigned int id);
	~Texture2D();

	void init(const std::string& path, bool pixelArtStyle = true);
	void bind();

	unsigned int id() const;
	glm::vec2 getSize() const;

private:
	unsigned int mTexture = 0;
	int mWidth = 0, mHeight = 0;
};