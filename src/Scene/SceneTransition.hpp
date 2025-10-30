#pragma once
#include <Siv3D.hpp>
#include <memory>
#include "../Option/Settings.hpp"
#include "../Option/AudioManager.hpp"

// シーンの状態遷移
enum class SceneState
{
	TITLE,
	GAME,
	RESULT,
	MINIGAME_0,
	MINIGAME_1,
	MINIGAME_2,
	MINIGAME_3,
};

// シーン間で共有するデータ
struct SharedData
{
	std::shared_ptr<Settings> settings;
	std::shared_ptr<AudioManager> audio;
	uint8 nextScene = 0;	// 次に遷移するミニゲームの番号 (0~4)
	bool isGameClear = false; // ゲームクリアしたかどうか(ミニゲーム3をクリアしたらtrueにする予定)
};

using App = SceneManager<SceneState, SharedData>;

class SceneTransition
{
private:
	std::unique_ptr<App> m_manager;
	std::shared_ptr<Settings> m_settings;
	std::shared_ptr<AudioManager> m_audio;


public:
	SceneTransition(std::shared_ptr<Settings> settings, std::shared_ptr<AudioManager> audio);
	~SceneTransition();

	bool SystemInit();
	void Update();

};
