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
	// **ステップ 1: BGMの読み込み**
	// 注: ここでは直接 Audio を生成しますが、AudioManagerでStream再生する場合はそちらを使用します。
	// m_bgm = Audio(U"example/sample.mp3", Audio::Stream);
	if (getData().audio)
	{
		getData().audio->PreLoadBGM(U"MiniGame0BGM", U"example/test.mp3"); // MiniGame用IDでロード
	}

// 0秒〜8秒のノーツ (既存のもの)
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

	// ... 実際の曲に合わせて増やす

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
	
	m_playerSlideY = Cursor::Pos().y;

	const double currentTime = Scene::Time() - m_gameStartTime;

	// **ステップ 4: ノーツの判定**
	// BGMの再生位置(m_bgm.posSec())を使う方が厳密だが、ここではScene::Time()で代用
	for (int i = m_currentNoteIndex; i < m_notes.size(); ++i)
	{
		const auto& note = m_notes[i];
		const double arrivalTime = note.startTime;
		const double noteEndTime = note.startTime + note.duration;

		// ノーツが判定ラインに到達する直前の時間
		if (arrivalTime - currentTime < -0.1) // 判定を逃した場合（-0.1秒を超過したらMiss）
		{
			// Miss処理
			m_combo = 0;
			m_currentNoteIndex++; // 次のノーツへ
			// (SE再生、エフェクト表示などをここに追加)
			continue; // 次のノーツへ
		}

		// ノーツが判定ラインに到達した（または滞在中の）時間帯
		if (currentTime >= arrivalTime)
		{
			// ノーツのピッチ範囲
			const int pitchCenter = note.pitch;
			const int pitchRange = 30; // 判定の許容範囲（ピクセル）

			// プレイヤーの位置がノーツの範囲内かチェック
			if (m_playerSlideY >= pitchCenter - pitchRange && m_playerSlideY <= pitchCenter + pitchRange)
			{
				// 成功判定中
				// 長いノーツの場合、プレイヤーが範囲内にいる間は成功判定を継続
				m_combo++;
				m_score += 100; // 成功でスコア加算

				// ノーツの終了時間もチェック
				if (currentTime >= noteEndTime)
				{
					// 長いノーツが終了したら、次のノーツへ
					m_currentNoteIndex++;
				}
				// (成功SE、エフェクト表示などをここに追加)
			}
			else
			{
				// ピッチMiss（範囲外）
				m_combo = 0;
				// 長いノーツでピッチMissした場合でも、終了時間までは同じノーツをチェックし続ける
				if (currentTime >= noteEndTime)
				{
					m_currentNoteIndex++;
				}
			}

			break; // 一番手前のノーツの判定を行ったら、ループを抜ける（同時に複数のノーツを処理しないため）
		}
	}

	// **ステップ 5: ゲーム終了判定**
	if (m_currentNoteIndex >= m_notes.size() && !m_bgm.isPlaying())
	{
		m_status = GameStatus::Result;
		// changeScene(SceneState::RESULT); // 結果画面へ遷移
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

	// **ステップ 6: 描画処理**

	// 判定ラインの描画
	Line{ 0, m_judgmentLineY, Scene::Width(), m_judgmentLineY }.draw(4, Palette::Red);

	// プレイヤーのスライド位置（トロンボーンのマウスカーソル）の描画
	Circle{ 200, m_playerSlideY, 10 }.draw(Palette::Yellow).drawFrame(2, Palette::Black);

	// ノーツの描画
	for (const auto& note : m_notes)
	{
		// 判定ライン到達までの時間（負の値だと既に過ぎたノーツ）
		const double timeToArrival = note.startTime - currentTime;

		// 画面上のX座標（右から左に流れる）
		const double x = Scene::Width() - (timeToArrival / m_approachTime) * (Scene::Width() - 200.0);

		// ノーツの長さ（X方向）
		const double lengthX = (note.duration / m_approachTime) * (Scene::Width() - 200.0);

		// ノーツのY座標（ピッチ）
		const double y = note.pitch;

		// ノーツが画面外に出たら描画しない
		if (x + lengthX < 0) continue;

		RectF(x, y - 15, lengthX, 30)
			.draw(ColorF{ 0.3, 0.7, 1.0, 0.8 });
		// トロンボーンのスライド位置を示す小さな円（ノーツの目標位置）
		Circle{ x + 10, y, 5 }.draw(Palette::Orange);
	}

	// ステータス表示
	if (m_status == GameStatus::Ready)
	{
		Print << U"MiniGame Scene 0: Trombone Champ風";
		Print << U"Click to Start!";
	}
	else if (m_status == GameStatus::Playing)
	{
		Print << U"Time: " << currentTime;
		Print << U"Score: " << m_score;
		Print << U"Combo: " << m_combo;
	}
	else if (m_status == GameStatus::Result)
	{
		Print << U"Result! Score: " << m_score;
		Print << U"Click to return to Game Scene";
	}
}
