#include "pch.h"
#include "platform.h"
#include <core/window.h>
#include <platform/glfw/glfw_window.h>
#include <platform/glfw/ImGuiGLFWLayer.h>

//Initialize the s_type static variable.
Amen::PlatformTypeE Amen::Platform::s_type;



void Amen::Platform::Init()
{
	#ifdef AMEN_WINDOWS
		s_type = PlatformTypeE::WINDOWS;
	#endif

	#ifdef AMEN_LINUX
		s_type = PlatformTypeE::LINUX;
	#endif
}


//GLFW Window.
#if defined(AMEN_WINDOWS) || defined(AMEN_LINUX)

	//Implement Window::Create()
	Amen::Window* Amen::Window::Create(const Amen::WindowProps& props)
	{
		return new GLFW_Window(props);
	}

	//Implement ImguiLayer::Create()
	Amen::Layer* Amen::ImguiLayer::Create()
	{
		return new ImGuiGLFWLayer();
	}
#endif