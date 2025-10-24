#pragma once

#include <Siv3D.hpp>
#include "../SceneTransition.hpp"
#include "../MiniGame/MiniGameScene_2/Puzzle.hpp"

class MiniGameScene_2 : public App::Scene
{
private:
	std::unique_ptr<Puzzle> m_puzzle;

	Font m_bigFont;
	Vec2 m_fontPos;
public:
	MiniGameScene_2(const InitData& init);
	~MiniGameScene_2();

	bool SystemInit();
	void GameInit();
	void update() override;
	void draw() const override;
};
