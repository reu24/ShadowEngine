#include "ImGuiWrapper.h"
#include <ImGui/imgui_impl_glfw.h>
#include <graphics/Window.h>
#include <ImGui/imgui_impl_opengl3.h>
#include <core/Log.h>
#include "ImGui/imgui_internal.h"

int ImGuiWrapper::mRotationStartIndex = -1;
ImFont* ImGuiWrapper::mFont = nullptr;

void ImGuiWrapper::init()
{
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(Window::glfw(), true);
	ImGui_ImplOpenGL3_Init("#version 330 core");

	ImGuiIO& io = ImGui::GetIO();
	mFont = io.Fonts->AddFontFromFileTTF("res/open-sans/OpenSans-Regular.ttf", 200);
	mFont->Scale = 0.2;
}

void ImGuiWrapper::newFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	//ImGui::PushFont(mFont);
}

void ImGuiWrapper::render()
{
	//ImGui::PopFont();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


void ImGuiWrapper::startRotation()
{
	mRotationStartIndex = ImGui::GetWindowDrawList()->VtxBuffer.Size;
}

ImVec2 operator-(const ImVec2& l, const ImVec2& r)
{
	return{l.x - r.x, l.y - r.y};
}

void ImGuiWrapper::endRotation(float rotation)
{
	if (mRotationStartIndex == -1)
	{
		LOG_ERROR("Can't rotate. Did you forget to call ImGuiRotation::startRotation()?", true);
	}

	float rad = radians(rotation + 90.0f);

	ImVec2 center = ImGuiWrapper::rotationCenter();

	float s = sin(rad), c = cos(rad);
	center = ImRotate(center, s, c) - center;

	auto& buf = ImGui::GetWindowDrawList()->VtxBuffer;
	for (int i = mRotationStartIndex; i < buf.Size; i++)
		buf[i].pos = ImRotate(buf[i].pos, s, c) - center;
}

ImVec2 ImGuiWrapper::rotationCenter()
{
	ImVec2 l(FLT_MAX, FLT_MAX), u(-FLT_MAX, -FLT_MAX);

	const auto& buf = ImGui::GetWindowDrawList()->VtxBuffer;
	for (int i = mRotationStartIndex; i < buf.Size; i++)
		l = ImMin(l, buf[i].pos), u = ImMax(u, buf[i].pos);

	return ImVec2((l.x + u.x) / 2, (l.y + u.y) / 2);
}
