#pragma once

#include <Siv3D.hpp>

// MainCamera はプロジェクト側で定義されている想定
class MainCamera;

class EffectBase : public s3d::IEffect
{
public:
	// ライフスパン (lifeSpanSec) を引数に追加
	EffectBase(const Vec2& position, double lifeSpanSec)
		: m_position(position)
		, m_lifeSpan(lifeSpanSec) // ライフスパンを初期化
		, m_isAlive(true)
		, m_startTime(Scene::Time())
		, m_elapsedTime(0.0)
	{
	}

	virtual ~EffectBase() = default;

	bool update(double timeSec) override
	{
		m_elapsedTime = timeSec;

		// ライフスパンによる自動破棄判定
		if (m_elapsedTime >= m_lifeSpan)
		{
			m_isAlive = false;
			return false; // 破棄
		}

		Update(); // 派生クラスの更新処理

		return m_isAlive; // true=継続, false=破棄
	}

	virtual void Update() = 0;

	// 描画（メインゲーム用: カメラ有り）
	virtual void Draw(const MainCamera& camera) const { Draw(); } // デフォルトはスクリーン座標版 Draw() を呼ぶ

	// 描画（ミニゲーム用: スクリーン座標）
	virtual void Draw() const = 0;

	// エフェクトの開始時刻
	double GetStartTime() const { return m_startTime; }

	// エフェクトの生存状態を外部から設定する関数を追加
	void SetAlive(bool alive) { m_isAlive = alive; }

	// 経過時間（IEffect::update でセットされる）
	double Elapsed() const { return m_elapsedTime; }

	// ライフスパン
	double LifeSpan() const { return m_lifeSpan; }

	// ライフスパンに対する進行度 (0.0～1.0)
	double Progress() const { return m_elapsedTime / m_lifeSpan; }

	bool IsAlive() const { return m_isAlive; }

protected:
	Vec2 m_position;
	double m_lifeSpan; // エフェクトの生存時間（秒）
	bool m_isAlive;

private:
	double m_startTime;
	double m_elapsedTime;
};
