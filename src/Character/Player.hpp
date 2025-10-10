#pragma once
#include "CharacterBase.hpp"
#include "MainCamera.hpp"

class Player : public CharacterBase
{
public:
	Player(const Texture& texture);

	void Update() override;
	void InitAnimation() override;
	void DrawCharacter(const MainCamera& camera) const;
	void DrawLight(const MainCamera& camera)const;

	Vec2 GetLightDirection() const;

private:
	void OnAnimationEnd(const String& animName) override;
	// 排他アニメーション開始用
	void StartExclusiveAnimation(const String& animName);

	enum class Direction { Down, Up, Left, Right };
	Direction m_lastDirection = Direction::Down;

	RenderTexture m_lightMask{ Scene::Size() };
	double m_lightAngle = 65_deg;//懐中電灯の照射角
	double m_lightLength = 350.0f;//照射距離
	Vec2 m_lightDirection = Vec2{ 0, 1 };
	ColorF m_lightColor = ColorF{ 1.0,1.0,0.8,0.9 }; //黄色みがある色
};
