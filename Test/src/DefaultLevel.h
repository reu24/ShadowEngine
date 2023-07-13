#pragma once
#include <core/Level.h>

class DefaultLevel : public Level
{
public:
	using Level::Level;

	virtual void loadLevel() override;
};