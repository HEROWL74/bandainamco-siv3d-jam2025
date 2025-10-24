#pragma once

#include "EffectBase.hpp"
#include <Siv3D.hpp>

class MainCamera;

/// @brief 泡エフェクト（例）
/// - Update()/Draw() は EffectBase の契約に従う
class BubbleEffect : public EffectBase
{
private:
	struct Bubble
	{
		Vec2 offset;
		double startTime;
		double scale;
		ColorF color;
	};

	Array<Bubble> m_bubbles;
	static constexpr double LIFETIME = 0.3;

public:
	BubbleEffect(const Vec2& pos, double baseHue);

	void Update() override;
	void Draw() const override;
	void Draw(const MainCamera& camera) const override;
};
