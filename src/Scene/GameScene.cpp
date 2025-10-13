#include "GameScene.hpp"
#include "../Core/Application.hpp"
#include "../Map/MapLoader.hpp"	

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

	//m_mapCollisions << RectF{ -1000, -1000, 2000, 50 }; JSONから読み込むのでコメントアウト

	m_mapCollisions = MapLoader::LoadCollisions(U"map_01");

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

	Vec2 currentPos = m_player.GetPosition();
	Vec2 nextVelocity = m_player.GetVelocity();

	Vec2 nextPosX = currentPos + Vec2{ nextVelocity.x, 0.0 };
	Circle nextPlayerColX = m_player.GetCollision().getWorldShape(nextPosX);

	bool collideX = false;
	for (const auto& mapCol : m_mapCollisions)
	{
		if (nextPlayerColX.intersects(mapCol))
		{
			collideX = true;
			break;
		}
	}
	if (!collideX)
	{
		// 衝突しなければX位置を更新
		currentPos.x = nextPosX.x;
	}

	Vec2 nextPosY = currentPos + Vec2{ 0.0, nextVelocity.y };
	Circle nextPlayerColY = m_player.GetCollision().getWorldShape(nextPosY);

	bool collideY = false;
	for (const auto& mapCol : m_mapCollisions)
	{
		if (nextPlayerColY.intersects(mapCol))
		{
			collideY = true;
			break;
		}
	}
	if (!collideY)
	{
		currentPos.y = nextPosY.y;
	}

	m_player.SetPosition(currentPos);
	m_player.SetVelocity(Vec2{ 0, 0 });

	//カメラ更新
	m_MainCamera.SetTarget(m_player.GetPosition());
	m_MainCamera.Update();
}

void GameScene::draw() const
{
	Scene::SetBackground(ColorF{ 0.05, 0.0, 0.05 }); // 夜色

	const ScopedRenderStates2D blend{ SamplerState::ClampNearest };

	{
		const auto t = m_MainCamera.GetTransformer();
#ifdef _DEBUG

		// グリッド線で地面を可視化
		for (int y = -5; y <= 5; ++y)
		{
			for (int x = -5; x <= 5; ++x)
			{
				RectF{ x * 64.0 - 32, y * 64.0 - 32, 64, 64 }
				.draw(ColorF{ 0.1 + ((x + y) % 2) * 0.05 });
			}
		}
		// コリジョンを赤枠で描画
		for (const auto& mapCol : m_mapCollisions)
		{
			mapCol.draw(ColorF{ 1.0, 0.0, 0.0, });
		}
		// 緑枠でプレイヤー用の円形コリジョンを描画
		m_player.GetCollision().getWorldShape(m_player.GetPosition())
			.drawFrame(2, 0, ColorF{ 0.0, 1.0, 0.0 });
#endif
	}

	{
		ScopedSpotlight target{ m_spotlight, ColorF{ 0.05, 0.05, 0.1 } };
		m_player.DrawLight(m_MainCamera);
	}

	m_spotlight.draw();

	{
		const auto t = m_MainCamera.GetTransformer();
		m_player.DrawCharacter(m_MainCamera);
	}

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
