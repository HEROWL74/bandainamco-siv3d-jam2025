//Application.hpp
#pragma once
#include <Siv3D.hpp>

//アプリケーションクラス
class Application
{
private:
	static constexpr int WINDOW_WIDTH = 800;
	static constexpr int WINDOW_HEIGHT = 600;

	static constexpr double CIRCLE_SPEED = 3.0;

	Vec2 mCirclePos{};	// サークルの位置

public:
	Application();
	~Application();

    bool init();
	void update();
	void draw();
};
