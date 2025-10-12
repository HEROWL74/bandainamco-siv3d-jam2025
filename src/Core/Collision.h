#pragma once
#include <Siv3D.hpp>

// キャラクターの円形コリジョン情報を保持する構造体
struct CharacterCollision
{
	// コリジョンの形状
	Circle shape;

	// キャラクターの中心座標(m_position)からのオフセット
	Vec2 offset = { 0, 0 };

	// ワールド座標でのコリジョン図形を取得するメソッド
	[[nodiscard]]
	Circle getWorldShape(const Vec2& characterWorldPos) const
	{
		return Circle{ characterWorldPos + offset, shape.r };
	}
};
