#include "TitleScene.hpp"


// コンストラクタ
TitleScene::TitleScene(const InitData& init)
	:IScene(init)
	, m_titleOption(nullptr)
	, m_normalBeethoven(U"assets/Image/Portrait/Beethoven.jpg")
	, m_smileBeethoven(U"assets/Image/Portrait/Beethoven_Smile.jpg")
	, m_font128(128, Typeface::Bold)
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
	auto& data = getData();
	m_titleOption = std::make_unique<TitleOption>(data.settings, data.audio);
	if (m_titleOption == nullptr) return false;

	// titleBGMのロード
	if (data.audio)
	{
		data.audio->PreLoadBGM(U"TitleBGM", U"example/test.mp3");
		m_clearSE = Audio(U"assets/sound/se/PuzzlePiace_Clear.mp3");
	}

	auto& audio = getData().audio;
	if (audio)
	{
		audio->PreLoadBGM(U"No14_1st", U"Assets/sound/bgm/No14_1st.mp3"); //タイトル
		audio->PreLoadBGM(U"No5_1st", U"Assets/sound/bgm/No5_1st.mp3"); //メインゲーム
		audio->PreLoadBGM(U"No9_1st", U"Assets/sound/bgm/No9_1st.mp3"); //ミニゲーム0
		audio->PreLoadBGM(U"No9_2nd", U"Assets/sound/bgm/No9_2nd.mp3"); //ミニゲーム1
		audio->PreLoadBGM(U"No9_3rd", U"Assets/sound/bgm/No9_3rd.mp3"); //ミニゲーム2
		audio->PreLoadBGM(U"No9_4th", U"Assets/sound/bgm/No9_4th.mp3"); //ミニゲーム3
	}

	// プレイリスト
	m_playlist = { U"No14_1st", U"No5_1st", U"No9_1st", U"No9_2nd", U"No9_3rd", U"No9_4th" };

	m_optionIcon = Texture{ U"⚙️"_emoji };
	m_musicIcon = Texture{ U"▶"_emoji };

	return true;
}

void TitleScene::GameInit()
{
	auto& data = getData();

	// BGMの再生
	if (data.audio)
	{
		data.audio->PlayBGM(U"TitleBGM", true);
	}

	const double ronded = 6;
	// スタートボタンの座標
	const double startButtonW = 700;
	const double startButtonH = 80;
	const double startButtonY = Scene::Height() * 0.70; // 画面下部寄り
	m_startButton = RectF{ Arg::center(Scene::Center().x, startButtonY), startButtonW, startButtonH }.rounded(ronded);
	// 終了ボタンの座標
	const double exitY = Scene::Height() * 0.90;
	const double exitW = 700;
	const double exitH = 80;
	m_exitButton = RectF{ Arg::center(Scene::Center().x, exitY), exitW, exitH }.rounded(ronded);

	// オプションボタンの座標
	const double buttonX = 1600.0;
	const double buttonY = 950.0;
	const double buttonW = 150;

	m_optionButton = RectF{ Arg::center(buttonX, buttonY), buttonW, buttonW }.rounded(ronded);

	const double musicButtonY = Scene::Height() * 0.8;
	const double musicW = 700;
	const double musicH = 80;

	m_musicButton = RectF{ Arg::center(Scene::Center().x, musicButtonY), musicW, musicH }.rounded(ronded);

	m_titleState = TitleState::Title;
}

