#pragma once
#include <Siv3D.hpp>

// 移動制限の軸ごとの種類を定義
enum class AnimalBoundary
{
	// Y軸の制限
	UpperHalf,   // 画面上半分のみ
	LowerHalf,   // 画面下半分のみ

	// X軸の制限
	FullWidth,   // 画面全体
	LeftHalf,    // 画面左半分のみ
	RightHalf,   // 画面右半分のみ
};

class Animal
{
public:
	// コンストラクタ: テクスチャ、Y軸境界、X軸境界、最大速度を受け取る
	Animal(const Texture& t, AnimalBoundary yBoundary, AnimalBoundary xBoundary, double maxSpeed);

	// 更新処理 (移動、境界チェック、速度更新)
	void update(double deltaTime);

	// 描画処理 (アニメーション含む)
	void draw(double elapsedTime) const;

	// MiniGameScene_2::GameInit() から位置とスケールを設定するために使用
	Vec2& position() { return m_position; }
	void setScale(double scale) { m_scale = scale; }

private:
	Texture m_texture;
	Vec2 m_position;
	Vec2 m_velocity;
	double m_scale;
	double m_maxSpeed; // ランダム速度生成の上限値
	double m_minSpeed; // ランダム速度生成の下限値

	AnimalBoundary m_yBoundary;
	AnimalBoundary m_xBoundary;

	// 衝突時のサイズ計算ヘルパー
	double halfWidth() const { return m_texture.width() * m_scale / 2.0; }
	double halfHeight() const { return m_texture.height() * m_scale / 2.0; }

	// 境界チェックと速度更新を行うプライベートメソッド
	bool checkAndResolveBoundary(const double halfScreenX, const double halfScreenY);
};
