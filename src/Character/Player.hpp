#pragma once
#include "CharacterBase.hpp"
#include "MainCamera.hpp"

class Player : public CharacterBase
{
public:
	Player(const Texture& texture, std::shared_ptr<AudioManager> audio);

	void Update() override;
	void Init();

	void DrawCharacter(const MainCamera& camera) const;
	void DrawLight(const MainCamera& camera)const;

	void TryMove(const Array<RectF>& mapCollisions);

	Vec2 GetLightDirection() const;

private:
	void InitAnimation() override;
	void InitCollision();
	void OnAnimationEnd(const String& animName) override;
	// 排他アニメーション開始用
	void StartExclusiveAnimation(const String& animName);

	enum class Direction { Down, Up, Left, Right };
	Direction m_lastDirection = Direction::Down;

	RenderTexture m_lightMask{ Scene::Size() };
	double m_lightAngle = 65_deg;//懐中電灯の照射角
	double m_lightLength = 350.0f;//照射距離
	Vec2 m_lightDirection = Vec2{ 0, 1 };
	//ColorF m_lightColor = ColorF{ 0.5, 0.5, 1.0,0.9 }; //青色
	ColorF m_lightColor_Bright = ColorF{ 1.0, 1.0, 0.7,0.9 }; //黄色
	ColorF m_lightColor_Dim = ColorF{ 0.5, 0.5, 1.0,0.9 }; //青色

	std::shared_ptr<AudioManager> m_audio;
};
