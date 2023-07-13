#pragma once
#include "core/RenderLayer.h"
#include "Drawable.h"
#include "graphics/Shadow.h"
#include "graphics/Renderer.h"
#include <reactphysics3d.h>
#include <graphics\ShaderPrograms.h>

/*
	The place where every object of the current world/scene/level is.
	This class contains the layers and the layers contains the objects.
*/
class Level
{
public:
	Level(Application& application)
		: mApplication(application), mUpdateableContainer(application), worldLayer(0), UILayer(1), debugLayer(2), physicsWorld(rp3d::Vector3(0.0, -9.81, 0.0))
	{
	};

	// INTERNAL! DON'T USE!
	void update()
	{
		worldLayer.update();
		UILayer.update();
		debugLayer.update();
	}

	unsigned int quadVAO = 0;
	unsigned int quadVBO;
	void renderQuad()
	{
		if (quadVAO == 0)
		{
			float quadVertices[] = {
				// positions        // texture Coords
				-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
				-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
				 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
				 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
			};
			// setup plane VAO
			glGenVertexArrays(1, &quadVAO);
			glGenBuffers(1, &quadVBO);
			glBindVertexArray(quadVAO);
			glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		}
		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);
	}

	// INTERNAL! DON'T USE!
	void draw()
	{
		if (Light::getDirLights().size() != 0)
		{
			Shadow::setupShadowDrawing();
			worldLayer.draw(true);
			Shadow::endShadowDrawing();
		}
	
		/*float near_plane = 1.0f, far_plane = 7.5f;
		setupDrawing();
		ShaderPrograms::getShadowDebugShader().bind();
		ShaderPrograms::getShadowDebugShader().addUniform("near_plane", near_plane);
		ShaderPrograms::getShadowDebugShader().addUniform("far_plane", far_plane);
		ShaderPrograms::getShadowDebugShader().addTexture(3, "depthMap");
		renderQuad();*/

		setupDrawing();
		worldLayer.draw();
		UILayer.draw();
		debugLayer.draw();
	}

	/*
		The layer where every 3D object should be in.
		Every objects inside a RenderLayer get's updated and drawn.
	*/
	RenderLayer worldLayer;
	/*
		The layer where every UI Element should be in.
		Every objects inside a RenderLayer get's updated and drawn.
	*/
	RenderLayer UILayer;
	/*
		The layer where debug text should be in.
		Every objects inside a RenderLayer get's updated and drawn.
	*/
	RenderLayer debugLayer;

	/*
		INTERNAL! DON'T USE!
		The React Physics 3D World
	*/
	rp3d::DynamicsWorld physicsWorld;

	//This methode gets called every time this level gets loaded.
	virtual void loadLevel() = 0;

protected:
	Application& mApplication;
	UpdateableContainer mUpdateableContainer;
};