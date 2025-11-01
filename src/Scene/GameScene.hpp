#pragma once

#include <Siv3D.hpp>
#include "SceneTransition.hpp"
#include "../Character/Player.hpp"
#include "../Effect/EffectManager.hpp"
#include "../Render/Spotlight.hpp"
#include "../Map/Door/Door.hpp"
#include "../Option/GameOption.hpp"

enum class GameState
{
	Game,
	Option,
};


class GameScene : public App::Scene
{
private:
	void HandleDoorTransition();

	static constexpr double CIRCLE_SPEED = 3.0;

	Vec2 mCirclePos{};	// サークルの位置

	Texture m_playerTexture;

	Player m_player;

	std::unique_ptr<Door> m_door;

	EffectManager m_effectManager;

	// オプション関連
	Array<RectF> m_mapCollisions;

	MainCamera m_MainCamera;
	Spotlight m_spotlight;
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
