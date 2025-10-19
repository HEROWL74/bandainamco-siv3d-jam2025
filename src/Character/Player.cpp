#include "Player.hpp"

Player::Player(const Texture& texture)
	: CharacterBase(texture)
{
}

void Player::StartExclusiveAnimation(const String& animName)
{
	// 攻撃開始時に向きを記録
	if (m_currentAnimationName == AnimationKeys::Walk_U || m_currentAnimationName == AnimationKeys::Idle_U) m_lastDirection = Direction::Up;
	else if (m_currentAnimationName == AnimationKeys::Walk_D || m_currentAnimationName == AnimationKeys::Idle_D) m_lastDirection = Direction::Down;
	else if (m_currentAnimationName == AnimationKeys::Walk_L || m_currentAnimationName == AnimationKeys::Idle_L) m_lastDirection = Direction::Left;
	else if (m_currentAnimationName == AnimationKeys::Walk_R || m_currentAnimationName == AnimationKeys::Idle_R) m_lastDirection = Direction::Right;

	PlayAnimation(animName);
	SetVelocity(Vec2{ 0, 0 });
}

void Player::Update()
{
	// イベント中は移動不可
	if (IsEvent())
	{
		CharacterBase::Update();
		return;
	}

	// 移動ベクトル初期化
	SetVelocity(Vec2{ 0, 0 });

	// Eキーでイベントアニメーション開始
	if (KeyE.down())
	{
		StartExclusiveAnimation(AnimationKeys::Event_0);
		CharacterBase::Update();
		return; // アニメーション時中は他の入力を無視
	}

	// Qキーでイベントアニメーション開始
	if (KeyQ.down())
	{
		StartExclusiveAnimation(AnimationKeys::Event_1);
		CharacterBase::Update();
		return;
	}

	// 移動入力
	if (KeyW.pressed()) m_velocity.y -= 1;
	if (KeyS.pressed()) m_velocity.y += 1;
	if (KeyA.pressed()) m_velocity.x -= 1;
	if (KeyD.pressed()) m_velocity.x += 1;

	// 移動ベクトルに応じてアニメーション切り替え
	if (m_velocity != Vec2{ 0, 0 })
	{
		if (m_velocity.y < 0) { PlayAnimation(AnimationKeys::Walk_U); m_lastDirection = Direction::Up; }
		else if (m_velocity.y > 0) { PlayAnimation(AnimationKeys::Walk_D); m_lastDirection = Direction::Down; }
		else if (m_velocity.x < 0) { PlayAnimation(AnimationKeys::Walk_L); m_lastDirection = Direction::Left; }
		else if (m_velocity.x > 0) { PlayAnimation(AnimationKeys::Walk_R); m_lastDirection = Direction::Right; }

		SetVelocity(m_velocity.normalize() * m_moveSpeed);
	}
	else // 停止時
	{
		if (m_currentAnimationName == AnimationKeys::Walk_U) { PlayAnimation(AnimationKeys::Idle_U); m_lastDirection = Direction::Up; }
		else if (m_currentAnimationName == AnimationKeys::Walk_D) { PlayAnimation(AnimationKeys::Idle_D); m_lastDirection = Direction::Down; }
		else if (m_currentAnimationName == AnimationKeys::Walk_L) { PlayAnimation(AnimationKeys::Idle_L); m_lastDirection = Direction::Left; }
		else if (m_currentAnimationName == AnimationKeys::Walk_R) { PlayAnimation(AnimationKeys::Idle_R); m_lastDirection = Direction::Right; }
	}

	//ライト方向計算
	Vec2 lightDir{};
	
	switch (m_lastDirection)
	{
	case Direction::Up:
		lightDir = Vec2{ 0,-1 };
		break;
	case Direction::Down:
		lightDir = Vec2{ 0,1 };
		break;
	case Direction::Right:
		lightDir = Vec2{ 1,0 };
		break;
	case Direction::Left:
		lightDir = Vec2{ -1,0 };
		break;
	}
	m_lightDirection = lightDir;

	CharacterBase::Update();
}

