#include "Text.h"
#include <ImGui/imgui.h>
#include "ImGui/ImGuiWrapper.h"
#include "graphics/Window.h"

Text::Text(Application& application)
	: Drawable(application)
{
}

void Text::setText(const std::string& text)
{
	mText = text;
}

void Text::setTextColor(const vec3& textColor)
{
	mTextColor = textColor;
}

void Text::draw(bool shadowPass)
{	
	ImGui::SetNextWindowSize(ImVec2(Window::getSize().x, Window::getSize().y));
	ImGui::SetNextWindowPos(ImVec2(Window::getSize().x * mPos.x, Window::getSize().y * mPos.y));

	ImFont* font = ImGuiWrapper::getFont();
	ImGui::PushFont(font);
	font->Scale = mScale.x;

	ImGui::Begin(" ", 0, NO_WINDOW);

	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(mTextColor.r, mTextColor.g, mTextColor.b, 1.0f));
	ImGuiWrapper::startRotation();
	ImGui::Text(mText.c_str());
	ImGui::PopFont();
	ImGuiWrapper::endRotation(mRotation.x);
	ImGui::PopStyleColor();

	ImGui::End();
}
