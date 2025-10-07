//Application.cpp
#include "Application.hpp"


Application::Application()
	: m_playerTexture(U"example/spritesheet/siv3d-kun-16.png")
	, m_player(m_playerTexture)
{
}

Application::~Application()
{
}

bool Application::init()
{
	Window::Resize(WINDOW_WIDTH, WINDOW_HEIGHT);
	Graphics::SetVSyncEnabled(true);

	// サークルの初期位置
	mCirclePos.x = 400.0;
	mCirclePos.y = 300.0;

	//----------------------
	// プレイヤー初期化
	//---------------------

	m_player.SetPosition(Scene::Center());
	m_player.InitAnimation();

	return true;
}

void Application::update()
{
	//更新処理
	if (KeyD.pressed())
	{
		mCirclePos.x += CIRCLE_SPEED;
		if (mCirclePos.x > WINDOW_WIDTH - 50.0)
			mCirclePos.x = WINDOW_WIDTH - 50.0;
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
		if (mCirclePos.y > WINDOW_HEIGHT - 50.0)
			mCirclePos.y = WINDOW_HEIGHT - 50.0;
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

void Application::draw()
{
	//描画処理
	Circle{ mCirclePos.x, mCirclePos.y, 50 }.draw(Palette::Orange);

	// プレイヤー描画
	{
		const ScopedRenderStates2D sampler{ SamplerState::ClampNearest };
		m_player.Draw();
	}
}
