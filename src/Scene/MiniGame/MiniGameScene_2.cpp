#include "MiniGameScene_2.hpp"

// コンストラクタ
MiniGameScene_2::MiniGameScene_2(const InitData& init)
	:IScene(init)
{
}

// デストラクタ
MiniGameScene_2::~MiniGameScene_2()
{
}

void MiniGameScene_2::update()
{
	// クリックでタイトルへ戻る
	if (MouseL.down())
	{
		changeScene(SceneState::GAME);
	}
}

void MiniGameScene_2::draw() const
{
	ClearPrint();
	Scene::SetBackground(ColorF{ 0.8, 0.7, 0.0 }); // 黄色
	Print << U"MiniGame Scene 2: Click to return to Game Scene";
}
