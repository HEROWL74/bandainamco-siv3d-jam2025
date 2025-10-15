//Application.cpp
#include "Application.hpp"


Application::Application()
	:m_settings(nullptr)
	,m_audio(nullptr)
	,m_scene(nullptr)
{
}

Application::~Application()
{
}

bool Application::init()
{
	Window::Resize(WINDOW_WIDTH, WINDOW_HEIGHT);
	Graphics::SetVSyncEnabled(true);

	m_settings = std::make_shared<Settings>();
	if (m_settings == nullptr) return false;
	if (!m_settings->Load()) return false;					// 音量設定のロード

	m_audio = std::make_shared<AudioManager>(m_settings);
	if (m_audio == nullptr) return false;

	m_scene = std::make_unique<SceneTransition>(m_settings, m_audio);
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
