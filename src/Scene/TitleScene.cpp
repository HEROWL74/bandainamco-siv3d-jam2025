#include "TitleScene.hpp"


// コンストラクタ
TitleScene::TitleScene(const InitData& init)
	:IScene(init)
	,m_titleOption(nullptr)
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
	}

	auto& audio = getData().audio;
	if (audio)
	{
		audio->PreLoadBGM(U"No5_1st", U"Assets/sound/bgm/No5_1st.mp3");
		audio->PreLoadBGM(U"No9_1st", U"Assets/sound/bgm/No9_1st.mp3");
		audio->PreLoadBGM(U"No9_2nd", U"Assets/sound/bgm/No9_2nd.mp3");
		audio->PreLoadBGM(U"No9_3rd", U"Assets/sound/bgm/No9_3rd.mp3");
	}

	// プレイリスト
	m_playlist = { U"No5_1st", U"No9_1st", U"No9_2nd", U"No9_3rd" };

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

	// オプションボタンの座標
	const double buttonX = 1600.0;
	const double buttonY = 950.0;
	const double buttonW = 40;
	const double ronded = 6;

	m_optionButton = RectF{ Arg::center(buttonX, buttonY), buttonW, buttonW }.rounded(ronded);

	const double musicX = 1500.0;
	const double musicY = 950.0;
	const double musicW = 40;

	m_musicButton = RectF{ Arg::center(musicX, musicY), musicW, 40 }.rounded(6);

	m_titleState = TitleState::Title;
}

void TitleScene::update()
{
	switch (m_titleState)
	{
	case TitleState::Title:

		if (KeyEnter.down())
		{
			changeScene(SceneState::GAME);
			getData().audio->StopBGM(1s);
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
}


void TitleScene::draw()const
{
	const auto& data = getData();
	if(data.isGameClear)
	{
		Scene::SetBackground(Palette::Gold);
	}
	else
	{
		Scene::SetBackground(Palette::Black);
	}

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
	m_optionIcon.scaled(0.3).drawAt(iconX, iconY);

	// オプション画面の描画
	if (m_titleState == TitleState::Option)
	{
		m_titleOption->Draw();
	}

	//  Musicボタン描画
	m_musicButton.draw(Palette::Skyblue);
	FontAsset(U"Menu")(U"Music").drawAt(m_musicButton.center(), Palette::White);

	const double musicIconX = 1500.0;
	const double musicIconY = 950.0;
	m_musicIcon.scaled(0.3).drawAt(musicIconX, musicIconY);

	// パネル表示
	if (m_showMusicPanel)
	{
		int clicked = DrawMusicPanel();
		if (clicked >= 0)
		{
			m_nextTrackIndex = clicked; // ← getData()ではなく自前変数に保存
		}
	}
}

int TitleScene::DrawMusicPanel() const
{
	const auto& data = getData();
	auto& audio = data.audio;
	int clickedIndex = -1;

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
		const bool isCurrent = (i == m_currentTrackIndex);

		if (SimpleGUI::Button(id, Vec2{ 550, 600 + i * 40 }, 180))
		{
			clickedIndex = i;

			// 曲クリックで最初から再生
			if (audio)
			{
				m_currentTrackIndex = i;
				audio->PlayBGM(id, true, 0s); // ← 再生開始位置を常に0秒に固定
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

		if (map.contains(id))
		{
			const Audio& current = map.at(id);
			double len = current.lengthSec();
			double pos = current.posSec();

			// 再生 / 停止ボタン
			if (SimpleGUI::Button(U"▶", Vec2{ 750, 600 }))
			{
				audio->PlayBGM(id, true, 0s); // ← 再生は常に最初から
			}
			if (SimpleGUI::Button(U"⏹", Vec2{ 850, 600 }))
			{
				audio->StopBGM();
			}

			// シークバー
			SimpleGUI::Slider(U"再生位置", pos, 0.0, len, Vec2{ 550, 800 }, 500);

			// 手動シーク反映（再生中のみ）
			if (MouseL.up() && len > 0)
			{
				map.at(id).seekTime(pos);
			}

			FontAsset(U"Menu")(U"Now Playing: " + id).draw(550, 710, Palette::White);
			FontAsset(U"Menu")(Format(U"{:.1f}/{:.1f} sec", pos, len)).draw(1100, 710);
		}
	}

	return clickedIndex;
}



bool TitleScene::Release()
{


	return true;
}

