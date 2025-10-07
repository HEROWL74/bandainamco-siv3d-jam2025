#include "Player.hpp"

Player::Player(const Texture& texture)
	: CharacterBase(texture)
{
}

void Player::Update()
{
	SetVelocity({ 0, 0 });

	// 方向ごとに加算
	if (KeyW.pressed()) m_velocity.y -= 1;
	if (KeyS.pressed()) m_velocity.y += 1;
	if (KeyA.pressed()) m_velocity.x -= 1;
	if (KeyD.pressed()) m_velocity.x += 1;


	if (m_velocity != Vec2{ 0, 0 })
	{
		// 左右キーを押しても上下キーのAnim優先 (逆にしても〇)
		if (m_velocity.y < 0)      PlayAnimation(AnimationKeys::Walk_U);
		else if (m_velocity.y > 0) PlayAnimation(AnimationKeys::Walk_D);
		else if (m_velocity.x < 0) PlayAnimation(AnimationKeys::Walk_L);
		else if (m_velocity.x > 0) PlayAnimation(AnimationKeys::Walk_R);

		// 移動
		SetVelocity(m_velocity.normalize() * m_moveSpeed);
	}
	else
	{
		if (m_currentAnimationName == AnimationKeys::Walk_U) PlayAnimation(AnimationKeys::Idle_U);
		else if (m_currentAnimationName == AnimationKeys::Walk_D) PlayAnimation(AnimationKeys::Idle_D);
		else if (m_currentAnimationName == AnimationKeys::Walk_L) PlayAnimation(AnimationKeys::Idle_L);
		else if (m_currentAnimationName == AnimationKeys::Walk_R) PlayAnimation(AnimationKeys::Idle_R);
	}

	ClearPrint();
	Print << m_currentAnimationName;

	CharacterBase::Update();
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

	// アニメーション登録
	AddAnimation(AnimationKeys::Idle_D, player_Idle_D);
	AddAnimation(AnimationKeys::Idle_U, player_Idle_U);
	AddAnimation(AnimationKeys::Idle_L, player_Idle_L);
	AddAnimation(AnimationKeys::Idle_R, player_Idle_R);

	AddAnimation(AnimationKeys::Walk_D, player_Walk_D);
	AddAnimation(AnimationKeys::Walk_U, player_Walk_U);
	AddAnimation(AnimationKeys::Walk_L, player_Walk_L);
	AddAnimation(AnimationKeys::Walk_R, player_Walk_R);

	// 初期アニメーション
	PlayAnimation(AnimationKeys::Idle_D);
}
