#include "Piece.hpp"

Piece::Piece(const Texture& t, const Vec2& correct, const Vec2& initial)		// 初期化用コンストラクタ
	:m_tex(t)
	, m_correctPos(correct)
	, m_pos(initial)
{

}

void Piece::Draw(int32 tileSize, int32 piecePx) const					// 描画サイズに合わせて描画
{
	// ピース画像を tileSize に合わせて中心に描く
	const double scale = tileSize / static_cast<double>(piecePx);
	m_tex.scaled(scale).drawAt(m_pos);
}

// 掴み開始
void Piece::StartDrag(const Vec2& mousePos)
{
	m_isDragging = true;
	m_grabOffset = mousePos - m_pos;
}

// ドラッグ中
void Piece::DragTo(const Vec2& mousePos)
{
	if (m_isDragging)
	{
		m_pos = mousePos - m_grabOffset;
	}
}

// ドラッグ終了
void Piece::StopDrag()
{
	m_isDragging = false;
}

bool Piece::SnapToCorrect()
{
	// 既に配置済みなら何もしない
	if (m_isPlaced)
	{
		return false;
	}

	m_pos = m_correctPos;
	m_isPlaced = true;
	m_isDragging = false;

	// スナップ成功を返す
	return true;
}

