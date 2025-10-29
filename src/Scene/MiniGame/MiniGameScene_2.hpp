#pragma once

#include <Siv3D.hpp>
#include "../SceneTransition.hpp"
#include "../MiniGame/MiniGameScene_2/Puzzle.hpp"

enum class State
{
	Idle,
	Playing,
	Clear,
	Finish,
};

class MiniGameScene_2 : public App::Scene
{
private:
	std::unique_ptr<Puzzle> m_puzzle;

	State m_state;
	Array<FilePath> m_puzzleImages;					// 使う画像たちを格納する
	int32 m_puzzleIndex;							// パズルの画像の番号
	double m_timer;									// 次の画像に推移するまでの時間

	Font m_bigFont;									// 描画するフォント

	void IdleUpdate();
	void PlayingUpdate();
	void ClearUpdate();
	void FinishUpdate();

	void IdleDraw() const;
	void PlayingDraw() const;
	void ClearDraw() const;
	void FinishDraw() const;

public:
	MiniGameScene_2(const InitData& init);
	~MiniGameScene_2();

	bool SystemInit();
	void GameInit();
	void update() override;
	void draw() const override;
};
