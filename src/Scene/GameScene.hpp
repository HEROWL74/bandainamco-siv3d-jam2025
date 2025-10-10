#pragma once

#include <Siv3D.hpp>
#include "SceneTransition.hpp"
#include "../Character/Player.hpp"
#include "../Option/GameOption.hpp"

enum class GameState
{
	Game,
	Option,
};

class GameScene : public App::Scene
{
private:
	static constexpr double CIRCLE_SPEED = 3.0;

	Vec2 m_circlePos{};	// サークルの位置

	Texture m_playerTexture;
	Player m_player;

	std::unique_ptr<class GameOption> m_gameOption;
	GameState m_gameState;

	Texture m_optionIcon;
	RoundRect m_optionButton;

public:
	GameScene(const InitData& init);
	~GameScene();

	bool SystemInit();
	void GameInit();
	void update() override;
	void draw() const override;
	bool Release();



};
