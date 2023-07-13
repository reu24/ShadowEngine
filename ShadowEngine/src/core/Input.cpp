#include "Input.h"
#include <GLFW/glfw3.h>
#include "Utils.h"
#include <core\Log.h>

// static vars
std::vector<int> Input::mJustPressedKeys;
std::vector<int> Input::mPressedKeys;
std::vector<int> Input::mRepeatedKeys;
std::vector<int> Input::mReleasedKeys;

std::vector<int> Input::mJustPressedMouse;
std::vector<int> Input::mPressedMouse;
std::vector<int> Input::mReleasedMouse;

double Input::mScrollOffset;
dvec2 Input::mCursorOffset;
dvec2 Input::mLastCursorPos;

std::vector < std::pair < KeyState, std::function<void(int key) > > > Input::mKeyListeners;
std::vector < std::pair < MouseState, std::function<void(int key)> > > Input::mMouseListeners;
std::vector < std::function<void(double scroll)> > Input::mScrollListeners;
std::vector < std::function<void(vec2 cursor)> > Input::mCursorListeners;

GLFWwindow* Input::mWindow;

void Input::init(GLFWwindow* window)
{
	// set callbacks
	glfwSetKeyCallback(window, keyCallback);
	glfwSetMouseButtonCallback(window, mouseCallback);
	glfwSetScrollCallback(window, scrollCallback);

	// reserve vector space
	mJustPressedKeys.reserve(6);
	mPressedKeys.reserve(6);
	mRepeatedKeys.reserve(6);
	mReleasedKeys.reserve(6);

	mJustPressedMouse.reserve(6);
	mPressedMouse.reserve(6);
	mReleasedMouse.reserve(6);

	mWindow = window;
}

void Input::update()
{
	// cursor
	dvec2 cursorPos = getCursorPos();
	mCursorOffset -= mLastCursorPos - cursorPos;
	mLastCursorPos = cursorPos;

	for (int i = 0; i < mCursorListeners.size(); i++)
	{
		mCursorListeners[i](mCursorOffset);
	}

	// scroll
	mScrollOffset = 0;

	// key
	mJustPressedKeys.clear();

	for (int i = 0; i < mKeyListeners.size(); i++)
	{
		if (mKeyListeners[i].first == KeyState::PRESSED)
		{
			for (int j = 0; j < mPressedKeys.size(); j++)
			{
				mKeyListeners[i].second(mPressedKeys[j]);
			}
		}
	}

	// mouse
	mJustPressedMouse.clear();

	for (int i = 0; i < mMouseListeners.size(); i++)
	{
		if (mMouseListeners[i].first == MouseState::PRESSED)
		{
			for (int j = 0; j < mPressedMouse.size(); j++)
			{
				mMouseListeners[i].second(mPressedMouse[j]);
			}
		}
	}
}

bool Input::key(KeyState state, int key)
{
	if (state == KeyState::JUST_PRESSED)
	{
		return containsElement(mJustPressedKeys, key);
	}
	if (state == KeyState::PRESSED)
	{
		return containsElement(mPressedKeys, key);
	}
	if (state == KeyState::REPEATED)
	{
		return containsElement(mReleasedKeys, key);
	}
	if (state == KeyState::RELEASED)
	{
		return containsElement(mRepeatedKeys, key);
	}
	if (state == KeyState::NOT_CLICKED)
	{
		return !containsElement(mPressedKeys, key);
	}
}

bool Input::mouse(MouseState state, int mouse)
{
	switch (state)
	{
	case MouseState::NOT_CLICKED:
		return !containsElement(mPressedMouse, mouse);
		break;
	case MouseState::JUST_PRESSED:
		return containsElement(mJustPressedMouse, mouse);
		break;
	case MouseState::PRESSED:
		return containsElement(mPressedMouse, mouse);
		break;
	case MouseState::RELEASED:
		return containsElement(mReleasedMouse, mouse);
		break;
	}
}

double Input::getScrollOffset()
{
	return mScrollOffset;
}

dvec2 Input::getCursorOffset()
{
	return mCursorOffset;
}

dvec2 Input::getCursorPos()
{
	dvec2 cursorPos;
	glfwGetCursorPos(mWindow, &cursorPos.x, &cursorPos.y);
	return cursorPos;
}

void Input::addKeyListener(KeyState state, std::function<void(int key)> listener)
{
	mKeyListeners.push_back(std::make_pair(state, listener));
}

