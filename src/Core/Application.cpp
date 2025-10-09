//Application.cpp
#include "Application.hpp"


Application::Application()
	:mScene(nullptr)
{
}

Application::~Application()
{
}

bool Application::init()
{
	Window::Resize(WINDOW_WIDTH, WINDOW_HEIGHT);
	Graphics::SetVSyncEnabled(true);

	mScene = std::make_unique<SceneTransition>();
	if (!mScene->SystemInit()) return false;

	return true;
}

void Application::update()
{
	mScene->Update();
}

void Application::draw()
{

}
