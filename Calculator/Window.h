#pragma once
#include "glad.h"
#include "imgui.h"
#include "imgui_internal.h"
#include <GLFW/glfw3.h>
#include "Calculator.h"

class Window
{
public:
	Window() {  }
	bool startup();
	bool update();
	void draw();
	void SetupDockspace(ImGuiIO& io);
	void CreateHistoryUI(ImGuiWindowFlags windowFlags);
	void CreateCalculatorUI(ImGuiWindowFlags windowFlags);
	void shutdown();

	void setBackgroundColour(float r, float g, float b, float a);
	//ImGuiID GetDockSpaceID() { return dockspaceID; }


private:
	GLFWwindow* m_window;
	int windowWidth = 340;
	int windowHeight = 320;
	//ImGuiID dockspaceID;

	Calculator calculator;
	char expression[256] = "0";

	ImFont* DefaultFont;
	ImFont* largerFont;


	ImVec2 ButtonSize = ImVec2(45, 45);

	ImGuiWindowFlags windowFlags
		= ImGuiWindowFlags_NoTitleBar
		| ImGuiWindowFlags_NoMove
		| ImGuiWindowFlags_NoResize
		| ImGuiWindowFlags_NoBringToFrontOnFocus
		| ImGuiWindowFlags_NoScrollbar
		| ImGuiWindowFlags_NoScrollWithMouse
		| ImGuiWindowFlags_MenuBar;

	
};

