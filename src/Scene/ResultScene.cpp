#include "ResultScene.hpp"


// コンストラクタ
ResultScene::ResultScene(const InitData& init)
	:IScene(init)
{
	SystemInit();
	GameInit();
}

// デストラクタ
ResultScene::~ResultScene()
{
	Release();
}

bool ResultScene::SystemInit()
{


	return true;
}

void ResultScene::GameInit()
{

}

void ResultScene::update()
{
	if (KeyEnter.down() || MouseL.down())
	{
		changeScene(SceneState::TITLE);
	}
}

void ResultScene::draw() const
{
	Scene::SetBackground(ColorF{ 0.2, 0.8, 0.0 });
}

bool ResultScene::Release()
{


	return true;
}
