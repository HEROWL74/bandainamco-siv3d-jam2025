#include "MovingBackground.hpp"

MovingBackground::MovingBackground()
{

}

MovingBackground::~MovingBackground()
{}

bool MovingBackground::SystemInit()
{
	m_textures << Texture{ U"⭐"_emoji };
	m_textures << Texture{ U"Assets/Image/Dinosaur/Tyrannosaurus.png" };
	m_textures << Texture{ U"Assets/Image/Dinosaur/Triceratops.png" };
	m_textures << Texture{ U"Assets/Image/Dinosaur/Stegosaurus.png" };
	m_textures << Texture{ U"Assets/Image/Dinosaur/Iguanodon.png" };
	m_textures << Texture{ U"Assets/Image/Animal/eto_uma_furikaeri.png" };
	m_textures << Texture{ U"Assets/Image/Animal/eto_remake_ushi.png" };
	m_textures << Texture{ U"Assets/Image/Animal/bird_aoitori_bluebird.png" };

	return true;
}

void MovingBackground::GameInit()
{
	m_items.clear();
	m_items.resize(m_spawnCount);

	for (auto& it : m_items)
	{
		SpawnOne(it);
	}
}

void MovingBackground::Update()
{
	const double dt = Scene::DeltaTime();
	const double w = Scene::Width();
	const double h = Scene::Height();

	for (auto& it : m_items)
	{
		// 位置更新
		it.pos += it.vel * dt;

		// 回転更新（角度をラップ）
		it.angle += it.angularVel * dt;
		it.angle = std::fmod(it.angle, Math::TwoPi);
		if (it.angle < 0.0) it.angle += Math::TwoPi;

		// 画面外判定：完全に画面外に行ったら再スポーン
		// ここではテクスチャのサイズを考慮して余裕を持って判定
		const double texW = m_textures[it.texIndex].width() * it.scale;
		const double texH = m_textures[it.texIndex].height() * it.scale;

		// 範囲チェック（周囲に余白を見て完全に画面の外に出たとき）
		if (it.pos.x < -texW - m_spawnMargin || it.pos.x > w + texW + m_spawnMargin ||
			it.pos.y < -texH - m_spawnMargin || it.pos.y > h + texH + m_spawnMargin)
		{
			SpawnOne(it);
		}
	}
}

void MovingBackground::Draw() const
{
	// 画像を重ねて描画。回転・拡大・透明度を反映
	for (const auto& it : m_items)
	{
		if (it.texIndex < 0 || it.texIndex >= static_cast<int32>(m_textures.size())) continue;

		const Texture& tex = m_textures[it.texIndex];

		// scaled -> rotated -> drawAt の順でチェーン可能
		// alpha は Draw の色に含める
		const double drawScale = it.scale;
		tex.scaled(drawScale).rotated(it.angle).drawAt(it.pos, ColorF(1.0, 1.0, 1.0));
	}
}

void MovingBackground::SpawnOne(Item& it)
{
	// ランダムにテクスチャを選択
	it.texIndex = Random(0, static_cast<int32>(m_textures.size()) - 1);

	// 出現は下から上へ斜めに移動させる（左右どちらから来るかランダム）
	const double w = Scene::Width();
	const double h = Scene::Height();

	// ランダムに左右どちらから斜めに上がってくるか決める
	const bool fromLeft = RandomBool();

	// スケール（大きさ）を決定
	it.scale = Random(m_minScale, m_maxScale);

	// 出現 X は左右の外側にランダムに（少し余白を持たせる）
	double x;
	if (fromLeft)
	{
		x = -m_spawnMargin - m_textures[it.texIndex].width() * it.scale * Random(0.0, 1.0);
	}
	else
	{
		x = w + m_spawnMargin + m_textures[it.texIndex].width() * it.scale * Random(0.0, 1.0);
	}

	// Y（下から出現）をランダム（画面下より少し下）
	double y = h + Random(0.0, h * 0.2);

	it.pos = Vec2{ x, y };

	// 速度：画像の大きさに応じて変更
	double t = 0.0;
	if (m_maxScale > m_minScale)
	{
		t = (it.scale - m_minScale) / (m_maxScale - m_minScale);		// その画像の大きさが指定範囲の何％かを計算
		t = Clamp(t, 0.0, 1.0);
	}
	else
	{
		t = 0.0;
	}

	const double speed = Math::Lerp(m_minSpeed, m_maxSpeed, t);			// 速度

	// 斜め角度
	const double angleDeg = (fromLeft ? -50.0_deg : -130.0_deg) + Random(-30.0_deg, 30.0_deg);
	const double rad = angleDeg;
	it.vel = Vec2{ std::cos(rad), std::sin(rad) } *speed;

	// 回転初期値と回転速度（ゆっくり）
	it.angle = Random(0.0, Math::TwoPi);
	it.angularVel = Random(m_minAngular, m_maxAngular);
}
