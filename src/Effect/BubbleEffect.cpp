#include "BubbleEffect.hpp"
#include "../Character/MainCamera.hpp"
#include <Siv3D.hpp>

BubbleEffect::BubbleEffect(const Vec2& pos, double baseHue)
	: EffectBase(pos)
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

	// 寿命チェック
	if (t >= LIFETIME)
	{
		m_isAlive = false;
	}

	// ここに位置や進行に関する状態更新があれば書く（今回は描画側で t を参照するだけ）
}

void BubbleEffect::Draw() const
{
	const double t = Elapsed();

	if (!IsAlive()) return;

	for (const auto& bubble : m_bubbles)
	{
		const double t2 = bubble.startTime + t;
		if (not InRange(t2, 0.0, 1.0)) continue;

		const double e = EaseOutExpo(t2);
		const Vec2 center = m_position + bubble.offset + (bubble.offset * 4 * t);
		const double radius = (e * 40 * bubble.scale);

		Circle{ center, radius }
			.draw(ColorF{ bubble.color, 0.15 })
			.drawFrame((30.0 * (1.0 - e) * bubble.scale), bubble.color);
	}
}

void BubbleEffect::Draw(const MainCamera& camera) const
{
	// ★ カメラの座標変換を適用
	const auto transformer = camera.GetTransformer();

	const double t = Elapsed();

	if (!IsAlive()) return;

	for (const auto& bubble : m_bubbles)
	{
		const double t2 = bubble.startTime + t;
		if (not InRange(t2, 0.0, 1.0)) continue;

		// 描画ロジックは Draw() と同じでOK。
		// ScopedCamera2D が m_position をワールド座標として扱ってくれる。
		const double e = EaseOutExpo(t2);
		const Vec2 center = m_position + bubble.offset + (bubble.offset * 4 * t);
		const double radius = (e * 40 * bubble.scale);

		Circle{ center, radius }
			.draw(ColorF{ bubble.color, 0.15 })
			.drawFrame((30.0 * (1.0 - e) * bubble.scale), bubble.color);
	}
}
