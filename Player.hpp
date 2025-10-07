#pragma once
#include "CharacterBase.hpp"

class Player : public CharacterBase
{
public:
	Player(const Texture& texture);

	void Update() override;
	void Draw() const override;

	void InitAnimation() override;

private:
	// イベントフラグ
	bool m_isEvent = false;
	void OnAnimationEnd(const String& animName) override;

	// 向き管理
	enum class Direction { Down, Up, Left, Right };
	Direction m_lastDirection = Direction::Down;
};
