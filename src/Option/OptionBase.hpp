#pragma once
#include <Siv3D.hpp>

class OptionBase
{
protected:
	bool m_isClosed;

	// 仮
	RoundRect m_roundRect;

public:
	OptionBase();
	virtual ~OptionBase() = default;

	virtual bool SystemInit();
	virtual void GameInit();
	virtual void Update();
	virtual void Draw() const;

	// 共通の関数
	bool IsClosed() const { return m_isClosed; }		// オプション画面を閉じた判定用

};
