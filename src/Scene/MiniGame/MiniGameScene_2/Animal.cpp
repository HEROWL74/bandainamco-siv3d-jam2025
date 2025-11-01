#include "Animal.hpp"

Animal::Animal(const Texture& t, AnimalBoundary yBoundary, AnimalBoundary xBoundary, double maxSpeed)
	: m_texture(t)
	, m_position(0, 0)
	, m_minSpeed(20.0)
	, m_velocity(Vec2(Random(-maxSpeed, maxSpeed), Random(-maxSpeed, maxSpeed)))
	, m_scale(1.0)
	, m_maxSpeed(maxSpeed)
	, m_yBoundary(yBoundary)
	, m_xBoundary(xBoundary)
{
	// 初期速度の方向をランダムに設定
	m_velocity.x *= (RandomBool() ? 1.0 : -1.0);
	m_velocity.y *= (RandomBool() ? 1.0 : -1.0);
}

void Animal::update(double deltaTime)
{
	m_position += m_velocity * deltaTime;

	const double halfScreenX = Scene::Width() / 2.0;
	const double halfScreenY = Scene::Height() / 2.0;

	// 境界チェックと速度の反転処理
	if (checkAndResolveBoundary(halfScreenX, halfScreenY))
	{
		const double newSpeedX = Random(m_minSpeed, m_maxSpeed);
		const double newSpeedY = Random(m_minSpeed, m_maxSpeed);

		// 速度のX, Y成分を独立したランダム値で再設定し、ランダムな方向 (-1 または 1) を乗算
		m_velocity = Vec2(newSpeedX * (RandomBool() ? 1.0 : -1.0),
						  newSpeedY * (RandomBool() ? 1.0 : -1.0));
	}
}

bool Animal::checkAndResolveBoundary(const double halfScreenX, const double halfScreenY)
{
	bool changedDirection = false;
	const double hW = halfWidth();
	const double hH = halfHeight();

	// ----------------------------------------------------
	// X軸の境界判定
	// ----------------------------------------------------

	// 画面左端の判定
	if (m_position.x < hW)
	{
		m_position.x = hW;
		m_velocity.x *= -1;
		changedDirection = true;
	}
	// 画面右端の判定
	else if (m_position.x > Scene::Width() - hW)
	{
		m_position.x = Scene::Width() - hW;
		m_velocity.x *= -1;
		changedDirection = true;
	}

	// 画面中央のX境界判定
	if (m_xBoundary == AnimalBoundary::LeftHalf)
	{
		// 画面中央の境界線 (動物の右端を境界Xに合わせる)
		if (m_position.x > halfScreenX - hW)
		{
			m_position.x = halfScreenX - hW;
			m_velocity.x = -abs(m_velocity.x); // 必ず左向きに反転
			changedDirection = true;
		}
	}
	else if (m_xBoundary == AnimalBoundary::RightHalf)
	{
		// 画面中央の境界線 (動物の左端を境界Xに合わせる)
		if (m_position.x < halfScreenX + hW)
		{
			m_position.x = halfScreenX + hW;
			m_velocity.x = abs(m_velocity.x); // 必ず右向きに反転
			changedDirection = true;
		}
	}

	// ----------------------------------------------------
	// Y軸の境界判定
	// ----------------------------------------------------

	// 画面上端の判定
	if (m_position.y < hH)
	{
		m_position.y = hH;
		m_velocity.y *= -1;
		changedDirection = true;
	}
	// 画面下端の判定
	else if (m_position.y > Scene::Height() - hH)
	{
		m_position.y = Scene::Height() - hH;
		m_velocity.y *= -1;
		changedDirection = true;
	}

	// 画面中央のY境界判定
	if (m_yBoundary == AnimalBoundary::UpperHalf)
	{
		// 画面半分の境界線 (動物の底を境界Yに合わせる)
		if (m_position.y > halfScreenY - hH)
		{
			m_position.y = halfScreenY - hH;
			m_velocity.y = -abs(m_velocity.y); // 必ず上向きに反転
			changedDirection = true;
		}
	}
	else if (m_yBoundary == AnimalBoundary::LowerHalf)
	{
		// 画面半分の境界線 (動物の頭を境界Yに合わせる)
		if (m_position.y < halfScreenY + hH)
		{
			m_position.y = halfScreenY + hH;
			m_velocity.y = abs(m_velocity.y); // 必ず下向きに反転
			changedDirection = true;
		}
	}

	return changedDirection;
}

void Animal::draw(double elapsedTime) const
{
	const double frequency = 3.0; // 揺れる速さ (調整可能)
	const double amplitude = 20.0 * m_scale; // 振幅 (調整可能)
	// X座標も利用して、個別に波をずらす
	const double sinValue = std::sin(elapsedTime * frequency + m_position.x * 0.01);

	const double yOffset = sinValue * amplitude;

	// 回転角度の計算
	const double maxAngle = 5.0_deg * m_scale; // 最大回転角度
	double rotationAngle = sinValue * maxAngle;

	// 描画中心座標 (元の位置 + Y軸のオフセット)
	const Vec2 center = { m_position.x, m_position.y + yOffset };

	// 移動方向に応じて画像をミラーリングするロジック
	bool isMovingRight = m_velocity.x > 0;

	// 描画
	if (isMovingRight)
	{
		// 右向きの場合はそのまま描画
		m_texture.scaled(m_scale).rotated(rotationAngle).drawAt(center, ColorF(1.0, 1.0, 1.0, 1.0));
	}
	else
	{
		// 左向きの場合はミラーリングして描画
		m_texture.scaled(m_scale).mirrored().rotated(rotationAngle).drawAt(center, ColorF(1.0, 1.0, 1.0, 1.0));
	}
}
