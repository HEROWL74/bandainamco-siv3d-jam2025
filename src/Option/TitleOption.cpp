#include "TitleOption.hpp"

TitleOption::TitleOption(std::shared_ptr<Settings> settings, std::shared_ptr<AudioManager> audio)
	:OptionBase(settings, audio)
	,m_isExit(false)
{
	GameInit();
}

TitleOption::~TitleOption()
{

}

void TitleOption::GameInit()
{
	m_exit = Font{ FontMethod::MSDF, 30 };

	m_isExit = false;
}

void TitleOption::Update()
{
	OptionBase::Update();		// 基底クラスの処理

	// ボタンを押したらタイトルに戻る
	if (OptionBase::m_btn.leftClicked())
	{
		m_isExit = true;
	}
}

void TitleOption::Draw() const
{
	OptionBase::Draw();			// 基底クラスの描画

	// ボタンの所に文字を描画
	const Vec2 btnPos = OptionBase::m_btnPos;
	m_exit(U"ゲームをおわる").drawAt(btnPos, ColorF{ Palette::White });
}
