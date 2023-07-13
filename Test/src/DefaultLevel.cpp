#include "DefaultLevel.h"
#include <graphics/Light.h>
#include <core/Cube.h>
#include <core/Cubemap.h>
#include <core/Model.h>
#include "Player.h"

void DefaultLevel::loadLevel()
{
	DirLight& dirLight = mUpdateableContainer.createLight<DirLight>(vec3(0.4f, -0.7f, 0.5f));
	Light::addLight(dirLight);
	

	Light::addLight(mUpdateableContainer.createLight<PointLight>(vec3(5.0f, 2.0f, 0.0f)));
	Light::addLight(mUpdateableContainer.createLight<PointLight>(vec3(0.0f, 3.0f, 1.0f)));
	Light::addLight(mUpdateableContainer.createLight<PointLight>(vec3(7.0f, 5.0f, 0.0f)));
	Light::addLight(mUpdateableContainer.createLight<PointLight>(vec3(3.0f, 1.0f, 0.0f)));
	Light::addLight(mUpdateableContainer.createLight<PointLight>(vec3(2.0f, 0.0f, 0.0f)));
	Light::addLight(mUpdateableContainer.createLight<PointLight>(vec3(10.0f, 5.0f, 0.0f)));

	Cube& platform = mUpdateableContainer.createUpdateable<Cube>();
	platform.setPos(vec3(0.0f, -1.0f, 0.0f));
	platform.setScale(vec3(100.0f, 1.0f, 100.0f));
	platform.getPhysicalBody().setType(BodyType::STATIC);
	mApplication.getWorldLayer().add(platform);

	Cube& testCube = mUpdateableContainer.createUpdateable<Cube>();
	testCube.setPos(vec3(2.0f, 10.0, 0.0f));
	testCube.getPhysicalBody().setLinearVelocity(vec3(0.0f, -1.0f, 0.0f));
	testCube.setRotation(vec3(1, 5, 3));
	mApplication.getWorldLayer().add(testCube);

	Cube& testCube2 = mUpdateableContainer.createUpdateable<Cube>();
	testCube2.setPos(vec3(2.0f, 0.0, 0.0f));
	testCube2.setRotation(vec3(1, 5, 3));
	mApplication.getWorldLayer().add(testCube2);

	Cube& testCube3 = mUpdateableContainer.createUpdateable<Cube>();
	testCube3.setPos(vec3(20.0f, 0.0, 0.0f));
	mApplication.getWorldLayer().add(testCube3);

	Cubemap& testCubeMap = mUpdateableContainer.createUpdateable<Cubemap>();
	testCubeMap.setCubemapFolder("res/cubemap");
	mApplication.getWorldLayer().add(testCubeMap);

	Model& testModel = mUpdateableContainer.createUpdateable<Model>();
	testModel.setModelPath("res/models/man/model.dae");
	testModel.setPos(vec3(1.0f, -1.1f, -2.0f));
	testModel.setRotation(vec3(-90.0f, 0.0f, 0.0f));
	testModel.getPhysicalBody().setType(BodyType::STATIC);
	mApplication.getWorldLayer().add(testModel);

	Model& testModel2 = mUpdateableContainer.createUpdateable<Model>();
	testModel2.setModelPath("res/models/astroboy/astroBoy_walk_Max.dae");
	testModel2.setPos(vec3(-1.0f, -1.1f, 2.0f));
	testModel2.setRotation(vec3(-90.0f, 0.0f, 0.0f));
	testModel2.getPhysicalBody().setType(BodyType::STATIC);
	mApplication.getWorldLayer().add(testModel2);

	Player& player = mUpdateableContainer.createUpdateable<Player>();
	player.setTestModel(&testModel);
	mApplication.getWorldLayer().add(player);
}