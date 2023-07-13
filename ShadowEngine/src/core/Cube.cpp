#include "Cube.h"
#include "graphics/Renderer.h"
#include "Utils.h"
#include "graphics/ShaderPrograms.h"
#include <core/Log.h>
#include <core/Physics.h>
#include "Camera.h"

VertexArray Cube::mVertexArray;

Cube::Cube(Application& application)
	: Object3D::Object3D(application)
{
	// lazy init
	if (mVertexArray.id() == 0)
	{
		float cubeVertices[] = {
			// positions           // normals           // texture coords
			 0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   1.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   1.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f,

			 0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   0.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   0.0f, 0.0f,

			 0.5f,  0.5f, -0.5f,   1.0f,  0.0f,  0.0f,   1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,   1.0f,  0.0f,  0.0f,   1.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,   1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  0.0f,   0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,   1.0f,  0.0f,  0.0f,   1.0f, 0.0f,

			-0.5f,  0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,   1.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,   1.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,   0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,   1.0f, 0.0f,

			 0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,   0.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,   1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,   1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,   1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,   0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,   0.0f, 1.0f,

			 0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,   0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,   1.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,   1.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,   1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,   0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,   0.0f, 1.0f
		};

		mVertexArray.fillVertexBuffer(sizeof(cubeVertices), cubeVertices);
		mVertexArray.setVertexCount(36);

		ShaderLayout layout;
		layout.push<float>(3);
		layout.push<float>(3);
		layout.push<float>(2);
		mVertexArray.vertexBufferLayout(layout);
	}
}

void Cube::update()
{
	mPos = mPhysicalBody.getPos();
	mRotation = mPhysicalBody.getRotation();
}

void Cube::draw(bool shadowPass)
{
	ShaderPrograms::getLightingShader(shadowPass).addUniform("test", true);
	if (mVisible)
	{
		mat4 model = createModel(mPos, mScale, mRotation);
		mVertexArray.setDefaultShader(ShaderPrograms::getLightingShader(shadowPass));
		Renderer::draw(mVertexArray, mLightingMaterial, model, shadowPass);
	}
}

void Cube::added()
{
	Object3D::added();
	rp3d::BoxShape* cubeShape = new rp3d::BoxShape(toReact(mScale / 2.0f));
	mProxyShape = mPhysicalBody.addCollisionShape(cubeShape, rp3d::Transform(), 10.0);
}

void Cube::removed()
{
	Object3D::removed();
	mPhysicalBody.removeCollisionShape(mProxyShape);
}

void Cube::setScale(const vec3& scale)
{
	Object3D::setScale(scale);
	if (mAdded)
	{
		// replace boxshape with a new one
		mPhysicalBody.removeCollisionShape(mProxyShape);
		rp3d::CollisionShape* boxShape = mPhysicalBody.getCollisionShapes()[0];
		delete boxShape;
		rp3d::BoxShape* cubeShape = new rp3d::BoxShape(toReact(mScale / 2.0f));
		mProxyShape = mPhysicalBody.addCollisionShape(cubeShape, rp3d::Transform(), 10.0);
	}
}