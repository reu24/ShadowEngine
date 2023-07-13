#pragma once
#include <vector>

class Updateable;

class RenderLayer
{
public:
	RenderLayer(int priority);
	void add(Updateable& updateable);
	void removeUpdateable(Updateable& updateable);
	const std::vector<Updateable*>& getUpdateables();
	
	template<class T>
	std::vector<T*> getAll()
	{
		std::vector<T*> objects;
		for (int i = 0; i < mUpdateables.size(); i++)
		{
			Updateable* drawable = mUpdateables[i];
			if (T* t = dynamic_cast<T*>(drawable))
			{
				objects.push_back(t);
			}
		}
		return objects;
	}

	void update();
	void draw(bool shadowPass = false);
	int getPriority();
protected:
	std::vector<Updateable*> mUpdateables;
	int mPriority;
};