void TitleScene::update()
{
	switch (m_titleState)
	{
	case TitleState::Title:
		// スタートボタン
		if (KeyEnter.down() || m_startButton.leftClicked())
		{
			changeScene(SceneState::GAME);
			getData().audio->StopBGM(1s);
		}

		// 終了ボタン
		if (m_exitButton.leftClicked())
		{
			System::Exit();
		}

		// Musicボタン
		if (m_musicButton.leftClicked())
		{
			m_showMusicPanel = !m_showMusicPanel; // トグル

			// パネルを開いたら Music 状態に遷移
			m_titleState = (m_showMusicPanel ? TitleState::Music : TitleState::Title);
		}

		// Optionボタン
		if (m_optionButton.leftClicked() || KeyO.down())
		{
			m_titleState = TitleState::Option;
		}
		break;

	case TitleState::Option:

		m_titleOption->Update();
		if (m_titleOption->IsClosed())
		{
			m_titleState = TitleState::Title;
			m_titleOption->GameInit();
		}

		if (m_titleOption->IsExit())
		{
			System::Exit();
		}
		break;

	case TitleState::Music:

		// 曲切り替え処理（クリック結果を反映）
		if (m_nextTrackIndex >= 0)
		{
			m_currentTrackIndex = m_nextTrackIndex;
			m_nextTrackIndex = -1;

			auto& audio = getData().audio;
			if (audio)
			{
				const String& id = m_playlist[m_currentTrackIndex];
				audio->PlayBGM(id, true);
			}
		}

		break;

	case TitleState::Exit:
		break;
	}

	// すべての曲が止まっていたら TitleBGM を再生
	auto& audio = getData().audio;
	if (audio)
	{
		bool anyPlaying = false;
		for (const auto& [id, bgm] : audio->GetBGMMap())
		{
			if (bgm.isPlaying())
			{
				anyPlaying = true;
				break;
			}
		}

		// プレイヤーが閉じていて、何も鳴ってない時にTitleBGMを流す
		if (!anyPlaying && !m_showMusicPanel)
		{
			audio->PlayBGM(U"TitleBGM", true, 0s);
			m_currentTrackIndex = -1; // 状態リセット
		}
	}

	// ゲームクリア時の演出処理
	if (getData().isGameClear)
	{
		if (m_clearTransitionStartTime == 0.0)
		{
			m_clearTransitionStartTime = Scene::Time();
		}

		if (!m_clearEffectPlayed)
		{
			// エフェクトを生成
			m_effectManager.Add<GameClearEffect>(Scene::Center(), 1.5);
			// SEを再生
			m_clearSE.playOneShot();
			m_clearEffectPlayed = true;
		}
	}
	else if (!getData().isGameClear)
	{
		// クリア状態ではない場合、時間をリセット
		m_clearTransitionStartTime = 0.0;
	}

	m_effectManager.Update();
}


void TitleScene::draw()const
{
	const auto& data = getData();
	if (data.isGameClear)
	{
		// クリア演出中
		const double transitionDuration = 2.0; // フェードにかける時間 (2.0秒)
		const double elapsedTime = Min(Scene::Time() - m_clearTransitionStartTime, transitionDuration);
		// フェードの進行度合い (0.0 -> 1.0)
		const double fadeAlpha = elapsedTime / transitionDuration;
		// 背景色のブレンド (Black -> Gold)
		Scene::SetBackground(Palette::Black.lerp(Palette::Gold, fadeAlpha));
		// ノーマルベートーヴェンはフェードアウト (1.0 -> 0.0)
		m_normalBeethoven.scaled(1.0).drawAt(Scene::Center(), ColorF(1.0, 1.0 - fadeAlpha));
		// スマイルベートーヴェンはフェードイン (0.0 -> 1.0)
		m_smileBeethoven.scaled(1.0).drawAt(Scene::Center(), ColorF(1.0, fadeAlpha));
	}
	else
	{
		Scene::SetBackground(Palette::Black);
		m_normalBeethoven.scaled(1.0).drawAt(Scene::Center());
	}
	m_font128(U"ベートヴェンの心").drawAt(100, Scene::Width() / 2.0, 100.0, Palette::Red);
	m_font128(U"「第九の呪い」").drawAt(80, Scene::Width() / 2.0, 200.0, Palette::Red);

	const ColorF startButtonColor = m_startButton.mouseOver() ? ColorF{ 0.2, 0.6, 1.0 } : ColorF{ 0.0, 0.4, 0.8 };
	m_startButton.draw(startButtonColor);

	// ボタン内のテキスト描画 (FontAsset("Menu")を使用)
	m_font128(U"スタート").drawAt(70, m_startButton.center(), Palette::White);

	const ColorF exitButtonColor = m_exitButton.mouseOver() ? ColorF{ 0.6, 0.4, 1.0 } : ColorF{ 0.4, 0.2, 0.8 };
	m_exitButton.draw(exitButtonColor);
	m_font128(U"おわる").drawAt(70, m_exitButton.center(), Palette::White);

	// 設定ボタンの描画	
	m_optionButton.draw(Palette::Silver);

	// マウスが図形の上に来たら影ができる
	if (m_optionButton.mouseOver())
	{
		m_optionButton.drawShadow(Vec2{ 2, 2 }, 12, 1).draw(ColorF{ 0.9, 0.8, 0.6 });
	}

	// 歯車マーク
	const double iconX = 1600.0;
	const double iconY = 950.0;
	m_optionIcon.scaled(0.8).drawAt(iconX, iconY);

	// オプション画面の描画
	if (m_titleState == TitleState::Option)
	{
		m_titleOption->Draw();
	}

	const ColorF musicButtonColor = m_musicButton.mouseOver() ? ColorF{ 1.0, 0.5, 0.0 } : ColorF{ 0.8, 0.3, 0.0 };
	m_musicButton.draw(musicButtonColor);
	m_font128(U"音楽が聴けるよ！").drawAt(70, m_musicButton.center(), Palette::White);

	// パネル表示
	if (m_showMusicPanel)
	{
		int clicked = DrawMusicPanel();
		if (clicked >= 0)
		{
			m_nextTrackIndex = clicked; // ← getData()ではなく自前変数に保存
		}
	}

	m_effectManager.Draw();
}

