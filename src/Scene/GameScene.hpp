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

	Vec2 mCirclePos{};	// サークルの位置

	Texture m_playerTexture;
	Player m_player;

	// オプション関連
	Texture m_optionIcon;
	RoundRect m_optionButton;

	std::unique_ptr<GameOption> m_gameOption;
	GameState m_gameState;


public:
	GameScene(const InitData& init);
	~GameScene();

	bool SystemInit();
	void GameInit();
	void update() override;
	void draw() const override;
	bool Release();



};
