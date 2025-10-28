#pragma once

#include <Siv3D.hpp>
#include "../SceneTransition.hpp"
#include "../../Option/AudioManager.hpp"
#include "../../Effect/EffectManager.hpp"

// ノーツのデータ構造
struct Note
{
	double startTime;// ノーツが到達する時間（秒）	
	double duration; // ノーツの長さ（秒）
	int pitch; // ノーツのピッチ（画面上のY座標に対応させる）

	enum class State {
		None,// 未判定
		Active_Perfect,
		Active_Miss,
		Hit, // 成功
		Miss, // ミス
	};
	State state = State::None;
};

enum class GameStatus
{
	Ready,
	Countdown,
	Playing,
	Result,
};

class MiniGameScene_0 : public App::Scene
{
public:
	MiniGameScene_0(const InitData& init);
	~MiniGameScene_0();

	void update() override;
	void draw() const override;

private:
	Font m_font20;
	Font m_font24;
	Font m_font30;

	struct Star {
		Vec2 pos;
		double speedRatio; // 0.0:遠い(静止) - 1.0:近い(ノーツと同じ移動)
		double size;
	};
	Array<Star> m_stars; // 星のリスト

	EffectManager m_effectManager;
	double m_lastHoldEffectTime = 0.0; // 長押しエフェクトが最後に発生した時間
	const double m_holdEffectInterval = 0.03; // エフェクトを発生させる間隔（秒）

	// ノーツリスト（譜面）
	Array<Note> m_notes;

	// 音楽
	Audio m_bgm;

	// プレイヤーのスライド位置（マウスのY座標に対応させる）
	double m_playerSlideY = 0.0;

	// 判定ラインのY座標（画面中央など）
	const double m_judgmentLineY = 1000.0;

	// ノーツが画面に現れてから判定ラインに到達するまでの時間（秒）
	const double m_approachTime = 2.0;

	// ゲームステータス
	GameStatus m_status = GameStatus::Ready;

	bool m_isPitchPerfect = false;

	// スコアリング
	int m_score = 0;
	int m_combo = 0;
	int m_currentNoteIndex = 0; // 次に判定するノーツのインデックス
	double m_gameStartTime = 0.0; // ゲーム開始時の時刻（Scene::Time()）

	const Texture m_earthIcon;
	const Texture m_starIcon;
	const Texture m_timeIcon;

	// プライベート関数
	void loadBGMAndNotes();
	void updateReady();
	void updatePlaying();
	void updateResult();
};
