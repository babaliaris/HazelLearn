#include "pch.h"
#include "ImGuiGLFWLayer.h"
#include <imgui.h>
#include <core/logger.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>




Amen::ImGuiGLFWLayer::ImGuiGLFWLayer()
{
}





Amen::ImGuiGLFWLayer::~ImGuiGLFWLayer()
{
	AMEN_INFO("Calling ~ImGuiGLFWLayer()");
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}





void Amen::ImGuiGLFWLayer::OnUpdate()
{
	//Start the frames.
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	//Drawing goes here.
	ImGui::ShowDemoWindow(&m_showDemoWindow);

	//Render.
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}





void Amen::ImGuiGLFWLayer::OnAttach()
{
	//Check and create ImGui context.
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	//Set ImGui flags.
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	//Set ImGui theme.
	ImGui::StyleColorsDark();

	//Initialize opengl and glfw implementations.
	ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)m_window.GetNativeWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 330");
}