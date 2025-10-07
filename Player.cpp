#include "Player.hpp"

Player::Player(const Texture& texture)
	: CharacterBase(texture)
{
}

void Player::Update()
{
	// イベント中は他の入力を受け付けない(仮)
	if (m_isEvent)
	{
		CharacterBase::Update();
		return;
	}

	SetVelocity({ 0, 0 });

	// FキーでAttackアニメーション
	if (MouseL.down())
	{
		// 攻撃開始時に向きを記録
		if (m_currentAnimationName == AnimationKeys::Walk_U || m_currentAnimationName == AnimationKeys::Idle_U) m_lastDirection = Direction::Up;
		else if (m_currentAnimationName == AnimationKeys::Walk_D || m_currentAnimationName == AnimationKeys::Idle_D) m_lastDirection = Direction::Down;
		else if (m_currentAnimationName == AnimationKeys::Walk_L || m_currentAnimationName == AnimationKeys::Idle_L) m_lastDirection = Direction::Left;
		else if (m_currentAnimationName == AnimationKeys::Walk_R || m_currentAnimationName == AnimationKeys::Idle_R) m_lastDirection = Direction::Right;

		m_isEvent = true;
		PlayAnimation(AnimationKeys::Event);
		SetVelocity({ 0, 0 });
		CharacterBase::Update();
		return;
	}

	// 方向ごとに加算
	if (KeyW.pressed()) m_velocity.y -= 1;
	if (KeyS.pressed()) m_velocity.y += 1;
	if (KeyA.pressed()) m_velocity.x -= 1;
	if (KeyD.pressed()) m_velocity.x += 1;

	if (m_velocity != Vec2{ 0, 0 })
	{
		if (m_velocity.y < 0) { PlayAnimation(AnimationKeys::Walk_U); m_lastDirection = Direction::Up; }
		else if (m_velocity.y > 0) { PlayAnimation(AnimationKeys::Walk_D); m_lastDirection = Direction::Down; }
		else if (m_velocity.x < 0) { PlayAnimation(AnimationKeys::Walk_L); m_lastDirection = Direction::Left; }
		else if (m_velocity.x > 0) { PlayAnimation(AnimationKeys::Walk_R); m_lastDirection = Direction::Right; }

		SetVelocity(m_velocity.normalize() * m_moveSpeed);
	}
	else
	{
		if (m_currentAnimationName == AnimationKeys::Walk_U) { PlayAnimation(AnimationKeys::Idle_U); m_lastDirection = Direction::Up; }
		else if (m_currentAnimationName == AnimationKeys::Walk_D) { PlayAnimation(AnimationKeys::Idle_D); m_lastDirection = Direction::Down; }
		else if (m_currentAnimationName == AnimationKeys::Walk_L) { PlayAnimation(AnimationKeys::Idle_L); m_lastDirection = Direction::Left; }
		else if (m_currentAnimationName == AnimationKeys::Walk_R) { PlayAnimation(AnimationKeys::Idle_R); m_lastDirection = Direction::Right; }
	}

	ClearPrint();
	Print << m_currentAnimationName;

	CharacterBase::Update();
}

void Player::OnAnimationEnd(const String& animName)
{
	if (animName == AnimationKeys::Event)
	{
		m_isEvent = false;
		// イベント後はイベント前の向きに応じてIdleアニメーションに戻す
		switch (m_lastDirection)
		{
		case Direction::Up:
			PlayAnimation(AnimationKeys::Idle_U);
			break;
		case Direction::Down:
			PlayAnimation(AnimationKeys::Idle_D);
			break;
		case Direction::Left:
			PlayAnimation(AnimationKeys::Idle_L);
			break;
		case Direction::Right:
			PlayAnimation(AnimationKeys::Idle_R);
			break;
		}
	}
}

void Player::Draw() const
{
	CharacterBase::Draw();
}

void Player::InitAnimation()
{
	// アニメーション定義
	Animation player_Idle_D{ 1, 1, 0, {0.2} };
	Animation player_Idle_U{ 1, 1, 3, {0.2} };
	Animation player_Idle_L{ 1, 1, 1, {0.2} };
	Animation player_Idle_R{ 1, 1, 2, {0.2} };

	Animation player_Walk_D{ 0, 2, 0, {0.1, 0.1, 0.1} };
	Animation player_Walk_U{ 0, 2, 3, {0.1, 0.1, 0.1} };
	Animation player_Walk_L{ 0, 2, 1, {0.1, 0.1, 0.1} };
	Animation player_Walk_R{ 0, 2, 2, {0.1, 0.1, 0.1} };

	Animation player_Event{ 3, 5, 0, {0.1, 0.1, 0.1} };

	// アニメーション登録
	AddAnimation(AnimationKeys::Idle_D, player_Idle_D);
	AddAnimation(AnimationKeys::Idle_U, player_Idle_U);
	AddAnimation(AnimationKeys::Idle_L, player_Idle_L);
	AddAnimation(AnimationKeys::Idle_R, player_Idle_R);

	AddAnimation(AnimationKeys::Walk_D, player_Walk_D);
	AddAnimation(AnimationKeys::Walk_U, player_Walk_U);
	AddAnimation(AnimationKeys::Walk_L, player_Walk_L);
	AddAnimation(AnimationKeys::Walk_R, player_Walk_R);

	AddAnimation(AnimationKeys::Event, player_Event);

	// 初期アニメーション
	PlayAnimation(AnimationKeys::Idle_D);
}
