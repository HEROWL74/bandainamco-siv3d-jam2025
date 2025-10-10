#include "OptionBase.hpp"
#include <Siv3D.hpp>

OptionBase::OptionBase()
	:m_isClosed(false)
{
	SystemInit();
	GameInit();
}

OptionBase::~OptionBase()
{
	Release();
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
	if (MouseR.down())
		m_isClosed = true;
}

void OptionBase::Draw() const
{
	RoundRect rect = RoundRect{ Arg::center(640.0, 360.0), 500.0, 200.0, 6.0 };
	rect.draw(ColorF{ 0.2, 0.2, 0.2 });
}

bool OptionBase::Release()
{


	return true;
}
