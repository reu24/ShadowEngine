#pragma once
#include "core/RenderLayer.h"
#include "core/Vector.h"
#include <string>

int main();

class Level;

// Handles the whole applicaiton
class Application
{
public:
	// initializes everything
	Application(ivec2 windowSize, std::string windowName, bool fullscreen);
	~Application();

	void start();
	// gets called every tick
	virtual void update() = 0;
	void changeLevel(Level* level);

	void setLogFPS(bool logFPS);
	// enables ImGui. This may affect the performance.
	void setEnableImGui(bool enableImGui);

	/*
		The layer where every 3D object should be in.
		Every objects inside a RenderLayer get's updated and drawn.
		Use mApplication.getWorldLayer().add() to add 3D objects.
	*/
	RenderLayer& getWorldLayer();
	/*
		The layer where every UI Element should be in.
		Every objects inside a RenderLayer get's updated and drawn.
		Use mApplication.getUILayer().add() to add 3D objects.
	*/
	RenderLayer& getUILayer();
	/*
		The layer where debug text should be in.
		Every objects inside a RenderLayer get's updated and drawn.
		Use mApplication.getDebugLayer().add() to add 3D objects.
	*/
	RenderLayer& getDebugLayer();
	

protected:
	Level* mActiveLevel = nullptr;
	bool mLogFPS = false;
	bool mEnableImGui = false;

	void logFPS();

	typedef Application Super;
};

Application* createApplication();
Level* createDefaultLevel(Application& app);
ivec2 getWindowSize();