int TitleScene::DrawMusicPanel() const
{
	const auto& data = getData();
	auto& audio = data.audio;
	int clickedIndex = -1;

	// 【新規追加】表示名とIDを対応させるマップを定義
	const std::map<String, String> trackTitles =
	{
		{ U"No14_1st", U"月光ソナタ 第1楽章" },
		{ U"No5_1st", U"運命 第1楽章" },
		{ U"No9_1st", U"第九 第1楽章" },
		{ U"No9_2nd", U"第九 第2楽章" },
		{ U"No9_3rd", U"第九 第3楽章" },
		{ U"No9_4th", U"第九 第4楽章" },
	};

	// 背景パネル
	RectF panel(500, 550, 900, 400);
	panel.draw(ColorF(0.1, 0.7, 0.9, 0.75));
	FontAsset(U"Menu")(U"🎵 Music Player").draw(540, 560, Palette::White);

	if (!audio) return -1;

	//----------------------------------------
	// 戻るボタン（パネルを閉じる）
	//----------------------------------------
	if (SimpleGUI::Button(U"← 戻る", Vec2{ 1250, 560 }))
	{
		m_showMusicPanel = false;
		const_cast<TitleScene*>(this)->m_titleState = TitleState::Title;

		// タイトルBGM再生
		if (auto& audio = data.audio)
		{
			// すべての曲が停止していたらタイトルBGM再生
			bool anyPlaying = false;
			for (const auto& [id, bgm] : audio->GetBGMMap())
			{
				if (bgm.isPlaying())
				{
					anyPlaying = true;
					break;
				}
			}

			if (!anyPlaying)
			{
				audio->PlayBGM(U"TitleBGM", true, 0s);
			}
		}
	}


	//----------------------------------------
	// 曲リストボタン
	//----------------------------------------
	for (int i = 0; i < m_playlist.size(); ++i)
	{
		const String& id = m_playlist[i];

		const String title = trackTitles.at(id);

		if (SimpleGUI::Button(title, Vec2{ 550, 600 + i * 40 }, 180))
		{
			if (audio)
			{
				const bool isSameTrack = (i == m_currentTrackIndex);

				m_currentTrackIndex = i;

				if (isSameTrack)
				{
					audio->PlayBGM(id, true);
				}
				else
				{
					audio->PlayBGM(id, true, 0s);
				}
			}
		}
	}

	//----------------------------------------
	// 再生中の曲情報表示
	//----------------------------------------
	if (m_currentTrackIndex >= 0)
	{
		const String& id = m_playlist[m_currentTrackIndex];
		const auto& map = audio->GetBGMMap();

		// 【修正3】Now Playing表示用にタイトルを取得
		const String title = trackTitles.at(id);

		if (map.contains(id))
		{
			const Audio& current = map.at(id);
			double len = current.lengthSec();
			double pos = current.posSec();

			// 再生 / 停止ボタン
			if (SimpleGUI::Button(U"▶", Vec2{ 750, 800 }))
			{
				current.play();
			}
			if (SimpleGUI::Button(U"⏹", Vec2{ 850, 800 }))
			{
				current.pause();
			}

			// シークバー
			if (SimpleGUI::Slider(U"再生位置", pos, 0.0, len, Vec2{ 550, 850 }, 100.0, 500.0))
			{
				// スライダーを動かした瞬間に反映（再生中でもOK）
				current.seekTime(pos);
			}
		}
	}

	return clickedIndex;
}

bool TitleScene::Release()
{
	return true;
}

