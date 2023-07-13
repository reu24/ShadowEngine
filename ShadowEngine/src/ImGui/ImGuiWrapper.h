#pragma once

#define NO_WINDOW 794863
struct ImVec2;
struct ImFont;

// INTERNAL DON'T USE!
class ImGuiWrapper
{
public:
	static void init();
	static void newFrame();
	static void render();

	static ImFont* getFont()
	{
		return mFont;
	}

	static void startRotation();
	static void endRotation(float rotation);

private:
	static ImVec2 rotationCenter();

	static int mRotationStartIndex;
	static ImFont* mFont;
};


