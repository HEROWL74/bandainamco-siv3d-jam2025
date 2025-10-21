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
		getData().audio->PreLoadBGM(U"MiniGame0BGM", U"assets/sound/bgm/Beethoven-Symphony-No9-1st-2013.mp3");
	}

	// 0秒〜8秒のノーツ
	m_notes.push_back({ 1.0, 0.5, 300 });
	m_notes.push_back({ 2.0, 0.7, 500 });
	m_notes.push_back({ 3.5, 1.0, 400 });
	m_notes.push_back({ 5.0, 0.5, 300 });
	m_notes.push_back({ 6.0, 0.5, 500 });
	m_notes.push_back({ 7.0, 0.5, 300 });
	m_notes.push_back({ 8.5, 1.0, 400 });

	// 10秒〜20秒のノーツ (追加分)
	m_notes.push_back({ 10.0, 0.5, 300 }); // 10秒
	m_notes.push_back({ 10.5, 0.3, 500 });
	m_notes.push_back({ 11.0, 0.2, 400 });
	m_notes.push_back({ 11.5, 0.5, 300 });
	m_notes.push_back({ 12.5, 0.5, 500 });
	m_notes.push_back({ 13.0, 0.5, 400 });
	m_notes.push_back({ 13.5, 0.5, 300 });
	m_notes.push_back({ 14.5, 2.0, 500 }); // 長いノーツ
	m_notes.push_back({ 17.0, 0.5, 300 });
	m_notes.push_back({ 17.5, 0.5, 400 });
	m_notes.push_back({ 18.0, 0.5, 500 });
	m_notes.push_back({ 19.0, 1.5, 400 }); // さらに長いノーツ

	// 20~40
	m_notes.push_back({ 20.0, 0.5, 300 });
	m_notes.push_back({ 20.5, 0.3, 400 });
	m_notes.push_back({ 21.0, 0.3, 500 });
	m_notes.push_back({ 22.0, 0.5, 300 });
	m_notes.push_back({ 22.5, 0.5, 500 });
	m_notes.push_back({ 23.0, 1.0, 400 });

	m_notes.push_back({ 25.0, 0.5, 300 });
	m_notes.push_back({ 25.5, 0.3, 400 });
	m_notes.push_back({ 26.0, 0.3, 500 });
	m_notes.push_back({ 27.0, 0.5, 300 });
	m_notes.push_back({ 27.5, 0.5, 500 });
	m_notes.push_back({ 28.0, 1.0, 400 });

	m_notes.push_back({ 30.0, 0.5, 300 });
	m_notes.push_back({ 30.5, 0.5, 400 });
	m_notes.push_back({ 31.0, 0.5, 500 });
	m_notes.push_back({ 32.0, 0.5, 300 });
	m_notes.push_back({ 32.5, 1.0, 500 }); // 少し長め

	m_notes.push_back({ 34.0, 0.5, 400 });
	m_notes.push_back({ 35.0, 0.5, 500 });
	m_notes.push_back({ 36.0, 0.5, 300 });
	m_notes.push_back({ 37.0, 0.5, 400 });
	m_notes.push_back({ 38.0, 0.5, 500 });
	m_notes.push_back({ 39.0, 1.5, 300 }); // 終盤の長いノーツ

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
			getData().audio->PlayBGM(U"MiniGame0BGM", true);
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
		changeScene(SceneState::GAME);
		getData().audio->PlayBGM(U"GameBGM", true); // ゲームシーンのBGMを再生
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
