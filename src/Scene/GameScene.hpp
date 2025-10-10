#pragma once

#include <Siv3D.hpp>
#include "SceneTransition.hpp"
#include "../Character/Player.hpp"
#include "../Render/Spotlight.hpp"

class GameScene : public App::Scene
{
private:
	static constexpr double CIRCLE_SPEED = 3.0;

	Vec2 mCirclePos{};	// サークルの位置

	Texture m_playerTexture;
	Player m_player;

	MainCamera m_MainCamera;
	Spotlight m_spotlight;
public:
	GameScene(const InitData& init);
	~GameScene();

	bool SystemInit();
	void GameInit();
	void update() override;
	void draw() const override;
	bool Release();



};
