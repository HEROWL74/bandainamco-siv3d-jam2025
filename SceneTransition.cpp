#include "SceneTransition.hpp"
#include "TitleScene.hpp"
#include "GameScene.hpp"
#include "ResultScene.hpp"

SceneTransition::SceneTransition()
	:mManager(nullptr)
{

}

SceneTransition::~SceneTransition()
{
}

bool SceneTransition::SystemInit()
{
	mManager = std::make_unique<App>();

	// シーンを登録
	mManager->add<TitleScene>(SceneState::TITLE);
	mManager->add<GameScene>(SceneState::GAME);
	mManager->add<ResultScene>(SceneState::RESULT);

	// ゲーム起動時にゲーム画面からスタート（フェードイン時間無し）
	mManager->init(SceneState::GAME, 0s);


	return true;
}

void SceneTransition::Update()
{
	mManager->update();
}
