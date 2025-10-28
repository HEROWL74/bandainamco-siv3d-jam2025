#include "BubbleEffect.hpp"
#include "../Character/MainCamera.hpp"
#include <Siv3D.hpp>

// コンストラクタ
BubbleEffect::BubbleEffect(const Vec2& pos, double lifeSpanSec, double baseHue)
	: EffectBase(pos, lifeSpanSec)
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
	// ここでは特に更新処理なし
	// const double t = Elapsed();
}

void BubbleEffect::Draw() const
{
	const double p = Progress();

	if (!IsAlive()) return;

	// サークルエフェクト用の時間 t (ライフスパンの半分 t=1.0 で終了)
	const double t = p * 2.0;

	if (t < 1.0) // t が 0.0 から 1.0 の間のみ描画
	{
		// フレーム幅を時間経過に応じて細くする
		const double frameWidth = 25.0 * (1.0 - t);

		if (frameWidth > 0)
		{
			// アルファ値: 1.0 から 0.5 に向かって線形補間
			const double alpha = 1.0 - t * 0.5;

			// 色を緑色に設定
			const ColorF green = ColorF(0.0, 1.0, 0.0, alpha);

			// 判定ラインの位置を中心としたサークルを描画
			Circle{ m_position, (30.0 + t * 80.0) }
				// フレーム描画
			.drawFrame(frameWidth, green);
		}
	}

	// 個別の泡（バブル）の描画
	for (const auto& bubble : m_bubbles)
	{
		// 進行度 p とバブル開始時間を使ってアニメーションを制御
		const double t2 = bubble.startTime + p;
		if (not InRange(t2, 0.0, 1.0)) continue;

		const double e = EaseOutExpo(t2);

		// 泡の移動
		const Vec2 center = m_position + bubble.offset + (bubble.offset * 4 * p);
		const double radius = (e * 30 * bubble.scale);

		Circle{ center, radius }
			.draw(ColorF{ bubble.color, 0.65 })
			.drawFrame((30.0 * (1.0 - e) * bubble.scale), bubble.color);
	}
}

// カメラあり描画
void BubbleEffect::Draw(const MainCamera& camera) const
{
	const auto transformer = camera.GetTransformer();

	// ライフスパンに対する進行度 (0.0〜1.0) を取得
	const double p = Progress();

	if (!IsAlive()) return;

	// 個別の泡（バブル）の描画
	for (const auto& bubble : m_bubbles)
	{
		const double t2 = bubble.startTime + p;
		if (not InRange(t2, 0.0, 1.0)) continue;

		// 描画ロジック
		const double e = EaseOutExpo(t2);
		const Vec2 center = m_position + bubble.offset + (bubble.offset * 4 * p);
		const double radius = (e * 40 * bubble.scale);

		Circle{ center, radius }
			.draw(ColorF{ bubble.color, 0.15 })
			.drawFrame((30.0 * (1.0 - e) * bubble.scale), bubble.color);
	}
}
