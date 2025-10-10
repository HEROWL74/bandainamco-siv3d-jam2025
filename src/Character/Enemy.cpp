#include "Enemy.hpp"

// コンストラクタ
Enemy::Enemy(const Texture& texture, const Player& player)
	: CharacterBase(texture)
	, m_player(player) // プレイヤーの参照を初期化
{
	// 移動速度をプレイヤーとは別に設定できます
	m_moveSpeed = 1.5;
}

// ----------------------------------------------------
// 更新処理
// ----------------------------------------------------
void Enemy::Update()
{
	// イベント中は移動・操作不可（CharacterBaseの機能を利用）
	if (IsEvent())
	{
		CharacterBase::Update();
		return;
	}

	// プレイヤーの位置を取得
	const Vec2 playerPos = m_player.GetPosition();
	// 自身の位置を取得
	const Vec2 enemyPos = GetPosition();

	// プレイヤーに向かうベクトルを計算
	Vec2 direction = playerPos - enemyPos;

	// プレイヤーとの距離が一定以上（例: 5.0ピクセル）離れている場合のみ移動
	if (direction.lengthSq() > 1000.0 * 10.0)
	{
		// 移動ベクトルを正規化し、移動速度を適用
		SetVelocity(direction.normalize() * m_moveSpeed);

		// アニメーションの切り替え
		// 追尾方向に応じてWalkアニメーションを再生します
		if (std::abs(direction.x) > std::abs(direction.y))
		{
			// X方向への移動が大きい
			if (direction.x > 0) PlayAnimation(AnimationKeys::Walk_R);
			else PlayAnimation(AnimationKeys::Walk_L);
		}
		else
		{
			// Y方向への移動が大きい（または同等）
			if (direction.y > 0) PlayAnimation(AnimationKeys::Walk_D);
			else PlayAnimation(AnimationKeys::Walk_U);
		}
	}
	else // プレイヤーに十分に近づいたら停止し、Idleアニメーションに戻る
	{
		SetVelocity(Vec2{ 0, 0 });

		// 現在のWalkアニメーションに応じてIdleアニメーションに切り替え
		if (m_currentAnimationName == AnimationKeys::Walk_U) PlayAnimation(AnimationKeys::Idle_U);
		else if (m_currentAnimationName == AnimationKeys::Walk_D) PlayAnimation(AnimationKeys::Idle_D);
		else if (m_currentAnimationName == AnimationKeys::Walk_L) PlayAnimation(AnimationKeys::Idle_L);
		else if (m_currentAnimationName == AnimationKeys::Walk_R) PlayAnimation(AnimationKeys::Idle_R);
		else if (m_currentAnimationName.starts_with(U"walk_")) // 停止したがIdleアニメーションが設定されていなかった場合の処理
		{
			// 最後に再生していたWalkアニメーションに対応するIdleを探す (簡単な例)
			if (m_currentAnimationName == AnimationKeys::Walk_U) PlayAnimation(AnimationKeys::Idle_U);
			// ... 他の方向についても同様に処理 ...
		}
	}

	// CharacterBaseのUpdateを呼び出し、位置の更新とアニメーションのフレーム更新を行う
	CharacterBase::Update();
}

// ----------------------------------------------------
// 描画処理
// ----------------------------------------------------
void Enemy::Draw() const
{
	CharacterBase::Draw();
}

// ----------------------------------------------------
// アニメーション初期化処理 (プレイヤーと同じスプライトシートを想定)
// ----------------------------------------------------
void Enemy::InitAnimation()
{
	// プレイヤーと同様のアニメーション定義を再利用（または独自のアニメーションを定義）
	// 敵キャラ専用のアニメーションがある場合は、ここに追加します。

	Animation enemy_Idle_D{ 1, 1, 0, {0.2} };
	Animation enemy_Idle_U{ 1, 1, 3, {0.2} };
	Animation enemy_Idle_L{ 1, 1, 1, {0.2} };
	Animation enemy_Idle_R{ 1, 1, 2, {0.2} };

	Animation enemy_Walk_D{ 0, 2, 0, {0.1, 0.1, 0.1} };
	Animation enemy_Walk_U{ 0, 2, 3, {0.1, 0.1, 0.1} };
	Animation enemy_Walk_L{ 0, 2, 1, {0.1, 0.1, 0.1} };
	Animation enemy_Walk_R{ 0, 2, 2, {0.1, 0.1, 0.1} };

	AddAnimation(AnimationKeys::Idle_D, enemy_Idle_D);
	AddAnimation(AnimationKeys::Idle_U, enemy_Idle_U);
	AddAnimation(AnimationKeys::Idle_L, enemy_Idle_L);
	AddAnimation(AnimationKeys::Idle_R, enemy_Idle_R);

	AddAnimation(AnimationKeys::Walk_D, enemy_Walk_D);
	AddAnimation(AnimationKeys::Walk_U, enemy_Walk_U);
	AddAnimation(AnimationKeys::Walk_L, enemy_Walk_L);
	AddAnimation(AnimationKeys::Walk_R, enemy_Walk_R);

	// 初期アニメーションを設定
	PlayAnimation(AnimationKeys::Idle_D);
}

// ----------------------------------------------------
// アニメーション終了時のコールバック
// ----------------------------------------------------
void Enemy::OnAnimationEnd(const String& animName)
{
	// 現状、敵の排他アニメーションは定義されていないため、このメソッドは特に処理を記述しません。
	// 必要に応じて、Player::OnAnimationEnd のように排他アニメーション終了時の処理を記述します。
	if (IsExclusiveAnimation(animName))
	{
		SetEvent(false);
		// 必要に応じてIdleアニメーションに戻す処理などを追加
	}
}
