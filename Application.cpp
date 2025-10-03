//Application.cpp
#include "Application.hpp"

Application::Application()
{ }

Application::~Application()
{ }

bool Application::init()
{
	Window::Resize(WINDOW_WIDTH, WINDOW_HEIGHT);
	return true;
}

void Application::update()
{
	//更新処理
}

void Application::draw()
{
	//描画処理
	Circle{ 400,300,50 }.draw(Palette::Orange);
}
