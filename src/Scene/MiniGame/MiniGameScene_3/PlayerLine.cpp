#include "PlayerLine.hpp"

PlayerLine::PlayerLine()
{

}

void PlayerLine::GameInit()
{
	m_lines.clear();
	m_lastPoint = Cursor::Pos();
	m_prevPressed = false;
}

bool PlayerLine::Update(const Point& pos, double minDist)
{
	if (!MouseL.pressed()) return false;

	const bool currentPressed = MouseL.pressed();
	const bool downNow = (currentPressed && !m_prevPressed);

	if (MouseL.down())
	{
		// 新しいストローク開始
		m_lines << LineString{ pos };
		m_lastPoint = pos;
		m_prevPressed = true;

		return true;
	}

	if (MouseL.pressed() && !m_lines.isEmpty())
	{
		// 押しっぱなしで追加
		LineString& cur = m_lines.back();
		if (m_lastPoint.distanceFrom(pos) >= minDist)
		{
			cur << pos;
			m_lastPoint = pos;
			m_prevPressed = true;

			return true;
		}
	}

	if (MouseL.up())
	{
		m_prevPressed = false;
	}

	return false;
}

void PlayerLine::Draw() const
{
	for (const auto& line : m_lines)
	{
		if (!line.empty())
		{
			line.draw(10, HSV{ 10, 1.0, 0.95 });
		}
	}
}

void PlayerLine::LineClear()
{
	m_lines.clear();
	m_prevPressed = false;
}

LineString PlayerLine::GetCurrentLine() const
{
	if (m_lines.isEmpty())
	{
		return LineString();
	}
	return m_lines.back();
}

LineString PlayerLine::GetMergeLine() const
{
	LineString merged;
	merged.reserve(128);
	for (const auto& ls : m_lines)
	{
		for (const auto& p : ls)
		{
			merged << p;
		}
	}
	return merged;
}
