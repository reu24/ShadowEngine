#pragma once
#include "Object3D.h"
#include <graphics/VertexArray.h>

/*
	A 3D Cube that can be rendered.
	Add it to the game using mApplication.getWorldLayer().add()
*/
class Cube : public Object3D
{
public:
	Cube(Application& application);

	void update() override;
	void draw(bool shadowPass) override;

	void added() override;
	void removed() override;

	void setScale(const vec3& scale) override;

private:
	rp3d::ProxyShape* mProxyShape = nullptr;
	static VertexArray mVertexArray;

};