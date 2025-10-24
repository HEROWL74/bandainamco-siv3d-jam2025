#include "BubbleEffect.hpp"
#include "../Character/MainCamera.hpp"
#include <Siv3D.hpp>

BubbleEffect::BubbleEffect(const Vec2& pos, double lifeSpanSec, double baseHue)
	: EffectBase(pos,lifeSpanSec)
{
	for (int32 i = 0; i < 8; ++i)
	{
		Bubble b{
			.offset = RandomVec2(Circle{ 30 }),
			.startTime = Random(-0.3, 0.1),
			.scale = Random(0.1, 1.2),
			.color = HSV{ baseHue + Random(-30.0, 30.0) }
		};
		m_bubbles << b;
	}
}

void BubbleEffect::Update()
{
	// 経過時間は EffectBase::Elapsed() から取得する
	const double t = Elapsed();
}

void BubbleEffect::Draw() const
{
	const double p = Progress();

	if (!IsAlive()) return;

	for (const auto& bubble : m_bubbles)
	{
		// 進行度 p を使用してアニメーションを制御
		const double t2 = bubble.startTime + p;
		if (not InRange(t2, 0.0, 1.0)) continue;

		const double e = EaseOutExpo(t2);

		// 泡の移動も進行度 p に依存させることで、ライフスパン全体で移動が完了するようにする
		const Vec2 center = m_position + bubble.offset + (bubble.offset * 4 * p);
		const double radius = (e * 40 * bubble.scale);

		Circle{ center, radius }
			.draw(ColorF{ bubble.color, 0.15 })
			.drawFrame((30.0 * (1.0 - e) * bubble.scale), bubble.color);
	}
}

void BubbleEffect::Draw(const MainCamera& camera) const
{
	const auto transformer = camera.GetTransformer();

	// 修正: ライフスパンに対する進行度 (0.0〜1.0) を取得
	const double p = Progress();

	if (!IsAlive()) return;

	for (const auto& bubble : m_bubbles)
	{
		const double t2 = bubble.startTime + p;
		if (not InRange(t2, 0.0, 1.0)) continue;

		// 描画ロジックは Draw() と同じでOK。
		// ScopedCamera2D が m_position をワールド座標として扱ってくれる。
		const double e = EaseOutExpo(t2);
		const Vec2 center = m_position + bubble.offset + (bubble.offset * 4 * p);
		const double radius = (e * 40 * bubble.scale);

		Circle{ center, radius }
			.draw(ColorF{ bubble.color, 0.15 })
			.drawFrame((30.0 * (1.0 - e) * bubble.scale), bubble.color);
	}
}
