#pragma once
#include <Siv3D.hpp>

class Piece
{
public:
	Piece(const Texture& t, const Vec2& correct, const Vec2& initial);

	void Draw(int32 tileSize, int32 piecePx) const;
	void StartDrag(const Vec2& mousePos);
	void DragTo(const Vec2& mousePos);
	void StopDrag();
	bool SnapToCorrect();
	bool IsNear(double range) const { return (m_pos - m_correctPos).length() <= range; }

	Texture m_tex;
	Vec2 m_correctPos{ 0, 0 };
	Vec2 m_pos{ 0, 0 };
	Vec2 m_grabOffset{ 0, 0 };
	bool m_isPlaced{ false };
	bool m_isDragging{ false };
};
