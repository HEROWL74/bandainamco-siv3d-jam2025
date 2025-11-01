#pragma once

#include <Siv3D.hpp>
#include "../SceneTransition.hpp"
#include "../MiniGame/MiniGameScene_3/PlayerLine.hpp"
#include "../MiniGame/MiniGameScene_3/ShapeManager.hpp"
#include "../MiniGame/MiniGameScene_3/MovingBackground.hpp"
#include "../../Effect/EffectManager.hpp"
#include "../../Effect/GameClearEffect.hpp"

enum class PlayerState
{
	Idle,
	Playing,
	Clear,
	Finish,
};

// 判定結果の構造体
struct CoverageResult
{
	double coverage = 0.0;					// 0..1 (通過率)
	int visitedCount = 0;
	int baseCount = 0;
	int maxGap = 0;
	Array<char32> visited;					// 各 base 点の訪問フラグ（描画用に公開）
};

class MiniGameScene_3 : public App::Scene
{
private:
	PlayerState m_state;
	std::unique_ptr<class PlayerLine> m_playerLine;
	std::unique_ptr<class ShapeManager> m_shapeManager;
	std::unique_ptr<class MovingBackground> m_movingBG;

	Font m_font;
	int32 m_shapeIndex;											// 図形の番号
	double m_time;												// 現在の時間

	const double m_timeLimit{ 20.0 };							// 制限時間
	const double m_minDist{ 6.0 };								// 線の描画を開始する最小の距離
	const double m_hausdorffThreshold{ 50.0 };					// 始点と終点がほぼ繋がったとみなす距離
	const double m_coverageThreshold{ 0.85 };					// base点のうちどのくらい通ったらOKかをみなす割合

	// 判定キャッシュ
	bool m_needRecalc;
	CoverageResult m_lastCoverage;								// ComputeCoverageの最新結果を保存

	bool m_unpainted;												// 塗り足りてないか（true：足りてない）

	struct Star {
		Vec2 pos;
		double speedRatio; // 0.0:遠い(静止) - 1.0:近い(速い)
		double size;
		ColorF color;
	};
	Array<Star> m_stars; // 星のリスト

	Texture m_earth;											// 地球の画像
	double m_earthRotateAngle;									// 回る角度
	double m_earthRotateSpeed;									// 角速度

	double m_gameStartTime{ 0.0 };
	Stopwatch m_stopwatch;
	bool m_isTimeOver;											// 時間経過したかのフラグ

	Texture m_mouseImage;

	Audio m_clearSE;                //パズルクリア時のSE
	EffectManager m_effectManager;

	//各状態時の更新処理関数
	void IdleUpdate();
	void PlayingUpdate();
	void ClearUpdate();
	void FinishUpdate();

	// 各状態時の描画処理関数
	void IdleDraw() const;
	void PlayingDraw() const;
	void ClearDraw() const;
	void FinishDraw() const;

	// ヘルパーさん
	void StartGame();
	void StartTimer();
	double GetRemainingTime() const;
	bool IsStrokeVaildAsShape(const LineString& userLine, const LineString& base);
	CoverageResult ComputeCoverage(const LineString& userLine, const LineString& base, double radius);

public:
	MiniGameScene_3(const InitData& init);
	~MiniGameScene_3() = default;

	bool SystemInit();
	void GameInit();
	void update() override;
	void draw() const override;
};
