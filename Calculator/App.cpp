#include "Calculator.h"
#include "Window.h"

int main()
{
	/*Calculator App;

	App.AddFunction([&App]() {return App.FirstEntry(1); });
	App.AddFunction([&App]() {return App.AdditionOperator(1); });
	App.AddFunction([&App]() {return App.SubtractionOperator(0.5f); });

	App.StartQueue();
	App.DebugDisplay();*/

	Window window;

	if (!window.startup())
	{
		return -1;
	}

	while (window.update())
	{
		window.draw();
	}

	window.shutdown();

	return 0;
}