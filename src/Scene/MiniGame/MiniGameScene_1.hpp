#pragma once

#include <Siv3D.hpp>
#include "../SceneTransition.hpp"
#include "../../Option/AudioManager.hpp" // MiniGameScene_1.cppでgetData().audioを使っているため
#include "MiniGameScene_0.hpp" // Note構造体とGameStatus enumを使うため

class MiniGameScene_1 : public App::Scene
{
private:
	//
	static constexpr int NumLanes = 4;
	const double m_approachTime = 2.0; // ノーツが降ってくる時間（秒）
	const double m_judgmentLineY = 500.0; // 判定ラインのY座標
	const Array<double> m_laneXPositions = { 0.0, 200.0, 400.0, 600.0, 800.0 };
	const Array<Input> m_judgmentKeys = { KeyD, KeyF, KeyJ, KeyK };

	// ゲームの状態
	GameStatus m_status = GameStatus::Ready;
	double m_gameStartTime = 0.0;
	Array<Note> m_notes;
	int m_currentNoteIndex = 0;
	int m_score = 0;
	int m_combo = 0;
	Array<int> m_activeNotes;

	// フォント
	Font m_font20;
	Font m_font30;
	Font m_font24;
	Font m_font40;

	void loadBGMAndNotes();
	void updateReady();
	void updatePlaying();
	void updateResult();

	// 描画関連
	RectF getLaneRect(int lane, double timeToArrival, double duration) const;
	void drawNote(const Note& note, double timeToArrival) const;

public:
	MiniGameScene_1(const InitData& init);
	~MiniGameScene_1();

	void update() override;
	void draw() const override;
};
