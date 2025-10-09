#include "SceneTransition.hpp"
#include "Title.hpp"
#include "Game.hpp"
#include "Result.hpp"

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
	mManager->add<Title>(State::TITLE);
	mManager->add<Game>(State::GAME);
	mManager->add<Result>(State::RESULT);


	return true;
}

void SceneTransition::Update()
{
	mManager->update();
}
