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
	m_isClosed = false;
	m_optionWindow = RoundRect{ OPTION_WINDOW_X, OPTION_WINDOW_Y, OPTION_WINDOW_WIDTH, OPTION_WINDOW_HEIGHT, 6.0 };
		
	const double btnW = OPTION_WINDOW_WIDTH / 15.0;
	const double btnH = OPTION_WINDOW_HEIGHT / 15.0;
	const double btnX = OPTION_WINDOW_X + OPTION_WINDOW_WIDTH - btnW;
	const double btnY = OPTION_WINDOW_Y;
	RectF rect = RectF{ btnX, btnY, btnW, btnH };
	m_closeOptionBtn = rect.rounded(0.0, 6.0, 0.0, 0.0);

	m_closeMark = Font{ FontMethod::MSDF, 30 };
}

void OptionBase::Update()
{
	// 描画された長方形を左クリックしたらゲーム画面に戻る
	if (m_closeOptionBtn.leftClicked())
		m_isClosed = true;
}

void OptionBase::Draw() const
{
	m_optionWindow.draw(ColorF{ 0.2, 0.2, 0.2 , 0.8 });

	const double btnW = OPTION_WINDOW_WIDTH / 15.0;
	const double btnH = OPTION_WINDOW_HEIGHT / 15.0;
	const double btnX = OPTION_WINDOW_X + OPTION_WINDOW_WIDTH - btnW;
	const double btnY = OPTION_WINDOW_Y;
	// オプションウィンドウの×ボタン
	m_closeOptionBtn.draw(ColorF{ 0.4, 0.4, 0.4 });
	m_closeMark(U"×").drawAt(btnX + btnW / 2.0, btnY + btnH / 2.0, ColorF{ Palette::Black });
	if (m_closeOptionBtn.mouseOver())
	{
		m_closeOptionBtn.draw(ColorF{ 0.6, 0.2, 0.2 });
		m_closeMark(U"×").drawAt(btnX + btnW / 2.0, btnY + btnH / 2.0, ColorF{ Palette::White });
	}

	// 位置の変数
	const double sliderX = OPTION_WINDOW_X + OPTION_WINDOW_WIDTH / 5.0;
	const double sliderY = OPTION_WINDOW_Y + OPTION_WINDOW_HEIGHT / 7.0;
	const double sliderGap = 50.0;
	const double sliderLabelW = 150.0;
	const double sliderW = OPTION_WINDOW_X + OPTION_WINDOW_WIDTH / 5.0;
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
}