void Input::removeKeyListener(std::function<void(int key)> listener)
{
	for (int i = 0; i < mKeyListeners.size(); i++)
	{
		if (&(mKeyListeners[i].second) == &listener)
		{
			mKeyListeners.erase(mKeyListeners.begin() + i);
		}
	}
}

void Input::addMouseListener(MouseState state, std::function<void(int key)> listener)
{
	mMouseListeners.push_back(std::make_pair(state, listener));
}

void Input::removeMouseListener(std::function<void(int key)> listener)
{
	for (int i = 0; i < mMouseListeners.size(); i++)
	{
		if (&(mMouseListeners[i].second) == &listener)
		{
			mMouseListeners.erase(mMouseListeners.begin() + i);
		}
	}
}

void Input::addScrollListener(std::function<void(double scroll)> listener)
{
	mScrollListeners.push_back(listener);
}

void Input::removeScrollListener(std::function<void(double scroll)> listener)
{
	for (int i = 0; i < mScrollListeners.size(); i++)
	{
		if (&mScrollListeners[i] == &listener)
		{
			mScrollListeners.erase(mScrollListeners.begin() + i);
		}
	}
}

void Input::addCursorListener(std::function<void(vec2 cursor)> listener)
{
	mCursorListeners.push_back(listener);
}

void Input::removeCursorListener(std::function<void(vec2 cursor)> listener)
{
	for (int i = 0; i < mCursorListeners.size(); i++)
	{
		if (&mCursorListeners[i] == &listener)
		{
			mCursorListeners.erase(mCursorListeners.begin() + i);
		}
	}
}

void Input::setCursorEnable(bool enable)
{
	if (enable)
	{
		glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	else
	{
		glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
}

std::string Input::getKeyName(int key)
{
	if (key == NO_KEY)
	{
		return "";
	}
	return glfwGetKeyName(key, NULL);
}

void Input::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	for (int i = 0; i < mKeyListeners.size(); i++)
	{
		switch (action)
		{
		case GLFW_PRESS:
			if (mKeyListeners[i].first == KeyState::JUST_PRESSED)
			{
				mKeyListeners[i].second(key);
			}
			break;
		case GLFW_REPEAT:
			if (mKeyListeners[i].first == KeyState::REPEATED)
			{
				mKeyListeners[i].second(key);
			}
			break;
		case GLFW_RELEASE:
			if (mKeyListeners[i].first == KeyState::RELEASED)
			{
				mKeyListeners[i].second(key);
			}
			break;
		default:
			break;
		}
	}

	switch (action)
	{
	case GLFW_PRESS:
		mJustPressedKeys.push_back(key);
		mPressedKeys.push_back(key);
		break;
	case GLFW_REPEAT:
		mRepeatedKeys.push_back(key);
		break;
	case GLFW_RELEASE:
		// remove from pressed keys
		for (int i = 0; i < mPressedKeys.size(); i++)
		{
			if (mPressedKeys[i] == key)
			{
				mPressedKeys.erase(mPressedKeys.begin() + i);
			}
		}
		// add to released
		mReleasedKeys.push_back(key);
		break;
	default:
		break;
	}
}

void Input::mouseCallback(GLFWwindow* window, int button, int action, int mods)
{
	for (int i = 0; i < mMouseListeners.size(); i++)
	{
		switch (action)
		{
		case GLFW_PRESS:
			if (mMouseListeners[i].first == MouseState::JUST_PRESSED)
			{
				mMouseListeners[i].second(button);
			}
			break;
		case GLFW_RELEASE:
			if (mMouseListeners[i].first == MouseState::RELEASED)
			{
				mMouseListeners[i].second(button);
			}
			break;
		default:
			break;
		}
	}

	switch (action)
	{
	case GLFW_PRESS:
		mJustPressedMouse.push_back(button);
		mPressedMouse.push_back(button);
		break;
	case GLFW_RELEASE:
		for (int i = 0; i < mPressedMouse.size(); i++)
		{
			if (mPressedMouse[i] == button)
			{
				mPressedMouse.erase(mPressedMouse.begin() + i);
			}
		}
		mReleasedMouse.push_back(button);
		break;
	default:
		break;
	}
}

void Input::scrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
	for (int i = 0; i < mScrollListeners.size(); i++)
	{
		mScrollListeners[i](yOffset);
	}

	mScrollOffset += yOffset;
}
