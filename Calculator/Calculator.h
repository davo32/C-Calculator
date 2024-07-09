#pragma once
#include <string>
#include "imgui.h"
#include "CalculatorHistory.h"

class Calculator
{
public:
	CalculatorHistory History;
	float first, second, result = 0;
	std::string operators;
	char expression[256] = "0";
	void OperatorButton(const char* label, ImVec2 ButtonSize);
	void EqualsOperatorButton(const char* label, ImVec2 ButtonSize);
	void RevertToHistory(int index);
	double removeTrailingZeros(double value);
	std::string formatExpression(double first, const std::string& operators, double second);
};
