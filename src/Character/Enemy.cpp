#include "Enemy.hpp"

// コンストラクタ
Enemy::Enemy(const Texture& texture, const Player& player)
	: CharacterBase(texture)
	, m_player(player)
{
	m_moveSpeed = 1.5;
}

void Enemy::Update()
{
	if (IsEvent())
	{
		CharacterBase::Update();
		return;
	}

	// パス再計算タイマーを更新
	m_pathRecalculateTimer += Scene::DeltaTime();

	// **パスの再計算** (0.5秒ごと)
	//if (m_pathRecalculateTimer >= PATH_RECALCULATE_INTERVAL)
	//{
	//m_pathRecalculateTimer = 0.0;
	m_path = FindPath(GetPosition(), m_player.GetPosition());
	//}

	// パスがない（初回のみ）か、追尾距離内に入ったら停止
	if (m_path.empty() || (m_player.GetPosition() - GetPosition()).length() <= chaseDistance)
	{
		SetVelocity(Vec2{ 0, 0 });
	}
	else
	{
		// パス上の次の目標地点を取得
		Vec2 nextTarget = m_path.front();

		// 現在のノードに十分に近づいたら、次のノードへ進む
		double arrivalThreshold = (m_path.size() > 1) ? 10.0 : 5.0;

		if ((nextTarget - GetPosition()).length() < arrivalThreshold)
		{
			// 目標地点に到達したので、パスから削除
			m_path.pop_front();

			if (m_path.empty())
			{
				SetVelocity(Vec2{ 0, 0 });
				CharacterBase::Update();
				return;
			}

			// 新しい目標を設定
			nextTarget = m_path.front();
		}

		// 新しい目標地点に向かうベクトルを計算
		Vec2 direction = nextTarget - GetPosition();

		// 移動速度を適用
		SetVelocity(direction.normalize() * m_moveSpeed);

		// アニメーションの切り替え
		if (std::abs(direction.x) > std::abs(direction.y))
		{
			// X方向への移動が大きい
			if (direction.x > 0) PlayAnimation(AnimationKeys::Walk_R);
			else PlayAnimation(AnimationKeys::Walk_L);
		}
		else
		{
			// Y方向への移動が大きい
			if (direction.y > 0) PlayAnimation(AnimationKeys::Walk_D);
			else PlayAnimation(AnimationKeys::Walk_U);
		}
	}

	CharacterBase::Update();
}

void Enemy::Draw() const
{
	CharacterBase::Draw();

	// 計算された経路を線で描画 (デバッグ用)
	if (m_path.size() >= 1)
	{
		Vec2 current = GetPosition();
		for (const auto& next : m_path)
		{
			Line(current, next).draw(2.0, Palette::Red); // Enemyから次の目標地点までの線
			Circle(next, 5).draw(Palette::Orange); // 目標地点
			current = next;
		}
	}
}

void Enemy::InitAnimation()
{
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

	PlayAnimation(AnimationKeys::Idle_D);
}

// アニメーション終了時のコールバック
void Enemy::OnAnimationEnd(const String& animName)
{
	if (IsExclusiveAnimation(animName))
	{
		SetEvent(false);
	}
}

// 経路探索の実装 (簡易版：直線と矩形の交差判定のみ)
std::deque<Vec2> Enemy::FindPath(const Vec2& start, const Vec2& target)
{
	// EnemyとPlayerを結ぶ線分と障害物を定義
	const Line directLine(start, target);
	const Rect obstacle = ObstacleRect;
	const double safeMargin = 20.0; // 障害物から離れるためのマージン

	// 障害物をマージン分拡大した矩形と直線が交差するかを判定
	if (directLine.intersects(obstacle.stretched(safeMargin)))
	{
		// 障害物に衝突する可能性があるため、回り込み経路を計算

		const Vec2 corners[] = {
			obstacle.tl() + Vec2(-safeMargin, -safeMargin), // 左上 外側
			obstacle.tr() + Vec2(safeMargin, -safeMargin), // 右上 外側
			obstacle.bl() + Vec2(-safeMargin,  safeMargin), // 左下 外側
			obstacle.br() + Vec2(safeMargin,  safeMargin)  // 右下 外側
		};

		double minCost = std::numeric_limits<double>::max();
		Vec2 bestCorner = target;

		//「スタート→候補点→ゴール」の総距離が最も小さい候補点を見つける
		for (const auto& corner : corners)
		{
			double cost = (corner - start).lengthSq() + (target - corner).lengthSq();
			if (cost < minCost)
			{
				minCost = cost;
				bestCorner = corner;
			}
		}

		return { bestCorner, target };
	}
	// 障害物が無い、または直線が交差しない場合、Playerに直接向かう経路を返す
	return { target };
}
