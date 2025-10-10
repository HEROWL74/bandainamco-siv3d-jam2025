//Application.hpp
#pragma once
#include <Siv3D.hpp>
#include "../Scene/SceneTransition.hpp"
#include <memory>

//アプリケーションクラス
class Application
{
private:
	std::unique_ptr<SceneTransition> m_scene;

public:
	Application();
	~Application();

	bool init();
	void update();
	void draw();

	static constexpr int WINDOW_WIDTH = 800;
	static constexpr int WINDOW_HEIGHT = 600;
};
