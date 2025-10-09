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

	// 円の移動処理
	if (KeyD.pressed())
	{
		mCirclePos.x += CIRCLE_SPEED;
		if (mCirclePos.x > Application::WINDOW_WIDTH - 50.0)
			mCirclePos.x = Application::WINDOW_WIDTH - 50.0;
	}
	if (KeyA.pressed())
	{
		mCirclePos.x -= CIRCLE_SPEED;
		if (mCirclePos.x < 50.0)
			mCirclePos.x = 50.0;
	}
	if (KeyS.pressed())
	{
		mCirclePos.y += CIRCLE_SPEED;
		if (mCirclePos.y > Application::WINDOW_HEIGHT - 50.0)
			mCirclePos.y = Application::WINDOW_HEIGHT - 50.0;
	}
	if (KeyW.pressed())
	{
		mCirclePos.y -= CIRCLE_SPEED;
		if (mCirclePos.y < 50.0)
			mCirclePos.y = 50.0;
	}

	// プレイヤー更新
	m_player.Update();
}

void GameScene::draw() const
{
	Scene::SetBackground(ColorF{ 0.8, 0.0, 0.2 });

	//描画処理
	Circle{ mCirclePos.x, mCirclePos.y, 50 }.draw(Palette::Orange);

	// プレイヤー描画
	{
		const ScopedRenderStates2D sampler{ SamplerState::ClampNearest };
		m_player.Draw();
	}
}

bool GameScene::Release()
{


	return true;
}
