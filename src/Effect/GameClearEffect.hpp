#pragma once

#include "EffectBase.hpp"
#include <Siv3D.hpp>

class MainCamera;

/// @brief ゲームクリア時のキラキラエフェクト
class GameClearEffect : public EffectBase
{
private:
	// パーティクル構造体
	struct Sparkle
	{
		Vec2 position;// 画面上の初期位置
		double startTime;// アニメーション開始遅延時間
		double scale;// サイズ
		ColorF color;// 色
		double lifeRatio;// 寿命に対するアニメーション比率
	};

	Array<Sparkle> m_sparkles;

public:
	GameClearEffect(const Vec2& position, double lifeSpanSec);

	void Update() override;
	void Draw() const override;
};
