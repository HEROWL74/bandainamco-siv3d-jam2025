#pragma once
#include <Siv3D.hpp>

class PlayerLine
{
private:
	LineString m_line;
	Point m_lastPoint;

public:
	PlayerLine();
	~PlayerLine() = default;

	void GameInit();
	bool Update(const Point& pos, double minDist);
	void Draw() const;

	void LineClear();
	const LineString& GetLine() const { return m_line; }
};
