#pragma once
#include <Siv3D.hpp>

struct Item
{
	int32 texIndex{ 0 };
	Vec2 pos{};
	Vec2 vel{};
	double angle{ 0.0 };
	double angularVel{ 0.0 };
	double scale{ 0.0 };
};

class MovingBackground
{
private:
	Array<Texture> m_textures;						// 読み込んだ画像群
	Array<Item> m_items;							// 画面上に存在する動く画像

	// 設定パラメータ
	int32 m_spawnCount{ 3 };						// 画面に同時に存在する数
	double m_minSpeed{ 30.0 };						// px / 秒（最小）
	double m_maxSpeed{ 120.0 };						// px / 秒（最大）
	double m_minAngular{ -15_deg };					// 回転速度の最小
	double m_maxAngular{ 15_deg };					// 回転速度の最大
	double m_minScale{ 0.4 };						// 縮尺最小
	double m_maxScale{ 1.2 };						// 縮尺最大
	double m_spawnMargin{ 40.0 };					// 画面外から出現させる余白

	void SpawnOne(Item& it);						// 1個を初期化（新規or再スポーン）

public:
	MovingBackground();
	~MovingBackground();

	bool SystemInit();
	void GameInit();
	void Update();
	void Draw() const;

	// オプション
	void SetSpawnCount(int32 c) { m_spawnCount = c; }
	void SetSpeedRange(double minV, double maxV) { m_minSpeed = minV; m_maxSpeed = maxV; }
	void SetAngularRange(double minA, double maxA) { m_minAngular = minA, m_maxAngular = maxA; }
	void SetScaleRange(double minS, double maxS) { m_minScale = minS, m_maxScale = maxS; }
};
