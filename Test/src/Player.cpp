#include "Player.h"
#include <core/Camera.h>
#include <core/Input.h>
#include <core\Log.h>

Player::Player(Application& application)
	: Cube(application)
{
	setupFirstPersonCamera(0.1f);
}

void Player::setTestModel(Model* testModel)
{
	mTestModel = testModel;
}

void Player::update()
{
	Cube::update();

	static bool firstTime = true;
	if (firstTime)
	{
		firstTime = false;
		Input::addKeyListener(KeyState::PRESSED, [&](int key)
			{
				if (key == KEY_E)
				{
					if (!mTestModel->isAnimationPlaying())
					{
						mTestModel->playAnimation(0, false);
					}
				}

				if (key == KEY_F)
				{
					Input::setCursorEnable(true);
					Camera::setLocked(true);
				}

				if (key == KEY_R)
				{
					mTestModel->stopAnimation();
				}

				if (key == KEY_T)
				{
					mTestModel->resetAnimation();
				}

				if (key == KEY_W)
				{
					mPhysicalBody.applyForceCenter(Camera::getForwardsVector() * MOVEMENT_FORCE);
					if (mDetached)
					{
						Camera::changePos(Camera::getForwardsVector() * 0.1f);
					}
				}
				if (key == KEY_S)
				{
					mPhysicalBody.applyForceCenter(Camera::getForwardsVector() * -MOVEMENT_FORCE);
					if (mDetached)
					{
						Camera::changePos(Camera::getForwardsVector() * -0.1f);
					}
				}
				if (key == KEY_A)
				{
					mPhysicalBody.applyForceCenter(Camera::getRightVector() * -MOVEMENT_FORCE);
					if (mDetached)
					{
						Camera::changePos(Camera::getRightVector() * -0.1f);
					}
				}
				if (key == KEY_D)
				{
					mPhysicalBody.applyForceCenter(Camera::getRightVector() * MOVEMENT_FORCE);
					if (mDetached)
					{
						Camera::changePos(Camera::getRightVector() * 0.1f);
					}
				}
				if (key == KEY_H)
				{
					mDetached = true;
				}
				if (key == KEY_ESCAPE)
				{
					Input::setCursorEnable(true);
				}
			});
	}

	if (!mDetached) 
	{
		vec3 cubePos = mPos;
		cubePos.y += 1.5;
		Camera::setPos(cubePos);
	}
}
