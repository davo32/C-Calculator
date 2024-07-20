#include "Calculator.h"
#include <cmath>
#include <iostream>
#include <sstream>
#include <iomanip>

void Calculator::OperatorButton(const char* label, ImVec2 ButtonSize)
{
	if (ImGui::Button(label, ButtonSize))
	{
		char* temp2;

		// Print for debugging
		std::cout << "Before conversion, expression: '" << expression << "'" << std::endl;

		// Convert expression to float
		first = strtof(expression, &temp2);
		std::cout << "FIRST: " << first << std::endl;

		// Check if the entire expression was converted
		if (*temp2 != '\0' && *temp2 != ' ') {
			std::cerr << "Error: Conversion of expression to float failed or incomplete." << std::endl;
			first = 0.0; // Handle error or set a default value
		}

		// Print for debugging
		std::cout << "After conversion, first operand: " << first << std::endl;

		// Clear expression for the next input
		snprintf(expression, sizeof(expression), "%s", "");
		operators = label;
	}
}

void Calculator::EqualsOperatorButton(const char* label, ImVec2 ButtonSize)
{
	if (ImGui::Button(label, ButtonSize))
	{
		char* temp;
		second = strtof(expression,&temp);
		std::cout << "Before: " << std::to_string(first) << ":" << second << std::endl;


		//first = removeTrailingZeros(first);
		//second = removeTrailingZeros(second);


		std::string expr = formatExpression(first, operators, second);
		std::cout << "After " << first << ":" << second << std::endl;
		History.Add(expr);
		std::cout << "FINAL: " << expr << std::endl;

		if (operators == "+")
		{
			result = first + second;
		}
		else if (operators == "-")
		{
			result = first - second;
		}
		else if (operators == "*")
		{
			result = first * second;
		}
		else if (operators == "/")
		{
			if (second != 0.0)
			{
				result = first / second;
			}
			else
			{
				// Handle division by zero error
				result = 0.0; // or set a specific error value
			}
		}

		// Remove trailing zeros from result
		result = removeTrailingZeros(result);

		// Format result into expression
		snprintf(expression, sizeof(expression), "%.2f", result);
		
		// Add formatted expression to history
		History.Add(expression);
		std::cout << expression << std::endl;
	}
}

void Calculator::RevertToHistory(int index)
{
	std::string expr = History.Get(index);
	if (!expr.empty())
	{
		strncpy_s(expression, expr.c_str(), sizeof(expression));
		expression[sizeof(expression) - 1] = '\0';//Null terminator
	}
}

double Calculator::removeTrailingZeros(double value)
{
	std::ostringstream oss;
	oss << std::fixed << std::setprecision(15) << value; // Adjust precision as needed
	std::string result = oss.str();

	// Remove trailing zeros
	size_t decimalPos = result.find('.');
	if (decimalPos != std::string::npos)
	{
		result.erase(result.find_last_not_of('0') + 1);
		if (result.back() == '.')
		{
			result.pop_back();
		}
	}

	return std::stod(result);
}

std::string Calculator::formatExpression(double first, const std::string& operators, double second) {
	std::ostringstream oss;

	// Set the precision to avoid unnecessary trailing zeros
	oss << std::fixed << std::setprecision(15);

	// Convert the first number
	oss << first;
	std::string firstStr = oss.str();
	size_t dotPos = firstStr.find('.');
	if (dotPos != std::string::npos) {
		firstStr.erase(firstStr.find_last_not_of('0') + 1);
		if (firstStr.back() == '.') {
			firstStr.pop_back();
		}
	}

	// Clear the stringstream
	oss.str("");
	oss.clear();

	// Convert the second number
	oss << second;
	std::string secondStr = oss.str();
	dotPos = secondStr.find('.');
	if (dotPos != std::string::npos) {
		secondStr.erase(secondStr.find_last_not_of('0') + 1);
		if (secondStr.back() == '.') {
			secondStr.pop_back();
		}
	}

	// Construct the final expression
	std::string expr = firstStr + " " + operators + " " + secondStr;

	return expr;
}