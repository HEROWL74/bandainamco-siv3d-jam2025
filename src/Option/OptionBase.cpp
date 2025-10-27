#include "OptionBase.hpp"


OptionBase::OptionBase(std::shared_ptr<Settings> settings, std::shared_ptr<AudioManager> audio)
	:m_settings(settings)
	,m_audio(audio)
	,m_isClosed(false)
{
	SystemInit();
	GameInit();
}


bool OptionBase::SystemInit()
{


	return true;
}

void OptionBase::GameInit()
{
	// オプション画面が閉じているかのフラグ
	m_isClosed = false;

	// オプション画面のウィンドウの位置とサイズ設定
	m_optionWindowSize = { Scene::Width() * 8.0 / 10.0 , Scene::Height() * 8.0 / 10.0 };										// 縦幅と横幅
	m_optionWindowPos = {(Scene::Width() - m_optionWindowSize.x) / 2.0, (Scene::Height() - m_optionWindowSize.y) / 2.0};		// 左上の座標
	const double round{ 6.0 };																									// 角の丸み
	m_optionWindow = RoundRect{ m_optionWindowPos, m_optionWindowSize, round };													// ウィンドウ（四角形）の定義

	// オプション画面の閉じるボタンの位置とサイズ設定
	m_optionCloseBtnSize = { m_optionWindowSize.x / 15.0 , m_optionWindowSize.y / 15.0 };										// 縦幅と横幅
	m_optionCloseBtnPos = { m_optionWindowPos.x + m_optionWindowSize.x - m_optionCloseBtnSize.x, m_optionWindowPos.y };			// 左上の位置
	RectF rect = RectF{ m_optionCloseBtnPos, m_optionCloseBtnSize };															// ボタンの定義
	m_optionCloseBtn = rect.rounded(0.0, round, 0.0, 0.0);																		// 右上の角を丸くする

	// 閉じるマークのフォント設定
	m_closeMark = Font{ FontMethod::MSDF, 30 };

	// オプション画面下方のボタンの位置とサイズ設定
	m_btnPos = { m_optionWindowPos.x + m_optionWindowSize.x / 2.0, Scene::Height() - m_optionWindowPos.y * 2.5 };				// 左上の位置
	m_btnSize = { m_optionWindowSize.x / 4.0, m_optionWindowSize.y / 5.0 };														// 縦幅と横幅
	RectF btn = RectF{ Arg::center(m_btnPos), m_btnSize };																		// ボタンの定義
	m_btn = btn.rounded(round);																									// 角を丸くする
}

void OptionBase::Update()
{
	// 閉じるボタンを左クリックしたらゲーム画面に戻る
	if (m_optionCloseBtn.leftClicked())
		m_isClosed = true;
}

void OptionBase::Draw() const
{
	// オプション画面のウィンドウの描画
	m_optionWindow.draw(ColorF{ 0.2, 0.2, 0.2 , 0.8 });

	// オプション画面の閉じるボタンの中心座標
	const double btnX = m_optionCloseBtnPos.x + m_optionCloseBtnSize.x / 2.0;
	const double btnY = m_optionCloseBtnPos.y + m_optionCloseBtnSize.y / 2.0;
	// オプション画面の閉じるボタンと×マークの描画
	m_optionCloseBtn.draw(ColorF{ 0.4, 0.4, 0.4 });
	m_closeMark(U"もどる").drawAt(btnX, btnY, ColorF{ Palette::Black });
	if (m_optionCloseBtn.mouseOver())
	{
		m_optionCloseBtn.draw(ColorF{ 0.6, 0.2, 0.2 });
		m_closeMark(U"もどる").drawAt(btnX, btnY, ColorF{ Palette::White });
	}

	// 位置の変数
	const double sliderX{ m_optionWindowPos.x + m_optionWindowSize.x / 5.0 };
	const double sliderY{ m_optionWindowPos.y + m_optionWindowSize.y / 7.0 };
	const double sliderGap{ m_optionWindowSize.y / 10.0 };
	const double sliderLabelW{ 150.0 };
	const double sliderW{ m_optionWindowPos.x + m_optionWindowSize.x / 4.0 };

	// 音量のスライダー
	if (SimpleGUI::Slider(U"Master Volume", m_settings->masterVolume, Vec2{ sliderX, sliderY }, sliderLabelW, sliderW))
	{
		m_audio->SetMasterVolume(m_settings->masterVolume);
	}
	if (SimpleGUI::Slider(U"BGM Volume", m_settings->bgmVolume, Vec2{ sliderX, sliderY + sliderGap }, sliderLabelW, sliderW))
	{
		m_audio->SetBusVolume(Bus::BGM, m_settings->bgmVolume);
	}
	if (SimpleGUI::Slider(U"SE Volume", m_settings->seVolume, Vec2{ sliderX, sliderY + sliderGap * 2.0}, sliderLabelW, sliderW))
	{
		m_audio->SetBusVolume(Bus::SE, m_settings->seVolume);
	}

	m_btn.draw(Palette::Red);
	if (m_btn.mouseOver())
	{
		m_btn.draw(Palette::Darkred);
	}
}

