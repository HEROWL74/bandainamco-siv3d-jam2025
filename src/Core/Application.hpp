//Application.hpp
#pragma once
#include <Siv3D.hpp>
#include "../Option/Settings.hpp"
#include "../Option/AudioManager.hpp"
#include "../Scene/SceneTransition.hpp"
#include <memory>

//アプリケーションクラス
class Application
{
private:
	std::shared_ptr<Settings> m_settings;
	std::shared_ptr<AudioManager> m_audio;
	std::unique_ptr<SceneTransition> m_scene;

public:
	Application();
	~Application();

	bool init();
	void update();
	void draw();

	static constexpr int WINDOW_WIDTH = 1920;
	static constexpr int WINDOW_HEIGHT = 1080;
};
