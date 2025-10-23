#include "SceneTransition.hpp"
#include "TitleScene.hpp"
#include "GameScene.hpp"
#include "ResultScene.hpp"
#include "MiniGame/MiniGameScene_0.hpp"
#include "MiniGame/MiniGameScene_1.hpp"
#include "MiniGame/MiniGameScene_2.hpp"
#include "MiniGame/MiniGameScene_3.hpp"

SceneTransition::SceneTransition(std::shared_ptr<Settings> settings, std::shared_ptr<AudioManager> audio)
	:m_manager(nullptr)
	,m_settings(settings)
	,m_audio(audio)
{

}

SceneTransition::~SceneTransition()
{
}

bool SceneTransition::SystemInit()
{
	m_manager = std::make_unique<App>();

	// 共有データの設定
	m_manager->get()->settings = m_settings;
	m_manager->get()->audio = m_audio;

	// シーンを登録
	m_manager->add<TitleScene>(SceneState::TITLE);
	m_manager->add<GameScene>(SceneState::GAME);
	m_manager->add<ResultScene>(SceneState::RESULT);
	m_manager->add<MiniGameScene_0>(SceneState::MINIGAME_0);
	m_manager->add<MiniGameScene_1>(SceneState::MINIGAME_1);
	m_manager->add<MiniGameScene_2>(SceneState::MINIGAME_2);
	m_manager->add<MiniGameScene_3>(SceneState::MINIGAME_3);

	// ゲーム起動時にゲーム画面からスタート（フェードイン時間無し）
	m_manager->init(SceneState::GAME, 0s);

	return true;
}

void SceneTransition::Update()
{
	m_manager->update();
}
