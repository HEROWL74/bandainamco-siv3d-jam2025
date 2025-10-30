#include "GameOption.hpp"

GameOption::GameOption(std::shared_ptr<Settings> settings, std::shared_ptr<AudioManager> audio)
	:OptionBase(settings, audio)
	,m_isTitle(false)
{
	GameInit();
}

GameOption::~GameOption()
{

}

void GameOption::GameInit()
{
	m_returnTitle = Font{ FontMethod::MSDF, 30 };

	m_isTitle = false;
}

void GameOption::Update()
{
	OptionBase::Update();

	// ボタンを押したらタイトルに戻る
	if (OptionBase::m_btn.leftClicked())
	{
		m_isTitle = true;
	}
}

void GameOption::Draw() const
{
	OptionBase::Draw();

	// ボタンの所に文字を描画
	const Vec2 btnPos = OptionBase::m_btnPos;
	m_returnTitle(U"タイトルにもどる").drawAt(btnPos, ColorF{Palette::White});
}
