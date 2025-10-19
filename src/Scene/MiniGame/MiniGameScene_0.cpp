#include "MiniGameScene_0.hpp"

// コンストラクタ
MiniGameScene_0::MiniGameScene_0(const InitData& init)
	:IScene(init)
{
}

// デストラクタ
MiniGameScene_0::~MiniGameScene_0()
{
}

void MiniGameScene_0::update()
{
	// クリックでタイトルへ戻る
	if (MouseL.down())
	{
		changeScene(SceneState::GAME);
	}
}

void MiniGameScene_0::draw() const
{
	ClearPrint();
	Scene::SetBackground(ColorF{ 0.0, 0.0, 0.0 }); // 黒色
	Print << U"MiniGame Scene 0: Click to return to Game Scene";
}
