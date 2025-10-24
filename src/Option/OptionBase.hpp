#pragma once
#include <Siv3D.hpp>
#include "Settings.hpp"
#include "AudioManager.hpp"
#include "../Core/Application.hpp"


class OptionBase
{
private:
	bool m_isClosed;							// オプション画面の開閉のフラグ

	Vec2 m_optionWindowPos;						// オプション画面のウィンドウの左上の座標
	Vec2 m_optionWindowSize;					// オプション画面のウィンドウの縦と横のサイズ
	RoundRect m_optionWindow;					// オプション画面のウィンドウ
	Vec2 m_optionCloseBtnPos;					// オプション画面の閉じるボタンの左上の座標
	Vec2 m_optionCloseBtnSize;					// オプション画面の閉じるボタンの縦と横のサイズ
	Polygon m_optionCloseBtn;					// オプション画面の閉じるボタン
	Font m_closeMark;							// オプション画面の×マーク

	std::shared_ptr<Settings> m_settings;
	std::shared_ptr<AudioManager> m_audio;

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
