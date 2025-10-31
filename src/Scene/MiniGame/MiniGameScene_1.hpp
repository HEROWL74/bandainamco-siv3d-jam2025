#pragma once

#include <Siv3D.hpp>
#include "../SceneTransition.hpp"
#include "../../Option/AudioManager.hpp"
#include "MiniGameScene_0.hpp"
#include "../../Effect/EffectManager.hpp"

enum class DinosaurType
{	
	Tyrannosaurus,
	Triceratops,
	Pteranodon,
	Tyrannosaurus_hair,
	Trex_kokkaku,
	Pachycephalosaurus,
	Stegosaurus,
	Brachiosaurus,
	Iguanodon
};

struct MovingDinosaur
{
	DinosaurType type;
	Texture texture;
	double x = 0.0;
	double speed = 0.0;
	bool movingRight = true;
	double scale = 1.0;
	double spawnTime = 0.0;
};

class MiniGameScene_1 : public App::Scene
{
private:
	static constexpr int NumLanes = 4;
	const double m_approachTime = 2.0; // ノーツが降ってくる時間（秒）
	const double m_judgmentLineY = 900; // 判定ラインのY座標
	static constexpr double LaneWidth = 200.0; // 1レーンあたりの幅
	static constexpr double GameAreaWidth = NumLanes * LaneWidth; // ゲーム領域の総幅 (800.0)
	const Array<double> m_laneRelativeXPositions = { 0.0, 200.0, 400.0, 600.0, 800.0 };
	const Array<Input> m_judgmentKeys = { KeyD, KeyF, KeyJ, KeyK };
	double m_lastHoldEffectTime = 0.0; // 長押しエフェクトが最後に発生した時間
	const double m_holdEffectInterval = 0.15; // エフェクトを発生させる間隔（秒）

	Audio m_laneAudios[NumLanes];
	Audio m_missAudio;

	// ゲームの状態
	GameStatus m_status = GameStatus::Ready;
	double m_gameStartTime = 0.0;
	Array<Note> m_notes;
	int m_currentNoteIndex = 0;
	int m_score = 0;
	int m_combo = 0;
	Array<int> m_activeNotes;
	bool m_isFullCombo = true;

	// フォント
	Font m_font20;
	Font m_font30;
	Font m_font24;
	Font m_font64;

	Texture m_timeIcon;
	Texture m_BackgroundTexture;

	Array<Texture> dinos =
	{
		Texture(U"assets/Image/Dinosaur/Tyrannosaurus.png"),
		Texture(U"assets/Image/Dinosaur/Triceratops.png"),
		Texture(U"assets/Image/Dinosaur/Tyrannosaurus_hair.png"),
		Texture(U"assets/Image/Dinosaur/trex_kokkaku.png"),
		Texture(U"assets/Image/Dinosaur/Pteranodon.png"),
		Texture(U"assets/Image/Dinosaur/dinosaur_pachycephalosaurus.png"),
		Texture(U"assets/Image/Dinosaur/Stegosaurus.png"),
		Texture(U"assets/Image/Dinosaur/Brachiosaurus.png"),
		Texture(U"assets/Image/Dinosaur/Iguanodon.png"),
	};
	Texture m_keyboardTexture; // キーボード操作の説明用テクスチャ

	Array<MovingDinosaur> m_movingDinosaurs; // 現在画面上にいる恐竜の配列

	const double m_dinosaurSpawnInterval = 1.0; // 恐竜が出現する基本間隔（秒）
	double m_dinosaurNextSpawnTime = 0.0; // 次の出現時間
	DinosaurType m_nextDinosaurType = DinosaurType::Tyrannosaurus;
	int m_dinosaurOrderIndex = 0;

	void drawLanes() const;
	void loadBGMAndNotes();
	void updateReady();
	void updateCountdown();
	void updatePlaying();
	void updateResult();

	// 描画関連
	RectF getLaneRect(int lane, double timeToArrival, double duration) const;
	void drawNote(const Note& note, double timeToArrival) const;

	EffectManager m_effectManager;

public:
	MiniGameScene_1(const InitData& init);
	~MiniGameScene_1();

	void update() override;
	void draw() const override;
};
