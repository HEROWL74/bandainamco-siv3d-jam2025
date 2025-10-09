#pragma once
#include <Siv3D.hpp>
#include <memory>

// シーンの状態遷移
enum class SceneState
{
	TITLE,
	GAME,
	RESULT,
};

using App = SceneManager<SceneState>;

class SceneTransition
{
private:
	std::unique_ptr<App> mManager;


public:
	SceneTransition();
	~SceneTransition();

	bool SystemInit();
	void Update();

};
