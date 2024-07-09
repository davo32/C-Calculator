#include "CalculatorHistory.h"
#include "imgui.h"
#include <iostream>

void CalculatorHistory::Add(const std::string& expression)
{
	if (!expression.empty()) // Ensure the expression is not empty
	{
		auto expr = std::make_unique<char[]>(expression.size() + 1);
		strncpy_s(expr.get(), expression.size() + 1, expression.c_str(), _TRUNCATE);
		history.push_back(std::move(expr));
	}
}

std::string CalculatorHistory::Get(int index)
{
	if (index >= 0 && index < history.size())
	{
		return history[index].get();
	}
	return "";
}

void CalculatorHistory::Display(std::function<void(int)> onButtonClick)
{
	auto start = history.begin();
	auto end = history.end();

	for (int i = 0; i < history.size(); ++i)
	{
		std::string buttonLabel = std::string(history[i].get()) + "##";
		if (ImGui::Button(buttonLabel.c_str()))
		{
			onButtonClick(i);
			for (i; i < history.size(); i++)
			{
				history.erase(start + i, end);
			}
		}
	}
}