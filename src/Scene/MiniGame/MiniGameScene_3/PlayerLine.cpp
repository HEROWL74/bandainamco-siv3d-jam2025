#include "PlayerLine.hpp"

PlayerLine::PlayerLine()
{

}

void PlayerLine::GameInit()
{
	m_line.clear();
	m_lastPoint = Cursor::Pos();
}

bool PlayerLine::Update(const Point& pos, double minDist)
{
	if (!MouseL.pressed()) return false;

	if (m_line.empty())
	{
		// 最初の点は、押し始めのフレームで追加
		if (MouseL.down())
		{
			m_line << pos;
			m_lastPoint = pos;

			return true;
		}
		return false;
	}
	else
	{
		const double dist = m_lastPoint.distanceFrom(pos);
		if (dist >= minDist)
		{
			m_line << pos;
			m_lastPoint = pos;

			return true;
		}
	}

	return false;
}

void PlayerLine::Draw() const
{
	if (!m_line.empty())
	{
		m_line.draw(10, HSV{ 10, 1.0, 0.95 });
	}
}

void PlayerLine::LineClear()
{
	m_line.clear();
}
