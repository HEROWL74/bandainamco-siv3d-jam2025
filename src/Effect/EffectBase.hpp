#pragma once

#include <Siv3D.hpp>

// MainCamera はプロジェクト側で定義されている想定
class MainCamera;

/// @brief エフェクトの基底クラス（IEffect 実装）
/// - update(double) は「更新のみ」を行い、描画は Draw 系で行う設計
class EffectBase : public s3d::IEffect
{
public:
	EffectBase(const Vec2& position)
		: m_position(position)
		, m_isAlive(true)
		, m_startTime(Scene::Time())
		, m_elapsedTime(0.0)
	{
	}

	virtual ~EffectBase() = default;

	// IEffect::update の実装（描画は行わない）
	bool update(double timeSec) override
	{
		m_elapsedTime = timeSec;
		Update(); // 派生クラスの更新処理
		return m_isAlive; // true=継続, false=破棄
	}

	// 派生クラスは更新ロジックをここに実装する
	virtual void Update() = 0;

	// 描画（メインゲーム用: カメラ有り）
	virtual void Draw(const MainCamera& camera) const { Draw(); } // デフォルトはスクリーン座標版 Draw() を呼ぶ

	// 描画（ミニゲーム用: スクリーン座標）
	virtual void Draw() const = 0;

	// エフェクトの開始時刻
	double GetStartTime() const { return m_startTime; }

	// 経過時間（IEffect::update でセットされる）
	double Elapsed() const { return m_elapsedTime; }

	bool IsAlive() const { return m_isAlive; }

protected:
	Vec2 m_position;
	bool m_isAlive;

private:
	double m_startTime;
	double m_elapsedTime;
};
