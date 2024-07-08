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
	CreateCalculatorUI(windowFlags);
	

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
		//ImGui::Text("%s", calculator.GetRollingEquation().c_str());
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
				
			}
			ImGui::Text("            |          ");
			if (ImGui::MenuItem("Clear History"))
			{
				//Clear History...
			}
			ImGui::EndMainMenuBar();
		}

		
		//ImGuiIO& io = ImGui::GetIO();
		
		ImGui::PushFont(largerFont);
		ImVec2 ISize(200, 27);
		ImGuiInputTextFlags flags = ImGuiInputTextFlags_ReadOnly | ImGuiInputTextFlags_AutoSelectAll;
		ImGui::InputTextMultiline("##Expression", expression, sizeof(expression),ISize,flags);

		ImGui::PopFont();

		if (ImGui::Button("C", ButtonSize))
		{
			snprintf(expression, sizeof(expression), "%d", 0);
		}
		ImGui::SameLine();
		if (ImGui::Button("CE", ButtonSize))
		{
			snprintf(expression, sizeof(expression), "%d", 0);
		}
		ImGui::SameLine();
		if (ImGui::Button("Bs", ButtonSize))
		{
			size_t length = strlen(expression);

			if (length > 0)
			{
				expression[length - 1] = '\0';
			}
		}

		// Number buttons
		for (int i = 1; i <= 9; ++i) {
			if (ImGui::Button(std::to_string(i).c_str(), ButtonSize))
			{
				if (expression[0] == '0')
				{
					snprintf(expression,sizeof(expression), "%i", i);
				}
				else
				{
					strcat_s(expression, std::to_string(i).c_str());
				}
				
			}
			if (i % 3 != 0) ImGui::SameLine();
			if (i == 3) 
			{ 
				ImGui::SameLine();  
				if (ImGui::Button("*", ButtonSize))
				{
					//strcat_s(expression, "*");
					char* temp;
					calculator.first = strtod(expression, &temp);
					snprintf(expression, sizeof(expression), "%s", "");
					calculator.operators = "*";
					std::cout << calculator.first << std::endl;
					//delete temp;
				}
			}
			else if (i == 6)
			{
				ImGui::SameLine();
				if (ImGui::Button("/", ButtonSize))
				{
					//strcat_s(expression, "/");
					char* temp;
					calculator.first = strtod(expression, &temp);
					snprintf(expression, sizeof(expression), "%s", "");
					calculator.operators = "/";
					//delete temp;
				}
			}
			else if (i == 9)
			{
				ImGui::SameLine();
				if (ImGui::Button(".", ButtonSize))
				{
					char target = '.';
					char* found = strchr(expression, target);

					if (found == nullptr)
					{
						strcat_s(expression, ".");
					}
				}
			}
		}


		if (ImGui::Button("+", ButtonSize))
		{
			//strcat_s(expression, "+");
			char* temp;
			calculator.first = strtod(expression, &temp);
			snprintf(expression, sizeof(expression), "%s", "");
			calculator.operators = "+";
			//delete temp;
		}
		ImGui::SameLine();
		// Zero button
		if (ImGui::Button("0", ButtonSize)) 
		{
			strcat_s(expression, "0");
		}
		ImGui::SameLine();
		if (ImGui::Button("-", ButtonSize))
		{
			//strcat_s(expression, "-");
			char* temp;
			calculator.first = strtod(expression, &temp);
			snprintf(expression, sizeof(expression), "%s", "");
			calculator.operators = "-";
			//delete temp;
		}
		ImGui::SameLine();
		if (ImGui::Button("=", ButtonSize))
		{
			char* temp;
			calculator.second = strtod(expression, &temp);
			std::cout << calculator.second << std::endl;

			if (calculator.operators == "+")
			{
				calculator.result = calculator.first + calculator.second;
				snprintf(expression, sizeof(expression), "%f", calculator.result);
			}
			else if (calculator.operators == "-")
			{
				calculator.result = calculator.first - calculator.second;
				snprintf(expression, sizeof(expression), "%f", calculator.result);
			}
			else if (calculator.operators == "*")
			{
				calculator.result = calculator.first * calculator.second;
				snprintf(expression, sizeof(expression), "%f", calculator.result);
			}
			else if (calculator.operators == "/")
			{
				calculator.result = calculator.first / calculator.second;
				snprintf(expression, sizeof(expression), "%f", calculator.result);
			}
			std::cout << calculator.result << std::endl;
		}


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


