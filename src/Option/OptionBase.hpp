#pragma once
#include <Siv3D.hpp>
#include "Settings.hpp"
#include "AudioManager.hpp"
#include "../Core/Application.hpp"

class OptionBase
{
private:
	double m_sliderX;
	double m_sliderY;
	bool m_isClosed;

	RoundRect m_optionWindow;
	Polygon m_closeOptionBtn;
	Font m_closeMark;

	std::shared_ptr<Settings> m_settings;
	std::shared_ptr<AudioManager> m_audio;


protected:
	static constexpr double OPTION_WINDOW_X = Application::WINDOW_WIDTH / 10.0;
	static constexpr double OPTION_WINDOW_Y = Application::WINDOW_HEIGHT / 10.0;
	static constexpr double OPTION_WINDOW_WIDTH = Application::WINDOW_WIDTH * 8.0 / 10.0;
	static constexpr double OPTION_WINDOW_HEIGHT = Application::WINDOW_HEIGHT * 8.0 / 10.0;

public:
	OptionBase(std::shared_ptr<Settings> settings, std::shared_ptr<AudioManager> audio);
	virtual ~OptionBase() = default;

	virtual bool SystemInit();
	virtual void GameInit();
	virtual void Update();
	virtual void Draw() const;

	// 共通の関数
	bool IsClosed() const { return m_isClosed; }		// オプション画面を閉じた判定用

};
