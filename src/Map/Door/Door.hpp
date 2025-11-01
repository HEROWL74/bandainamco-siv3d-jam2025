// Door/Door.hpp
#pragma once
#include <Siv3D.hpp>

class Door
{
public:
	Door(const Vec2& drawCenter, const Vec2& hitBoxCenter, const Vec2& hitBoxSize, const FilePath& texturePath);

	void update();
	void draw() const;

	[[nodiscard]]
	bool isNear(const Circle& playerCollision) const;

private:
	Vec2 m_drawCenterPos; // 描画の中心座標
	Vec2 m_drawSize;      // 描画したいサイズ (width, height)
	Texture m_texture;

	Vec2 m_hitBoxCenterPos; // 当たり判定の中心座標
	Vec2 m_hitBoxSize;      // 当たり判定のサイズ (width, height)
};
