#pragma once
#include <vector>
#include <string>
#include <memory>
#include <functional>

class CalculatorHistory
{
public:
	std::vector<std::unique_ptr<char[]>> history;
	void Add(const std::string& expression);
	std::string Get(int index);
	void Display(std::function<void(int)> onButtonClick);
};

