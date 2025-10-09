#pragma once
#include "CharacterBase.hpp"
#include "MainCamera.hpp"

class Player : public CharacterBase
{
public:
	Player(const Texture& texture);

	void Update() override;
	void Draw() const override;
	void InitAnimation() override;

	MainCamera& GetCamera() { return m_camera; }
	MainCamera GetCamera() const { return m_camera; }
private:
	void OnAnimationEnd(const String& animName) override;

	// 排他アニメーション開始用
	void StartExclusiveAnimation(const String& animName);

	enum class Direction { Down, Up, Left, Right };
	Direction m_lastDirection = Direction::Down;

	MainCamera m_camera;
};
