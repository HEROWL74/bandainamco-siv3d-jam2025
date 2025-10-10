#pragma once
#include "AnimationInfo.hpp"
#include <unordered_map>
#include "MainCamera.hpp"

class CharacterBase
{
public:
	CharacterBase(const Texture& texture);

	virtual void Update();
	virtual void Draw(const MainCamera& camera) const;

	virtual void InitAnimation() {} // 継承先でアニメーション初期化
	void AddAnimation(const String& name, const Animation& anim); // アニメーション登録
	void PlayAnimation(const String& name); // アニメーション再生

	// 排他アニメーションの登録とチェック
	bool IsExclusiveAnimation(const String& name) const;

	// イベント状態の公開
	bool IsEvent() const { return m_isEvent; }
	void SetEvent(bool value) { m_isEvent = value; }

	// まだ未使用な部分が多い。将来的に使う予定
	// convertible_toは明示的に何初期化してるかを書くために使っている
	void SetPosition(const std::convertible_to<Vec2> auto& pos) { m_position = static_cast<Vec2>(pos); } // 位置設定
	void SetVelocity(const std::convertible_to<Vec2> auto& vel) { m_velocity = static_cast<Vec2>(vel); } // 速度設定
	[[nodiscard]]
	Vec2 GetPosition() const noexcept { return m_position; } // 位置取得
	[[nodiscard]]
	Vec2 GetVelocity() const noexcept { return m_velocity; } // 速度取得

protected:
	String m_currentAnimationName; // 現在のアニメーション名
	Vec2 m_position = Scene::Center(); // 位置
	Vec2 m_velocity = { 0, 0 }; // 速度ベクトル
	double m_height = 10.0; // Z軸
	int m_frameheight = 28;
	double m_moveSpeed = 2.0; // 移動速度
	virtual void OnAnimationEnd(const String& animName) {} // アニメーション終了時コールバック
private:

	Texture m_texture; // スプライトシート
	std::unordered_map<String, Animation> m_animations; // アニメーションリスト
	double m_animationTimer = 0.0;
	int m_currentFrame = 0;
	int m_framewidth = 20; //1枚のフレームの幅

	// 排他アニメーション管理
	bool m_isEvent = false;
};
