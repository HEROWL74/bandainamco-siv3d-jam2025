#include "MiniGameScene_0.hpp"
#include "../../Core/Application.hpp" // getData()を使うため追加

// コンストラクタ
MiniGameScene_0::MiniGameScene_0(const InitData& init)
	:IScene(init)
{
	loadBGMAndNotes();
}

// デストラクタ
MiniGameScene_0::~MiniGameScene_0()
{
	// BGMを停止
	m_bgm.stop();
}

void MiniGameScene_0::loadBGMAndNotes()
{
	if (getData().audio)
	{
		getData().audio->PreLoadBGM(U"MiniGame0BGM", U"assets/sound/bgm/No9_1st.mp3");
	}

	// 中難易度：テンポよくリズミカルに配置（40秒想定）
	m_notes.push_back({ 1.0, 0.5, 300 });
	m_notes.push_back({ 2.0, 0.5, 400 });
	m_notes.push_back({ 3.2, 0.5, 500 });
	m_notes.push_back({ 4.5, 0.8, 400 });

	m_notes.push_back({ 6.0, 0.5, 300 });
	m_notes.push_back({ 7.2, 0.5, 500 });
	m_notes.push_back({ 8.5, 0.5, 400 });
	m_notes.push_back({ 10.0, 1.0, 500 }); // 長押し1

	m_notes.push_back({ 12.0, 0.5, 300 });
	m_notes.push_back({ 13.0, 0.5, 400 });
	m_notes.push_back({ 14.2, 0.5, 500 });
	m_notes.push_back({ 15.5, 0.5, 300 });
	m_notes.push_back({ 16.8, 0.8, 400 });

	m_notes.push_back({ 18.5, 0.5, 500 });
	m_notes.push_back({ 19.5, 0.5, 300 });
	m_notes.push_back({ 20.8, 1.0, 400 }); // 長押し2

	m_notes.push_back({ 23.0, 0.5, 300 });
	m_notes.push_back({ 24.0, 0.5, 500 });
	m_notes.push_back({ 25.2, 0.5, 400 });
	m_notes.push_back({ 26.5, 0.5, 300 });
	m_notes.push_back({ 27.8, 0.5, 500 });

	m_notes.push_back({ 29.0, 1.0, 400 }); // やや長押し
	m_notes.push_back({ 31.0, 0.5, 300 });
	m_notes.push_back({ 32.0, 0.5, 400 });
	m_notes.push_back({ 33.2, 0.5, 500 });
	m_notes.push_back({ 34.5, 0.5, 400 });
	m_notes.push_back({ 36.0, 1.5, 300 }); // 終盤の長いノーツ



	m_status = GameStatus::Ready;
}

void MiniGameScene_0::updateReady()
{
	// クリックでゲーム開始
	if (MouseL.down())
	{
		// 修正: AudioManager経由でフェードインなしで即時再生
		if (getData().audio)
		{
			// MiniGame0BGMを再生開始 (フェードインなし)
			getData().audio->PlayBGM(U"MiniGame0BGM", true,19s);
		}

		m_gameStartTime = Scene::Time();
		m_status = GameStatus::Playing;
		m_currentNoteIndex = 0;
		m_score = 0;
		m_combo = 0;
	}
}

void MiniGameScene_0::updatePlaying()
{

	m_playerSlideY = Cursor::Pos().y; // マウスのY座標をプレイヤーのスライド位置に対応させる

	const double currentTime = Scene::Time() - m_gameStartTime; // ゲーム開始からの経過時間

	m_isPitchPerfect = false; // ピッチが合っているかどうかのフラグをリセット
	bool isNoteCurrentlyActive = false; // 現在アクティブなノーツがあるかどうか

	// ノーツの判定処理
	for (int i = m_currentNoteIndex; i < m_notes.size(); ++i)
	{
		// ノーツ情報を取得
		auto& note = m_notes[i];
		const double arrivalTime = note.startTime; // ノーツが判定ラインに到達する時間
		const double noteEndTime = note.startTime + note.duration; // ノーツの終了時間

		// ノーツが終了時間を過ぎている場合 (ミス判定が確定した場合や、ヒットして通り過ぎた場合)
		if (arrivalTime - currentTime < -noteEndTime)
		{
			// ノーツが未判定またはアクティブ状態の場合、Missに設定
			if (note.state == Note::State::None || note.state == Note::State::Active)
			{
				note.state = Note::State::Miss;
				m_combo = 0;
			}
			m_currentNoteIndex++;
			continue; // 次のノーツへ
		}

		// ノーツが判定ラインに到達したかチェック
		if (currentTime >= arrivalTime)
		{
			// ノーツが未判定の場合、アクティブ状態に変更
			if (note.state == Note::State::None)
			{
				note.state = Note::State::Active;
			}

			isNoteCurrentlyActive = true;

			// ノーツのピッチ範囲をチェック
			const int pitchCenter = note.pitch;
			const int pitchRange = 30;

			// プレイヤーの位置がノーツの範囲内かチェック
			if (m_playerSlideY >= pitchCenter - pitchRange && m_playerSlideY <= pitchCenter + pitchRange)
			{
				// ピッチHit
				m_isPitchPerfect = true;

				const int scorePerSecond = 2000;
				const double scoreToAdd = scorePerSecond * Scene::DeltaTime();

				// スコア加算
				m_score += static_cast<int>(scoreToAdd);

				// ノーツの終了判定
				if (currentTime >= noteEndTime)
				{
					// ノーツ成功判定
					note.state = Note::State::Hit;
					m_currentNoteIndex++;
				}
			}
			else
			{
				// ピッチMiss
				m_combo = 0; // コンボリセット

				// ノーツの終了判定
				if (currentTime >= noteEndTime)
				{
					note.state = Note::State::Miss;
					m_currentNoteIndex++;
				}
				// アクティブ中にMissの場合も、終了時まで状態はActiveのまま（ピッチが合えばHitに変わる可能性があるため）
			}

			break; // 一番手前のノーツの判定を行ったら、ループを抜ける
		}
	}

	if (getData().audio)
	{
		double targetPitch = 0.0; // デフォルトは通常ピッチ (0.0)

		if (isNoteCurrentlyActive)
		{
			targetPitch = m_isPitchPerfect ? 0.0 : -1.0; // ピッチが合っていれば通常ピッチ、外れていれば遅くする
		}

		// 現在の BGM ピッチを取得
		const double currentPitch = getData().audio->GetBGMPitch(U"MiniGame0BGM");

		// ピッチを徐々に目標値に近づける
		const double newPitch = Math::Lerp(currentPitch, targetPitch, 0.05);

		getData().audio->SetBGMPitch(U"MiniGame0BGM", newPitch);
	}

	// ゲーム終了判定
	if (m_currentNoteIndex >= m_notes.size() && !m_bgm.isPlaying())
	{
		m_status = GameStatus::Result;
		// changeScene(SceneState::RESULT); // 結果画面へ遷移

		// ゲーム終了時、BGMのピッチを通常に戻す
		if (getData().audio)
		{
			getData().audio->SetBGMPitch(U"MiniGame0BGM", 0.0);
		}
	}
}

