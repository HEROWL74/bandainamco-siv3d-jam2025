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
	case GameState::Game:	// ゲーム画面
		// エンターキーでシーンをresultへ
		if (KeyEnter.down())
		{
			changeScene(SceneState::RESULT);
		}
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

	if (m_optionButton.leftClicked())
	{
		m_gameState = GameState::Option;
	}


	// プレイヤー更新
	m_player.Update();

	//カメラ更新
	m_MainCamera.SetTarget(m_player.GetPosition());
	m_MainCamera.Update();
}

void GameScene::draw() const
{
	Scene::SetBackground(ColorF{ 0.05, 0.0, 0.05 }); // 夜色

	const ScopedRenderStates2D blend{ SamplerState::ClampNearest };

	// カメラ視点で床を描く（ワールド座標）
	{
		const auto t = m_MainCamera.GetTransformer();
#ifdef _DEBUG
		// グリッド線で地面を可視化
		for (int y = -5; y <= 5; ++y)
		{
			for (int x = -5; x <= 5; ++x)
			{
				Vec3 worldPos{ x * 64.0, y * 64.0, 0.0 };
				Vec2 screenPos = m_MainCamera.WorldToScreen(worldPos);

				RectF{ screenPos.x - 32, screenPos.y - 32, 128, 128 }
				.draw(ColorF{ 0.1 + ((x + y) % 2) * 0.05 });
			}
		}
#endif
	}

	// ライト
	{
		ScopedSpotlight target{ m_spotlight, ColorF{ 0.05, 0.05, 0.1 } };
		m_player.DrawLight(m_MainCamera);
	}

	m_spotlight.draw();

	// キャラ描画
	m_player.DrawCharacter(m_MainCamera);
	//描画処理
	Circle{ mCirclePos.x, mCirclePos.y, 50 }.draw(Palette::Orange);

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
