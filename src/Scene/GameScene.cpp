#include "GameScene.hpp"
#include "../Core/Application.hpp"


// コンストラクタ
GameScene::GameScene(const InitData& init)
	:IScene(init)
	, m_playerTexture(U"example/spritesheet/siv3d-kun-16.png")
	, m_player(m_playerTexture)
	, m_enemyTexture(U"example/spritesheet/siv3d-kun-16.png")
	, m_enemy(m_enemyTexture, m_player)
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

	m_enemy.SetPosition(Scene::Center() + Vec2{ 200, -100 });
	m_enemy.InitAnimation();
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

	m_enemy.Update();

	// カメラ更新
	m_MainCamera.SetTarget(m_player.GetPosition());
	m_MainCamera.Update();
}

void GameScene::draw() const
{
	Scene::SetBackground(ColorF{ 0.8, 0.0, 0.2 });

	// カメラ適用
	const auto transformer = m_MainCamera.GetTransformer();

	Circle{ mCirclePos.x, mCirclePos.y, 50 }.draw(Palette::Orange);

#ifdef DEBUG
	Rect{ 0, 0, Application::WINDOW_WIDTH, Application::WINDOW_HEIGHT }.drawFrame(40.0, Palette::Skyblue);
#endif // DEBUG
	Rect{ 0, 0, Application::WINDOW_WIDTH, Application::WINDOW_HEIGHT }.drawFrame(40.0, Palette::Skyblue);


	const ScopedRenderStates2D sampler{ SamplerState::ClampNearest };
	m_player.Draw();
	m_enemy.Draw();
}

bool GameScene::Release()
{


	return true;
}
