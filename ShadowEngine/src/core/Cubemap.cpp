#include "Cubemap.h"
#include <GL/glew.h>
#include <SOIL/SOIL.h>
#include "graphics/ShaderPrograms.h"
#include <core/Camera.h>
#include <core/Log.h>

VertexArray Cubemap::mVertexArray;

Cubemap::Cubemap(Application& application)
	: Drawable(application)
{
	// lazy init
	if (mVertexArray.id() == 0)
	{
		float skyboxVertices[] = {
			// positions          
			-1.0f,  1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			-1.0f,  1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f
		};

		mVertexArray.fillVertexBuffer(sizeof(skyboxVertices), &skyboxVertices);

		// layout
		ShaderLayout skyboxVaoLayout;
		skyboxVaoLayout.push<float>(3); // position
		mVertexArray.vertexBufferLayout(skyboxVaoLayout);

		// shader
		mVertexArray.setDefaultShader(ShaderPrograms::getCubemapShader());
	}
}

void Cubemap::update()
{
}

void Cubemap::draw(bool shadowPass)
{
	if (mTexture == 0 && mCubemapFolder != "")
	{
		loadTextures();
	}

	if (mTexture != 0)
	{
		// draw
		Shader& shader = mVertexArray.getDefaultShader();
		shader.bind();
		glDepthFunc(GL_LEQUAL);
		glDepthMask(GL_FALSE);
		shader.addUniform("projection", Camera::getProjection());
		shader.addUniform("view", glm::mat4(glm::mat3(Camera::getView())));
		mVertexArray.bind();
		glBindTexture(GL_TEXTURE_CUBE_MAP, mTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glDepthMask(GL_TRUE);
	}
}

void Cubemap::loadTextures()
{
	glGenTextures(1, &mTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mTexture);

	// load texture faces
	std::vector<std::string> textureFaces
	{
		mCubemapFolder + "/right.jpg",
		mCubemapFolder + "/left.jpg",
		mCubemapFolder + "/top.jpg",
		mCubemapFolder + "/bottom.jpg",
		mCubemapFolder + "/front.jpg",
		mCubemapFolder + "/back.jpg"
	};

	// SOIL stuff
	int width, height, channels;
	unsigned char* data;
	for (int i = 0; i < 6; i++)
	{
		data = SOIL_load_image(textureFaces[i].c_str(), &width, &height, &channels, SOIL_LOAD_RGB);

		if (!data)
		{
			LOG_ERROR("Failed to load texture " + textureFaces[i], false);
		}

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_SRGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, (void*)data);
		SOIL_free_image_data(data);
	}

	// parameters
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}
