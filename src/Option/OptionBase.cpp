#include "OptionBase.hpp"


OptionBase::OptionBase()
	:m_isClosed(false)
	,m_roundRect(RoundRect{ Arg::center(400.0, 300.0), 500.0, 200.0, 6.0 })
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
}

void OptionBase::Update()
{
	// 描画された長方形を左クリックしたらゲーム画面に戻る
	if (m_roundRect.leftClicked())
		m_isClosed = true;
}

void OptionBase::Draw() const
{
	m_roundRect.draw(ColorF{ 0.2, 0.2, 0.2 });
	if (m_roundRect.mouseOver())
	{
		m_roundRect.drawShadow(Vec2{ 2, 2 }, 12, 1).draw(ColorF{ 0.1, 0.1, 0.1 });
	}
}

