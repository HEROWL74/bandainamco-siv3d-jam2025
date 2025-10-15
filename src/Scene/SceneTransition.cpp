#include "SceneTransition.hpp"
#include "TitleScene.hpp"
#include "GameScene.hpp"
#include "ResultScene.hpp"

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

	// ゲーム起動時にゲーム画面からスタート（フェードイン時間無し）
	m_manager->init(SceneState::TITLE, 0s);


	return true;
}

void SceneTransition::Update()
{
	m_manager->update();
}