void MiniGameScene_0::updateResult()
{
	// クリックでタイトルへ戻る
	if (MouseL.down())
	{
		// 共有データの取得
		auto& data = getData();
		data.nextMiniGame = (data.nextMiniGame % 4) + 1; // 次のミニゲームへ (0->1->2->3->0...)

		changeScene(SceneState::GAME);
		//data.audio->PlayBGM(U"GameBGM", true); // ゲームシーンのBGMを再生
	}
}

void MiniGameScene_0::update()
{
	Cursor::RequestStyle(CursorStyle::Hidden);
	switch (m_status)
	{
	case GameStatus::Ready:
		updateReady();
		break;
	case GameStatus::Playing:
		updatePlaying();
		break;
	case GameStatus::Result:
		updateResult();
		break;
	}
}

void MiniGameScene_0::draw() const
{
	ClearPrint();
	Scene::SetBackground(ColorF{ 0.1, 0.1, 0.2 }); // 暗い青色

	const double currentTime = Scene::Time() - m_gameStartTime;

	if (m_status == GameStatus::Playing)
	{
		// 判定ライン
		Line{ 200, 0, 200, Scene::Height() }.draw(4, Palette::Red);

		// プレイヤーのスライド位置
		Circle{ 200, m_playerSlideY, 10 }.draw(Palette::Yellow).drawFrame(2, Palette::Black);

		// ノーツの描画
		for (const auto& note : m_notes)
		{
			// 判定ライン到達までの時間（負の値だと既に過ぎたノーツ）
			const double timeToArrival = note.startTime - currentTime;

			const double x = 200.0 + (timeToArrival / m_approachTime) * (Scene::Width() - 200.0); // ノーツのX座標（時間に基づく）			
			const double lengthX = (note.duration / m_approachTime) * (Scene::Width() - 200.0); // ノーツの長さに基づく幅			
			const double y = note.pitch; // ノーツのY座標（ピッチに基づく）

			if (x + lengthX < 0) continue;// 画面外のノーツは描画しない

			//ノーツの状態に応じて色を変更
			ColorF noteColor;
			switch (note.state)
			{
			case Note::State::None:
				// 未判定 (画面遠くにあるノーツ)
				noteColor = ColorF{ 0.3, 0.7, 1.0, 0.8 }; // デフォルトの明るい青
				break;
			case Note::State::Active:
				// 判定中
				noteColor = ColorF{ 1.0, 1.0, 0.3, 0.8 }; // 黄色
				break;
			case Note::State::Hit:
				// 成功
				noteColor = ColorF{ 0.3, 1.0, 0.3, 0.8 }; // 緑
				break;
			case Note::State::Miss:
				// 失敗 (ミスしたノーツ)
				noteColor = ColorF{ 1.0, 0.3, 0.3, 0.8 }; // 赤
				break;
			}

			RectF(x, y - 15, lengthX, 30).draw(noteColor); // ノーツ本体	
			Circle{ x + 10, y, 5 }.draw(Palette::Orange);// ノーツの先端
		}
	}


	// ステータス表示
	if (m_status == GameStatus::Ready)
	{
		Print << U"MiniGame Scene 0";
		Print << U"Click to Start!";
	}
	else if (m_status == GameStatus::Playing)
	{
		Print << U"Time: " << currentTime;
		Print << U"Score: " << m_score;
	}
	else if (m_status == GameStatus::Result)
	{
		Print << U"Result! Score: " << m_score;
		Print << U"Click to return to Game Scene";
	}
}
