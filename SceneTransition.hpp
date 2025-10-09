#pragma once
#include <Siv3D.hpp>
#include <memory>

// シーンの状態遷移
enum class State
{
	TITLE,
	GAME,
	RESULT,
};

using App = SceneManager<State>;

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
