// StarDustEffect.cpp

#include "StarDustEffect.hpp"
#include "../Character/MainCamera.hpp" 

// コンストラクタ
StarDustEffect::StarDustEffect(const Vec2& pos, double lifeSpanSec, double baseHue, double playerY)
	: EffectBase(pos, lifeSpanSec)
	, m_playerY(playerY) // プレイヤーのY座標を保持
{
	const int numDusts = 25; //

	// プレイヤーの中心座標の設定
	const Vec2 playerCenter = { 50, playerY };

	// ノーツ位置から地球中心への目標ベクトルを計算
	const Vec2 streamDirection = (playerCenter - pos).normalized();

	for (int32 i = 0; i < numDusts; ++i)
	{
		const Vec2 initialOffset = RandomVec2(Circle{ 30 }); // ノーツ周囲にランダム配置

		m_dusts << Dust{
			.startOffset = initialOffset,
			.endOffset = streamDirection * 20.0,
			.speed = Random(0.8, 1.2),
			.scale = Random(1.0, 1.5),
			.color = HSV{ baseHue + Random(-10.0, 10.0), 1.0, Random(0.8, 1.0) }
		};
	}
}

void StarDustEffect::Update()
{
}


void StarDustEffect::Draw() const
{
	const double p = Progress();
	if (!IsAlive()) return;

	// プレイヤーの中心座標の設定
	const Vec2 playerCenter = { 50, m_playerY };

	// ノーツ位置からプレイヤー中心への目標ベクトルを計算
	const Vec2 toPlayer = playerCenter - m_position;

	for (const auto& dust : m_dusts)
	{
		const double t = EaseInQuad(p * dust.speed);

		const Vec2 startPos = m_position + dust.startOffset;
		// 左向きに移動
		const Vec2 center = startPos + toPlayer * t;

		const double currentThickness = 3.0 * dust.scale * (1.0 - t);

		// ColorF のコンストラクタで透明度を再構築
		const ColorF drawColor = ColorF{ dust.color.r, dust.color.g, dust.color.b, 1.0 - t };

		// パーティクルがいた少し前の位置を計算 (軌跡の開始点)
		const double t_prev = Max(0.0, t - 0.05);
		const Vec2 prevCenter = startPos + toPlayer * t_prev;

		// 軌跡のある線として描画
		Line(prevCenter, center)
			.draw(currentThickness, drawColor);
	}
}

void StarDustEffect::Draw(const MainCamera& camera) const
{
	Draw();
}

void StarDustEffect::updateDusts() {}
