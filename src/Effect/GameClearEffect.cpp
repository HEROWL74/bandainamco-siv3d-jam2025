#include "GameClearEffect.hpp"
#include <Siv3D.hpp>

using namespace s3d;

// コンストラクタ
GameClearEffect::GameClearEffect(const Vec2& position, double lifeSpanSec)
	: EffectBase(position, lifeSpanSec)
{
	const int numSparkles = 200;
	const RectF screenRect = Scene::Rect();

	for (int32 i = 0; i < numSparkles; ++i)
	{
		const Vec2 pos = RandomVec2(screenRect);
		const double hue = Random(40.0, 60.0);

		m_sparkles << Sparkle{
			.position = pos,
			.startTime = Random(-0.8, 0.0),
			.scale = Random(0.5, 1.5),
			.color = HSV{ hue, 1.0, 1.0 },
			.lifeRatio = Random(0.8, 1.2),
		};
	}
}

void GameClearEffect::Update()
{
}

void GameClearEffect::Draw() const
{
	const double p = Progress();

	if (!IsAlive()) return;

	const double flashAlpha = Max(0.0, 0.5 - p * 5.0);
	Scene::Rect().draw(ColorF(1.0, 1.0, 0.8, flashAlpha));

	for (const auto& sparkle : m_sparkles)
	{
		const double t = (p + sparkle.startTime) / sparkle.lifeRatio;

		if (not InRange(t, 0.0, 1.0)) continue;

		const double brightness = (t < 0.5)
			? EaseOutQuad(t * 2.0)
			: 1.0 - EaseInQuad((t - 0.5) * 2.0);

		const double alpha = brightness;
		const double s = sparkle.scale * (1.0 + brightness * 0.5);

		const double angle = p * 180_deg;

		Shape2D::NStar(5, 12.0 * s, 4.0 * s, sparkle.position, angle)
			.draw(ColorF{ sparkle.color, alpha });
	}
	const double overallAlpha = Max(0.0, 1.0 - p);
}
