#include "MiniGameScene_3.hpp"

// コンストラクタ
MiniGameScene_3::MiniGameScene_3(const InitData& init)
	:IScene(init)
{
}

// デストラクタ
MiniGameScene_3::~MiniGameScene_3()
{
}

void MiniGameScene_3::update()
{
	// クリックでタイトルへ戻る
	if (MouseL.down())
	{
		changeScene(SceneState::RESULT);
	}
}

void MiniGameScene_3::draw() const
{
	ClearPrint();
	Scene::SetBackground(ColorF{ 0.0, 0.7, 0.6 }); // シアン色
	Print << U"MiniGame Scene 3: Click to return to Game Scene";
}
