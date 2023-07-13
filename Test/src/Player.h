#pragma once
#include <core/Cube.h>
#include <core/Model.h>

class Player : public Cube
{
public:
	Player(Application& application);
	void setTestModel(Model* testModel);
	void update();

private:
	Model* mTestModel = nullptr;
	bool mDetached = false;

	const float MOVEMENT_FORCE = 200.0f;
};