#pragma once
#include "Drawable.h"
#include <graphics/VertexArray.h>

/*
	Use this if you want to add a 3D background image to your game.
	Use this by setting the cubefolder to a folder with the following images:
	back.jpg, bottom.jpg, front.jpg, left.jpg, right.jpg, top.jpg
	Add it to the game using mApplication.getWorldLayer().add()
*/
class Cubemap : public Drawable
{
public:
	Cubemap(Application& application);

	void update() override;
	void draw(bool shadowPass) override;

	/*
		The folder where the images of the cubemap are
		The folder should contain:
		back.jpg, bottom.jpg, front.jpg, left.jpg, right.jpg, top.jpg
	*/
	void setCubemapFolder(const std::string& cubemapFolder)
	{
		mCubemapFolder = cubemapFolder;
	}

private:
	void loadTextures();

	std::string mCubemapFolder = "";

	unsigned int mTexture = 0;
	static VertexArray mVertexArray;

	typedef Cubemap Super;
};