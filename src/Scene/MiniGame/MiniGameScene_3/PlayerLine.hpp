#pragma once
#include <Siv3D.hpp>

class PlayerLine
{
private:
	Array<LineString> m_lines;
	Point m_lastPoint;
	bool m_prevPressed;

public:
	PlayerLine();
	~PlayerLine() = default;

	void GameInit();
	bool Update(const Point& pos, double minDist);
	void Draw() const;

	void LineClear();
	const Array<LineString>& GetLine() const { return m_lines; }
	LineString GetCurrentLine() const;
	LineString GetMergeLine() const;
};
