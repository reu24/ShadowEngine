#pragma once
#include "KeyCodes.h"
#include "Vector.h"
#include <string>
#include <functional>

enum class KeyState
{
	// Key isn't clicked by the user
	NOT_CLICKED,
	// Key was just pressed by the user in this tick
	JUST_PRESSED,
	// Key is currently down
	PRESSED,
	// Key is clicked for some time (for writing text)
	REPEATED,
	// Key got just released by the user in this tick
	RELEASED
};

enum class MouseState
{
	// Mouse button not down
	NOT_CLICKED,
	// Mouse button was just pressed by the user in this tick
	JUST_PRESSED,
	// Mouse button is down
	PRESSED,
	// Mouse button was just released by the user in this tick
	RELEASED
};

struct GLFWwindow;

// Used for any kind of user input
class Input
{
public:
	// INTERNAL! DON'T USE!
	static void init(GLFWwindow* window);
	// INTERNAL! DON'T USE!
	static void update();

	/*
		Tests if the specified key is in the specified state. 
		Example: Input::key(KeyState::PRESSED, KEY_W)
		@param state: The state of the key
		@param key: The key that should be checked. Start with "KEY_" and then name.
		@return Returns true if the key is in that state and false if it isn't
	*/
	static bool key(KeyState state, int key);
	/*
		Tests if the specified mouse button is in the specified state.
		Example: Input::mouse(MouseState::PRESSED, MOUSE_BUTTON_LEFT)
		@param state: The state of the mouse button
		@param key: The button that should be checked. Start with "MOUSE_BUTTON_" and then name.
		@return Returns true if the button is in that state and false if it isn't
	*/
	static bool mouse(MouseState state, int key);

	// Returns how much the scroll wheel changed.
	static double getScrollOffset();
	// Returns how much the cursor position changed.
	static dvec2 getCursorOffset();
	/*
		Returns the position of the cursor. 
		Note: Only use this if the cursor is enabled. See Input::setCursorEnable
	*/
	static dvec2 getCursorPos();

	/*
		Adds a listener to a key state.
		@param state: The state that a key has to be in order to trigger.
		@param listener: The listener that gets called when a key gets in that state. 
		The function should be: void listener(int key). You can check which key it is using
		for example: if (key == KEY_W).
	*/
	static void addKeyListener(KeyState state, std::function<void(int key)> listener);
	// Removes a key listener that was added using Input::addKeyListener.
	static void removeKeyListener(std::function<void(int key)> listener);
	/*
		Adds a listener to a mouse button state.
		@param state: The state that a mouse button has to be in order to trigger.
		@param listener: The listener that gets called when a mouse button gets in that state.
		The function should be: void listener(int key). You can check which mouse button it is using
		for example: if (key == MOUSE_BUTTON_LEFT).
	*/
	static void addMouseListener(MouseState state, std::function<void(int key)> listener);
	// Removes a key listener that was added using Input::addMouseListener.
	static void removeMouseListener(std::function<void( int key)> listener);
	/*
		Adds a listener to when the user scrolls.
		@param listener: The function that get's called when the user scrolls. 
		The function should be: void listener(double scroll). 
		scroll is how strong the user scrolled.
	*/
	static void addScrollListener(std::function<void(double scroll)> listener);
	// Removes a scroll listener that was added using Input::addScrollListener.
	static void removeScrollListener(std::function<void(double scroll)> listener);
	/*
		Adds a listener to when the user changes the cursor position.
		@param listener: The function that get's called when the cursor position changes.
		The function should be: void listener(vec2 cursor).
		cursorOffset is how stong the cursor changed.
	*/
	static void addCursorListener(std::function<void(vec2 cursorOffset)> listener);
	// Removes a cursor listener that was added using Input::addCursorListener.
	static void removeCursorListener(std::function<void(vec2 cursorOffset)> listener);

	// Changes wheather the cursor should be visible and move or not
	static void setCursorEnable(bool enable);

	/*
		Returns the name of the key. 
		Example: Input::getKeyName(KEY_W)
		The methode above returns W.
	*/
	static std::string getKeyName(int key);

private:
	static std::vector<int> mJustPressedKeys;
	static std::vector<int> mPressedKeys;
	static std::vector<int> mRepeatedKeys;
	static std::vector<int> mReleasedKeys;

	static std::vector<int> mJustPressedMouse;
	static std::vector<int> mPressedMouse;
	static std::vector<int> mReleasedMouse;

	static double mScrollOffset;
	static dvec2 mCursorOffset;
	static dvec2 mLastCursorPos;

	static std::vector < std::pair < KeyState, std::function<void(int key) > > > mKeyListeners;
	static std::vector < std::pair < MouseState, std::function<void(int key)> > > mMouseListeners;
	static std::vector < std::function<void(double scroll)> > mScrollListeners;
	static std::vector < std::function<void(vec2 cursor)> > mCursorListeners;

	static GLFWwindow* mWindow;

	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouseCallback(GLFWwindow* window, int button, int action, int mods);
	static void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);
};