#pragma once
#include "core/Drawable.h"

class Text : public Drawable
{
public:
	Text(Application& application);

	void setText(const std::string& text);
	void setTextColor(const vec3& textColor);

	void update() { }
	void draw(bool shadowPass) override;

private:
	std::string mText = "";
	vec3 mTextColor = vec3(0.0f);
};