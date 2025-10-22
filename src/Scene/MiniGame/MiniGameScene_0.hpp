#pragma once

#include <Siv3D.hpp>
#include "../SceneTransition.hpp"
#include "../../Option/AudioManager.hpp"

// ノーツのデータ構造
struct Note
{
	double startTime;// ノーツが到達する時間（秒）	
	double duration; // ノーツの長さ（秒）
	int pitch; // ノーツのピッチ（画面上のY座標に対応させる）

	enum class State {
		None,// 未判定
		Active, // 判定ライン到達済、判定中
		Hit, // 成功
		Miss, // ミス
	};
	State state = State::None;
};

enum class GameStatus
{
	Ready,
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

	// プライベート関数
	void loadBGMAndNotes();
	void updateReady();
	void updatePlaying();
	void updateResult();
};
