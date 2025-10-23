#include "TitleScene.hpp"


// コンストラクタ
TitleScene::TitleScene(const InitData& init)
	:IScene(init)
	,m_titleOption(nullptr)
{
	SystemInit();
	GameInit();
}

// デストラクタ
TitleScene::~TitleScene()
{
	Release();
}


bool TitleScene::SystemInit()
{
	auto& data = getData();
	m_titleOption = std::make_unique<TitleOption>(data.settings, data.audio);
	if (m_titleOption == nullptr) return false;

	// titleBGMのロード
	if (data.audio)
	{
		data.audio->PreLoadBGM(U"TitleBGM", U"example/test.mp3");
	}

	m_optionIcon = Texture{ U"⚙️"_emoji };

	return true;
}

void TitleScene::GameInit()
{
	auto& data = getData();

	// BGMの再生
	if (data.audio)
	{
		data.audio->PlayBGM(U"TitleBGM", true);
	}

	// オプションボタンの座標
	const double buttonX = 1600.0;
	const double buttonY = 950.0;
	const double buttonW = 40;
	const double ronded = 6;

	m_optionButton = RectF{ Arg::center(buttonX, buttonY), buttonW, buttonW }.rounded(ronded);

	m_titleState = TitleState::Title;
}

void TitleScene::update()
{
	switch (m_titleState)
	{
	case TitleState::Title:

		if (KeyEnter.down())
		{
			changeScene(SceneState::GAME);
			getData().audio->StopBGM(1s);
		}

		// オプションボタンが押されたらオプション画面へ
		if (m_optionButton.leftClicked())
		{
			m_titleState = TitleState::Option;
		}

		break;

	case TitleState::Option:

		m_titleOption->Update();
		if (m_titleOption->IsClosed())
		{
			m_titleState = TitleState::Title;
			m_titleOption->GameInit();
		}

		break;

	case TitleState::Exit:

		break;
	}

}

void TitleScene::draw() const
{
	Scene::SetBackground(ColorF{ 0.0, 0.2, 0.8 });

	// 設定ボタンの描画	
	m_optionButton.draw(Palette::Silver);

	// マウスが図形の上に来たら影ができる
	if (m_optionButton.mouseOver())
	{
		m_optionButton.drawShadow(Vec2{ 2, 2 }, 12, 1).draw(ColorF{ 0.9, 0.8, 0.6 });
	}
	// 歯車マーク
	const double iconX = 1600.0;
	const double iconY = 950.0;
	m_optionIcon.scaled(0.3).drawAt(iconX, iconY);

	// オプション画面の描画
	if (m_titleState == TitleState::Option)
	{
		m_titleOption->Draw();
	}
}

bool TitleScene::Release()
{


	return true;
}

