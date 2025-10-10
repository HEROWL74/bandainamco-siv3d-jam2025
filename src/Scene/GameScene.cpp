#include "GameScene.hpp"
#include "../Core/Application.hpp"


// コンストラクタ
GameScene::GameScene(const InitData& init)
	:IScene(init)
	, m_playerTexture(U"example/spritesheet/siv3d-kun-16.png")
	, m_player(m_playerTexture)
	, m_gameOption(nullptr)
{
	SystemInit();
	GameInit();
}

// デストラクタ
GameScene::~GameScene()
{
	Release();
}

bool GameScene::SystemInit()
{
	m_gameOption = std::make_unique<GameOption>();
	m_gameOption->SystemInit();

	m_optionIcon = Texture{ U"⚙️"_emoji };

	return true;
}

void GameScene::GameInit()
{
	// サークルの初期位置
	m_circlePos.x = 400.0;
	m_circlePos.y = 300.0;

	//----------------------
	// プレイヤー初期化
	//----------------------
	m_player.SetPosition(Scene::Center());
	m_player.InitAnimation();

	// ゲームの状態
	m_gameState = GameState::Game;

	m_gameOption->GameInit();

	// オプションボタンの座標
	const double buttonX = 750.0;
	const double buttonY = 550.0;
	const double buttonW = 40;
	const double ronded = 6;

	m_optionButton = RectF{ Arg::center(buttonX, buttonY), buttonW, buttonW }.rounded(ronded);
}

void GameScene::update()
{
	switch (m_gameState)
	{
	case GameState::Game:												// ゲーム画面

		// エンターキーでシーンをresultへ
		if (KeyEnter.down())
		{
			changeScene(SceneState::RESULT);
		}

		// 円の移動処理
		if (KeyD.pressed())
		{
			m_circlePos.x += CIRCLE_SPEED;
			if (m_circlePos.x > Application::WINDOW_WIDTH - 50.0)
				m_circlePos.x = Application::WINDOW_WIDTH - 50.0;
		}
		if (KeyA.pressed())
		{
			m_circlePos.x -= CIRCLE_SPEED;
			if (m_circlePos.x < 50.0)
				m_circlePos.x = 50.0;
		}
		if (KeyS.pressed())
		{
			m_circlePos.y += CIRCLE_SPEED;
			if (m_circlePos.y > Application::WINDOW_HEIGHT - 50.0)
				m_circlePos.y = Application::WINDOW_HEIGHT - 50.0;
		}
		if (KeyW.pressed())
		{
			m_circlePos.y -= CIRCLE_SPEED;
			if (m_circlePos.y < 50.0)
				m_circlePos.y = 50.0;
		}

		//オプションボタンを押した時の処理
		if (m_optionButton.leftClicked())
		{
			m_gameState = GameState::Option;
		}

		// プレイヤー更新
		m_player.Update();

		break;

	case GameState::Option:												// オプション画面
		m_gameOption->Update();
		if (m_gameOption->IsClosed())
		{
			m_gameState = GameState::Game;
			m_gameOption->GameInit();
		}
		break;
	}
	
}

void GameScene::draw() const
{
	Scene::SetBackground(ColorF{ 0.8, 0.0, 0.2 });

	//描画処理
	Circle{ m_circlePos.x, m_circlePos.y, 50 }.draw(Palette::Orange);

	// プレイヤー描画
	{
		const ScopedRenderStates2D sampler{ SamplerState::ClampNearest };
		m_player.Draw();
	}	

	// 設定ボタンの描画	
	m_optionButton.draw(Palette::Silver);

	// マウスが図形の上に来たら影ができる
	if (m_optionButton.mouseOver())
	{
		m_optionButton.drawShadow(Vec2{ 2, 2 }, 12, 1).draw(ColorF{ 0.9, 0.8, 0.6 });
	}
	// 歯車マーク
	double iconX = 750.0;
	double iconY = 550.0;
	m_optionIcon.scaled(0.3).drawAt(iconX, iconY);

	// オプション画面の描画
	if (m_gameState == GameState::Option)
	{
		m_gameOption->Draw();
	}
}

bool GameScene::Release()
{


	return true;
}
