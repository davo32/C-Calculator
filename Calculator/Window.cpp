#include "Window.h"
#include <stdio.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"
#include <string>
#include <iostream>

bool Window::startup()
{
	if (glfwInit() == false)
	{
		return false;
	}

	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	m_window = glfwCreateWindow(windowWidth,windowHeight, "Calculator", nullptr, nullptr);

	

	if (m_window == nullptr)
	{
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(m_window);

	if (!gladLoadGL())
	{
		glfwDestroyWindow(m_window);
		glfwTerminate();
		return false;
	}

	printf("GL: %i.%i\n", GLVersion.major, GLVersion.minor);

	ImGui::CreateContext();

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();


	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(m_window, true);
	ImGui_ImplOpenGL3_Init("#version 130");

	// update IMGUI input flags
	ImGuiIO& io = ImGui::GetIO();
	DefaultFont = io.Fonts->AddFontDefault();

	ImFontConfig config;
	config.SizePixels = 20.0f;
	largerFont = io.Fonts->AddFontDefault(&config);
	io.Fonts->Build();
	// Docking
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	setBackgroundColour(0.25, 0.25, 0.25, 1.0f);
	return true;
}

bool Window::update()
{
	while (glfwWindowShouldClose(m_window) || glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		return false;
	}
		
	glfwPollEvents();
	return true;
}

void Window::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glfwGetFramebufferSize(m_window, &windowWidth, &windowHeight);
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2((float)windowWidth, (float)windowHeight);
	

	//Start the ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	CreateHistoryUI(windowFlags);
	CreateCalculatorUI(windowFlags | extraFlags);
	

	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	glfwSwapBuffers(m_window);
}



void Window::CreateHistoryUI(ImGuiWindowFlags windowFlags)
{
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.70f, 0.70f, 0.70f, 1.0f));


	ImGui::SetNextWindowSize(ImVec2(120, 320));
	ImGui::SetNextWindowPos(ImVec2(220, 0));

	if (ImGui::Begin("History", (bool*)1, windowFlags))
	{
		ImGui::Text("History");
		calculator.History.Display([this](int index)
			{
				this->calculator.RevertToHistory(index);
			});
	}
	ImGui::End();

	ImGui::PopStyleColor();
}

void Window::CreateCalculatorUI(ImGuiWindowFlags windowFlags)
{
	// Set the background color to opaque white
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.10f, 0.10f, 0.10f, 1.0f));


	ImGui::SetNextWindowSize(ImVec2(220, 320));
	ImGui::SetNextWindowPos(ImVec2(0, 0));

	// Create a simple window
	{
		ImGui::Begin("Calculator",(bool*)1,windowFlags);

		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::MenuItem("About"))
			{
				ImGui::OpenPopup("About", ImGuiPopupFlags_None);
				
			}

			if (ImGui::BeginPopup("About"))
			{
				ImGui::Text("Developed by David Shannon");
				if (ImGui::Button("Close", ImVec2(40, 20)))
				{
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}


			ImGui::Text("            |          ");
			if (ImGui::MenuItem("Clear History"))
			{
				//Clear History...
				calculator.History.history.clear();
			}
			ImGui::EndMainMenuBar();
		}

		
		ImGui::PushFont(largerFont);
		ImVec2 ISize(200, 27);
		ImGuiInputTextFlags flags = ImGuiInputTextFlags_ReadOnly | ImGuiInputTextFlags_AutoSelectAll;
		ImGui::InputTextMultiline("##Expression", calculator.expression, sizeof(calculator.expression),ISize,flags);

		ImGui::PopFont();
		//Clear All
		if (ImGui::Button("C", ButtonSize))
		{
			snprintf(calculator.expression, sizeof(calculator.expression), "%d", 0);
		}
		ImGui::SameLine();
		//Clear Entry
		if (ImGui::Button("CE", ButtonSize))
		{
			snprintf(calculator.expression, sizeof(calculator.expression), "%d", 0);
		}
		ImGui::SameLine();
		//Backspace
		if (ImGui::Button("Bs", ButtonSize))
		{
			size_t length = strlen(calculator.expression);

			if (length > 0)
			{
				calculator.expression[length - 1] = '\0';
			}
		}

		// Number buttons
		for (int i = 1; i <= 9; ++i) {
			//Numbers
			if (ImGui::Button(std::to_string(i).c_str(), ButtonSize))
			{
				if (calculator.expression[0] == '0')
				{
					snprintf(calculator.expression,sizeof(calculator.expression), "%i", i);
				}
				else
				{
					strcat_s(calculator.expression, std::to_string(i).c_str());
				}
				
			}
			if (i % 3 != 0) ImGui::SameLine();
			if (i == 3) 
			{ 
				ImGui::SameLine(); 
				//Multiply
				calculator.OperatorButton("*", ButtonSize);
			}
			else if (i == 6)
			{
				ImGui::SameLine();
				//Divide
				calculator.OperatorButton("/", ButtonSize);
			}
			else if (i == 9)
			{
				ImGui::SameLine();
				//Decimal
				if (ImGui::Button(".", ButtonSize))
				{
					char target = '.';
					char* found = strchr(calculator.expression, target);

					if (found == nullptr)
					{
						strcat_s(calculator.expression, ".");
					}
				}
			}
		}

		//Plus
		calculator.OperatorButton("+", ButtonSize);
		ImGui::SameLine();

		// Zero
		if (ImGui::Button("0", ButtonSize)) 
		{
			strcat_s(calculator.expression, "0");
		}

		ImGui::SameLine();

		//Minus
		calculator.OperatorButton("-", ButtonSize);

		ImGui::SameLine();

		//Equals
		calculator.EqualsOperatorButton("=", ButtonSize);

		ImGui::End();
	}


	ImGui::PopStyleColor();
}

void Window::shutdown()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(m_window);
	glfwTerminate();
}

void Window::setBackgroundColour(float r, float g, float b, float a) 
{
	glClearColor(r, g, b, a);
}


