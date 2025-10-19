#include "MiniGameScene_1.hpp"

// コンストラクタ
MiniGameScene_1::MiniGameScene_1(const InitData& init)
	:IScene(init)
{
}

// デストラクタ
MiniGameScene_1::~MiniGameScene_1()
{
}

void MiniGameScene_1::update()
{
	// クリックでタイトルへ戻る
	if (MouseL.down())
	{
		changeScene(SceneState::GAME);
	}
}

void MiniGameScene_1::draw() const
{
	ClearPrint();
	Scene::SetBackground(ColorF{ 0.8, 0.7, 0.6 }); // ベージュ色
	Print << U"MiniGame Scene 1: Click to return to Game Scene";
}
