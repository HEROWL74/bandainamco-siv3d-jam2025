#pragma once
#include "CharacterBase.hpp"
#include "Player.hpp"
#include <deque> // std::deque を使用するために必要です

class Enemy : public CharacterBase
{
public:
	Enemy(const Texture& texture, const Player& player);

	void Update() override;
	void Draw() const override; // 描画関数にもデバッグ用の経路表示を追加します
	void InitAnimation() override;

private:
	static constexpr Rect ObstacleRect{ 400, 400, 700, 700 }; //障害物（汎用性のあるない）

	const Player& m_player; // プレイヤー参照
	void OnAnimationEnd(const String& animName) override;
	const double chaseDistance = 125.0; // 追尾距離

	std::deque<Vec2> m_path; // 経路上の目標地点
	double m_pathRecalculateTimer = 0.0; // パス再計算タイマー
	const double PATH_RECALCULATE_INTERVAL = 0.5; // 0.5秒ごとにパスを再計算

	std::deque<Vec2> FindPath(const Vec2& start, const Vec2& target);
};
