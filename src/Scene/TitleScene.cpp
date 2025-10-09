#include "TitleScene.hpp"


// コンストラクタ
TitleScene::TitleScene(const InitData& init)
	:IScene(init)
{
	SystemInit();
	GameInit();
}

// デストラクタ
TitleScene::~TitleScene()
{
	Release();
}


bool TitleScene::SystemInit()
{


	return true;
}

void TitleScene::GameInit()
{

}

void TitleScene::update()
{
	if (KeyEnter.down() || MouseL.down())
	{
		changeScene(SceneState::GAME);
	}
}

void TitleScene::draw() const
{
	Scene::SetBackground(ColorF{ 0.0, 0.2, 0.8 });

	
}

bool TitleScene::Release()
{


	return true;
}

