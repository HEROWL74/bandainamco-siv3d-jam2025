//Application.cpp
#include "Application.hpp"


Application::Application()
	:m_scene(nullptr)
{
}

Application::~Application()
{
}

bool Application::init()
{
	Window::Resize(WINDOW_WIDTH, WINDOW_HEIGHT);
	Graphics::SetVSyncEnabled(true);

	m_scene = std::make_unique<SceneTransition>();
	if (!m_scene->SystemInit()) return false;

	return true;
}

void Application::update()
{
	m_scene->Update();
}

void Application::draw()
{

}
