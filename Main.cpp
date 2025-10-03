//Main.cpp
#include "Application.hpp"

void Main()
{
	Application app;
	if (!app.init()) return;
	while (System::Update())
	{
		app.update();
		app.draw();
	}
}

