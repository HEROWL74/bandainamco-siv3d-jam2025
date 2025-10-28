#include "Door.hpp"

Door::Door(const Vec2& drawCenter,
		 const Vec2& hitBoxCenter, const Vec2& hitBoxSize,
		 const FilePath& texturePath)
	: m_drawCenterPos(drawCenter)
	, m_hitBoxCenterPos(hitBoxCenter)
	, m_hitBoxSize(hitBoxSize)
	, m_texture(texturePath)
{
	if (!m_texture)
	{
		Logger << U"Door: テクスチャのロードに失敗しました。パス: {}"_fmt(texturePath);
	}
}

void Door::update()
{
	//アニメーションを追加する場合  
}

void Door::draw() const
{
	m_texture.scaled(20).drawAt(m_drawCenterPos);

#ifdef _DEBUG
	RectF(Arg::center(m_hitBoxCenterPos), m_hitBoxSize.x, m_hitBoxSize.y).drawFrame(3, ColorF{ 0.0, 1.0, 1.0, 0.7 });
#endif
}

bool Door::isNear(const Circle& playerCollision) const
{
	const RectF doorCheckArea = RectF(Arg::center(m_hitBoxCenterPos), m_hitBoxSize.x, m_hitBoxSize.y);

	// プレイヤーの円形コリジョンとドアの矩形領域が交差しているかチェック
	return playerCollision.intersects(doorCheckArea);
}
