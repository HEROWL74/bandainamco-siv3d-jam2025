#pragma once

#include "EffectBase.hpp"
#include <Siv3D.hpp>

class MainCamera;

/// @brief ノーツ吸収時の星屑エフェクト（エナジーストリーム）
class StarDustEffect : public EffectBase
{
private:
	struct Dust
	{
		Vec2 startOffset;
		Vec2 endOffset;
		double speed;
		double scale;
		ColorF color;
	};

	Array<Dust> m_dusts;

	// プレイヤーのY座標
	double m_playerY = 0.0;

public:
	StarDustEffect(const Vec2& pos, double lifeSpanSec, double baseHue, double playerY);

	void Update() override;
	void Draw() const override;
	void Draw(const MainCamera& camera) const override;

private:
	void updateDusts();
};
