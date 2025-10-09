#pragma once

#include <Siv3D.hpp>
#include "SceneTransition.hpp"
#include "Player.hpp"

class Game : public App::Scene
{
private:
	static constexpr double CIRCLE_SPEED = 3.0;

	Vec2 mCirclePos{};	// サークルの位置

	Texture m_playerTexture;
	Player m_player;

public:
	Game(const InitData& init);
	~Game();

	bool SystemInit();
	void GameInit();
	void update() override;
	void draw() const override;
	bool Release();



};