void Player::OnAnimationEnd(const String& animName)
{
	// アニメーション終了時
	if (IsExclusiveAnimation(animName))
	{
		// アニメーション終了時にイベントフラグOFF
		SetEvent(false);

		// 向きに応じてIdleアニメーションに戻す
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

void Player::DrawCharacter(const MainCamera& camera) const
{
	CharacterBase::Draw(camera);
}

void Player::DrawLight(const MainCamera& camera) const
{	
	Vec2 playerScreenPos = camera.GetViewMatrix().transformPoint(m_position); // プレイヤーに追従
	const Vec2 lightCenterPos = Scene::Center(); // 画面中央に固定
	const double maxRadius = Scene::Size().length() / 4.0; // 画面全体を覆う半径

	// 懐中電灯の方向計算
	Vec2 dir = m_lightDirection; 
	Vec2 projectedDir = dir.normalized(); 
	const double theta = Math::Atan2(projectedDir.x, -projectedDir.y);
	const double length = m_lightLength * 0.8;
	const double angle = m_lightAngle;

	// 懐中電灯と手元光の描画位置補正
	playerScreenPos.y += 40; // 垂直オフセット
	//playerScreenPos += m_lightDirection * 20.0; // 水平オフセット

	// 画面全体の薄い光
	{
		// 光の強さ: 薄い黄色 (0.1)
		const ColorF centerColor = ColorF(m_lightColor_Dim, 0.1);
		const ColorF outerColor = ColorF{ 0.0 }; // 完全に透明

		// 画面全体を覆う大きな円
		Circle{ lightCenterPos, maxRadius }
		.draw(centerColor, outerColor);
	}


	// プレイヤー周囲の円形の光
	{
		// 光の強さと範囲
		constexpr double MAX_RADIUS = 120;
		constexpr double MAX_ALPHA = 0.25;

		const ColorF centerColor = ColorF(m_lightColor_Bright, MAX_ALPHA);
		const ColorF outerColor = ColorF{ 0.0 };

		Circle{ playerScreenPos, MAX_RADIUS }
		.draw(centerColor, outerColor);
	}

	// 懐中電灯の光
	{
		// 光の強さと範囲
		const double MAX_LENGTH = length * 1.0;
		constexpr double MAX_ALPHA = 0.25;

		const ColorF centerColor = ColorF(m_lightColor_Bright, MAX_ALPHA);
		const ColorF outerColor = ColorF{ 0.0 };

		Circle{ playerScreenPos, MAX_LENGTH }
			.drawPie(theta - angle / 2, angle,
						 centerColor, outerColor);
	}
}

void Player::TryMove(const Array<RectF>& mapCollisions)
{
	if (IsEvent()) return;

	Vec2 currentPos = m_position;
	Vec2 nextVelocity = m_velocity;
	Vec2 nextPosX = currentPos + Vec2{ nextVelocity.x, 0.0 };
	Circle nextPlayerColX = GetCollision().getWorldShape(nextPosX);

	bool collideX = false;
	for (const auto& mapCol : mapCollisions)
	{
		if (nextPlayerColX.intersects(mapCol))
		{
			collideX = true;
			break;
		}
	}
	if (!collideX)
	{
		currentPos.x = nextPosX.x;
	}

	Vec2 nextPosY = currentPos + Vec2{ 0.0, nextVelocity.y };
	Circle nextPlayerColY = GetCollision().getWorldShape(nextPosY);

	bool collideY = false;
	for (const auto& mapCol : mapCollisions)
	{
		if (nextPlayerColY.intersects(mapCol))
		{
			collideY = true;
			break;
		}
	}
	if (!collideY)
	{
		currentPos.y = nextPosY.y;
	}

	SetPosition(currentPos);
	SetVelocity(Vec2{ 0, 0 });
}


Vec2 Player::GetLightDirection() const
{
	switch (m_lastDirection)
	{
	case Direction::Up:    return Vec2{ 0, -1 };
	case Direction::Down:  return Vec2{ 0,  1 };
	case Direction::Left:  return Vec2{ -1, 0 };
	case Direction::Right: return Vec2{ 1, 0 };
	default: return Vec2{ 0, 1 };
	}
}

void Player::InitAnimation()
{
	Animation player_Idle_D{ 2, 2, 0, {0.2} };
	Animation player_Idle_U{ 2, 2, 3, {0.2} };
	Animation player_Idle_L{ 2, 2, 1, {0.2} };
	Animation player_Idle_R{ 2, 2, 2, {0.2} };

	Animation player_Walk_D{ 0, 7, 0, {0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1} };
	Animation player_Walk_U{ 0, 7, 3, {0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1} };
	Animation player_Walk_L{ 0, 7, 1, {0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1} };
	Animation player_Walk_R{ 0, 7, 2, {0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1} };

	Animation player_Event_0{ 3, 5, 0, {0.1, 0.1, 0.1}, true };
	Animation player_Event_1{ 3, 5, 3, {0.1, 0.1, 0.1}, true };

	AddAnimation(AnimationKeys::Idle_D, player_Idle_D);
	AddAnimation(AnimationKeys::Idle_U, player_Idle_U);
	AddAnimation(AnimationKeys::Idle_L, player_Idle_L);
	AddAnimation(AnimationKeys::Idle_R, player_Idle_R);

	AddAnimation(AnimationKeys::Walk_D, player_Walk_D);
	AddAnimation(AnimationKeys::Walk_U, player_Walk_U);
	AddAnimation(AnimationKeys::Walk_L, player_Walk_L);
	AddAnimation(AnimationKeys::Walk_R, player_Walk_R);

	AddAnimation(AnimationKeys::Event_0, player_Event_0);
	AddAnimation(AnimationKeys::Event_1, player_Event_1);

	PlayAnimation(AnimationKeys::Idle_D);
}

void Player::InitCollision()
{
	CharacterCollision playerCol;
	playerCol.shape = Circle{ 0,0,30 };
	playerCol.offset = Vec2{ 0,m_frameheight * 8 / 2 - playerCol.shape.r };
	SetCollision(playerCol);
}

void Player::Init()
{
	InitAnimation();
	InitCollision();
}
