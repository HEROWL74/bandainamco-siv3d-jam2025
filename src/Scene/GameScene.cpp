#include "GameScene.hpp"
#include "../Core/Application.hpp"


// コンストラクタ
GameScene::GameScene(const InitData& init)
	:IScene(init)
	, m_playerTexture(U"example/spritesheet/siv3d-kun-16.png")
	, m_player(m_playerTexture)
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


	return true;
}

void GameScene::GameInit()
{
	// サークルの初期位置
	mCirclePos.x = 400.0;
	mCirclePos.y = 300.0;

	//----------------------
	// プレイヤー初期化
	//----------------------
	m_player.SetPosition(Scene::Center());
	m_player.InitAnimation();
}

void GameScene::update()
{
	if (KeyEnter.down() || MouseL.down())
	{
		changeScene(SceneState::RESULT);
	}

	// プレイヤー更新
	m_player.Update();

	// カメラ更新
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
}



bool GameScene::Release()
{


	return true;
}
