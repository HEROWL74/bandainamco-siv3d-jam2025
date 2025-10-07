#pragma once
#include "AnimationInfo.hpp"
#include <unordered_map>

class CharacterBase
{
public:
	CharacterBase(const Texture& texture);

	virtual void Update();
	virtual void Draw() const;

	virtual void InitAnimation() {} // 継承先でアニメーション初期化
	void AddAnimation(const String& name, const Animation& anim); // アニメーション登録
	void PlayAnimation(const String& name); // アニメーション再生

	// まだ未使用な部分が多い。将来的に使う予定
	void SetPosition(const Vec2& pos) { m_position = pos; } // 位置設定
	void SetVelocity(const Vec2& vel) { m_velocity = vel; } // 速度設定
	Vec2 GetPosition() const { return m_position; } // 位置取得
	Vec2 GetVelocity() const { return m_velocity; } // 速度取得

protected:
	String m_currentAnimationName; // 現在のアニメーション名
	Vec2 m_position = Scene::Center(); // 位置
	Vec2 m_velocity = { 0, 0 }; // 速度ベクトル
	double m_moveSpeed = 2.0; // 移動速度
	virtual void OnAnimationEnd(const String& animName) {} // アニメーション終了時コールバック
private:

	Texture m_texture; // スプライトシート
	std::unordered_map<String, Animation> m_animations; // アニメーションリスト
	double m_animationTimer = 0.0;
	int m_currentFrame = 0;
	int m_framewidth = 20; //1枚のフレームの幅
	int m_frameheight = 28;
};
