#include "RenderLayer.h"
#include "Drawable.h"
#include "Utils.h"
#include "UpdateableContainer.h"

constexpr int NUM_THREADS = 16;

RenderLayer::RenderLayer(int priority)
	: mPriority(priority)
{
}

void RenderLayer::add(Updateable& updateable)
{
	updateable.added();
	mUpdateables.push_back(&updateable);
}

void RenderLayer::removeUpdateable(Updateable& updateable)
{
	for (int i = 0; i < mUpdateables.size(); i++)
	{
		if (mUpdateables[i] == &updateable)
		{
			mUpdateables.erase(mUpdateables.begin() + i);
			mUpdateables[i]->removed();
		}
	}
}

const std::vector<Updateable*>& RenderLayer::getUpdateables()
{
	return mUpdateables;
}

void RenderLayer::update()
{
	for (int i = 0; i < mUpdateables.size(); i++)
	{
		mUpdateables[i]->update();
	}
}

void RenderLayer::draw(bool shadowPass)
{
	for (int i = 0; i < mUpdateables.size(); i++)
	{
		if (Drawable* drawable = dynamic_cast<Drawable*>(mUpdateables[i]))
			drawable->draw(shadowPass);
	}
}

int RenderLayer::getPriority()
{
	return mPriority;
}