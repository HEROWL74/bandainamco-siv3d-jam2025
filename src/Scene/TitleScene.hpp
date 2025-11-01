#pragma once
#include <Siv3D.hpp>
#include "SceneTransition.hpp"
#include "../Option/TitleOption.hpp"
#include "../Effect/EffectManager.hpp"
#include "../Effect/GameClearEffect.hpp"
#include <vector>

enum class TitleState
{
	Title,
	Option,
	Music,
	Exit,
};

class TitleScene : public App::Scene
{
private:
	Texture m_optionIcon;
	Texture m_normalBeethoven;
	Texture m_smileBeethoven;

	Font m_font128;

	RoundRect m_startButton;
	RoundRect m_exitButton;
	RoundRect m_optionButton;
	RoundRect m_musicButton;
	Texture m_musicIcon;

	TitleState m_titleState;

	std::unique_ptr<TitleOption> m_titleOption;
	mutable bool m_showMusicPanel = false;

	Array<String> m_playlist;
	const std::map<String, String> m_trackTitles =
	{
		{ U"No14_1st", U"月光ソナタ 第1楽章" },
		{ U"No5_1st", U"運命 第1楽章" },
		{ U"No9_1st", U"第九 第1楽章" },
		{ U"No9_2nd", U"第九 第2楽章" },
		{ U"No9_3rd", U"第九 第3楽章" },
		{ U"No9_4th", U"第九 第4楽章" },
	};
	mutable int m_currentTrackIndex = -1;
	mutable int m_nextTrackIndex = -1; // -1は何も再生していない

	double m_clearTransitionStartTime = 0.0;

	Audio m_clearSE; // クリアSEを再生するためのAudioオブジェクト
	bool m_clearEffectPlayed = false;
	EffectManager m_effectManager;

	int DrawMusicPanel() const;
public:
	TitleScene(const InitData& init);
	~TitleScene();

	bool SystemInit();
	void GameInit();
	void update() override;
	void draw() const override;
	bool Release();


};
