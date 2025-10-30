#include "MiniGameScene_0.hpp"
#include "../../Core/Application.hpp" // getData()を使うため追加
#include "../../Effect/StarDustEffect.hpp"

// コンストラクタ
MiniGameScene_0::MiniGameScene_0(const InitData& init)
	:IScene(init)
	// アイコン初期化
	, m_earthIcon(U"🌎"_emoji)
	, m_starIcon(U"⭐"_emoji)
	, m_timeIcon(U"🕒"_emoji)
	, m_font20(20)
	, m_font24(24)
	, m_font64(64)
	, m_mouseImage(U"assets/image/mouse.png")
{
	loadBGMAndNotes();

	for (int i = 0; i < 300; ++i) // 300個の星を作成
	{
		m_stars.push_back({
			{ Random(0.0, (double)Scene::Width()), Random(0.0, (double)Scene::Height()) }, // 全画面にランダム配置
			Random(0.0, 0.1), // 遠景の星はゆっくり動かす
			Random(1.0, 3.0) // 星のサイズ
		});
	}
}

// デストラクタ
MiniGameScene_0::~MiniGameScene_0()
{
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
	m_playerSlideY = Cursor::Pos().y;
	// クリックでゲーム開始
	if (MouseL.down())
	{
		// BGM再生開始
		if (getData().audio)
		{
			// MiniGame0BGMを再生開始
			getData().audio->PlayBGM(U"MiniGame0BGM", true, 1s);
		}

		m_gameStartTime = Scene::Time();
		m_status = GameStatus::Countdown;
		m_currentNoteIndex = 0;
		m_score = 0;
		m_combo = 0;
		m_lastHoldEffectTime = m_gameStartTime;
	}
}

void MiniGameScene_0::updateCountdown()
{
	m_playerSlideY = Cursor::Pos().y;

	const double countdownDuration = 2.0; // 待機時間 2秒
	const double elapsed = Scene::Time() - m_gameStartTime; // Readyから経過した時間

	if (elapsed >= countdownDuration)
	{
		// BGM再生開始
		if (getData().audio)
		{
			// MiniGame0BGMを再生開始
			getData().audio->PlayBGM(U"MiniGame0BGM", true, 1s);
		}

		// ゲーム本編開始
		m_gameStartTime = Scene::Time(); // ゲーム開始時刻を再設定
		m_status = GameStatus::Playing; // Playing に遷移
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
			if (note.state == Note::State::None || note.state == Note::State::Active_Perfect || note.state == Note::State::Active_Miss)
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
				// 仮で Active_Perfect に設定（次の if-else で上書きされる）
				note.state = Note::State::Active_Perfect;
			}

			isNoteCurrentlyActive = true;

			// ノーツのピッチ範囲をチェック
			const int pitchCenter = note.pitch;
			const int pitchRange = 90;

			// プレイヤーの位置がノーツの範囲内かチェック
			if (m_playerSlideY >= pitchCenter - pitchRange && m_playerSlideY <= pitchCenter + pitchRange)
			{
				// ピッチHit
				m_isPitchPerfect = true;

				const int scorePerSecond = 2000;
				const double scoreToAdd = scorePerSecond * Scene::DeltaTime();

				// スコア加算
				m_score += static_cast<int>(scoreToAdd);

				const Vec2 effectPos = { 200.0, (double)note.pitch };
				const double lifeSpan = 0.4; // 0.4秒で収束
				// 成功色 (オレンジに近い黄色) の色相を取得
				const double baseHue = HSV(ColorF{ 1.0, 0.7, 0.3, 0.8 }).h;

				// 状態をActive_Perfectに設定
				if (note.state != Note::State::Hit)
				{
					note.state = Note::State::Active_Perfect;

					const double gameTime = Scene::Time();
					if (gameTime - m_lastHoldEffectTime >= m_holdEffectInterval)
					{
						m_effectManager.Add<StarDustEffect>(effectPos, 0.2, baseHue, m_playerSlideY); // 短いライフスパンで連続的に流れるように

						// 最後に発生した時間を更新
						m_lastHoldEffectTime = gameTime;
					}
				}

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

				// 状態をActive_Missに設定
				if (note.state != Note::State::Hit)
				{
					note.state = Note::State::Active_Miss;
				}

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
			// ノーツがアクティブな場合、ピッチが合っていれば通常ピッチ、外れていれば遅くする
			targetPitch = m_isPitchPerfect ? 0.0 : -1.0;
		}

		// 現在の BGM ピッチを取得
		const double currentPitch = getData().audio->GetBGMPitch(U"MiniGame0BGM");

		// ピッチを徐々に目標値に近づける
		const double newPitch = Math::Lerp(currentPitch, targetPitch, 0.05);

		getData().audio->SetBGMPitch(U"MiniGame0BGM", newPitch);
	}

	m_effectManager.Update();

	// ゲーム終了判定
	if (m_currentNoteIndex >= m_notes.size() && currentTime > (m_notes.back().startTime + m_notes.back().duration + 1.0))
	{
		m_status = GameStatus::Result;

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
	}
}

void MiniGameScene_0::update()
{
	// プレイヤーを地球にしたため、マウスカーソルを非表示に
	Cursor::RequestStyle(CursorStyle::Hidden);

	switch (m_status)
	{
	case GameStatus::Ready:
		updateReady();
		break;
	case GameStatus::Countdown:
		updateCountdown();
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
	// 背景色を宇宙の深遠な色に変更
	Scene::SetBackground(ColorF{ 0.05, 0.05, 0.15 });

	const double currentTime = Scene::Time() - m_gameStartTime;

	// 【移動】星の描画（パララックス効果）
	// ステータスに関わらず常に描画
	const double baseOffsetX = currentTime * 50.0; // 時間経過で右から左へ動くベースのオフセット（速度50.0）
	for (const auto& star : m_stars)
	{
		// speedRatioに応じて移動速度を調整 (0.0:遅い/遠い -> 1.0:速い/近い)
		double parallaxOffsetX = baseOffsetX * star.speedRatio;

		// 星の現在のX座標
		double starX = star.pos.x - parallaxOffsetX;

		// X座標が画面外に出たら、右端から再出現させる
		if (starX < 0) {
			starX += Scene::Width();
		}
		else if (starX > Scene::Width()) {
			starX -= Scene::Width();
		}

		// 星を描画 (遠い星は暗く小さく、近い星は明るく大きく)
		Circle{ starX, star.pos.y, star.size }
		.draw(ColorF{ 1.0, 1.0, 1.0, 0.1 + star.speedRatio * 0.7 }); // 透明度と明るさを調整
	}

	// ゲームステータスに応じた描画
	if (m_status == GameStatus::Playing || m_status == GameStatus::Ready || m_status == GameStatus::Countdown) // Ready状態を追加
	{
		// 判定ラインを光の柱（ゴールド）に変更
		Line{ 200, 0, 200, Scene::Height() }.draw(4, ColorF{ 1.0, 0.9, 0.6 });

		// プレイヤーのスライド位置（地球の絵文字）
		const double iconScale = 1.2;
		const double iconSize = m_earthIcon.size().x * iconScale;

		// プレイヤーの位置
		const Vec2 playerPos = { 200, m_playerSlideY };

		// ピッチが合っている時、光のオーラを描画 (Ready時はm_isPitchPerfectはfalseなので描画されない)
		if (m_isPitchPerfect)
		{
			// 淡い水色で光るオーラ
			Circle{ playerPos, iconSize * 0.5 + 4 }
			.drawFrame(4, ColorF{ 0.7, 0.9, 1.0, 0.7 });
		}

		// 地球のテクスチャを描画（時間と共に自転させる）
		const double rotationAngle = Scene::Time() * 30_deg; // 30度/秒で回転
		m_earthIcon.scaled(iconScale).rotated(rotationAngle).drawAt(playerPos);

		if (m_status == GameStatus::Playing) // Playing状態でのみノーツ、エフェクト、スコアパネル、プログレスバーを描画
		{
			// ノーツの描画
			for (const auto& note : m_notes)
			{
				// 判定ライン到達までの時間（負の値だと既に過ぎたノーツ）
				const double timeToArrival = note.startTime - currentTime;

				const double x = 200.0 + (timeToArrival / m_approachTime) * (Scene::Width() - 200.0); // ノーツのX座標（時間に基づく）			
				const double lengthX = (note.duration / m_approachTime) * (Scene::Width() - 200.0); // ノーツの長さに基づく幅			
				const double y = note.pitch; // ノーツのY座標（ピッチに基づく）

				if (x + lengthX < 0) continue;// 画面外のノーツは描画しない

				// ノーツの状態に応じて色を変更（宇宙テーマに合わせた色）
				ColorF noteColor;
				switch (note.state)
				{
				case Note::State::None: // 未判定
					noteColor = ColorF{ 0.4, 0.8, 1.0, 0.6 }; // 明るい青
					break;
				case Note::State::Active_Perfect: // 判定ライン到達済、または長押し中
					noteColor = ColorF{ 0.5, 0.7, 0.3, 0.8 }; // 黄緑
					break;
				case Note::State::Active_Miss: // 長押し中に離した時
					noteColor = ColorF{ 0.8, 0.3, 0.8, 0.8 }; // 赤紫
					break;
				case Note::State::Hit: // 成功
					noteColor = ColorF{ 0.3, 1.0, 0.3, 0.8 }; // 緑
					break;
				case Note::State::Miss: // ミス
					noteColor = ColorF{ 1.0, 0.3, 0.3, 0.8 }; // 赤
					break;
				}

				RoundRect(x, y - 25, lengthX, 50, 20).draw(noteColor); // ノーツ本体	

				// ノーツの先端を星の絵文字に変更
				const double starScale = 0.8; // サイズ調整
				const Vec2 starPos = { x + 15, y };
				const double starRotation = Scene::Time() * 60_deg; // 60度/秒で回転

				// 星のテクスチャを描画
				m_starIcon.scaled(starScale).rotated(starRotation).drawAt(starPos);

				m_effectManager.Draw();
			}
			// 最後のノーツの終了時刻 + 1.0秒 をゲームの総時間とする
			const double totalGameTime = m_notes.back().startTime + m_notes.back().duration + 1.0;

			// 進行度 (0.0 から 1.0 へ)
			const double progress = Min(1.0, currentTime / totalGameTime);

			// 描画設定
			const double barWidth = 800.0;
			const double barHeight = 50.0;
			const Vec2 barPos = { (Scene::Width() - barWidth) / 2.0, Scene::Height() - barHeight - 60 }; // 画面下端、中央

			// バーの背景 (ゲーム総時間)
			RectF{ barPos.x, barPos.y, barWidth, barHeight }
			.draw(ColorF{ 0.1, 0.1, 0.1, 0.8 }); // 暗い背景

			// プログレスバー本体 (現在の経過時間)
			const double currentBarWidth = barWidth * progress;
			RectF{ barPos.x, barPos.y, currentBarWidth, barHeight }
				// 経過に応じて色が暖色から寒色に変化するように (0.0=青 -> 1.0=赤)
			.draw(HSV{ 220 - (100 * progress), 0.8, 1.0 });

			// バーのフレーム
			RectF{ barPos.x, barPos.y, barWidth, barHeight }
			.drawFrame(1.0, ColorF{ 0.8, 0.8, 0.8 });

			const double timeIconScale = 1.2; // 地球アイコンと同じスケールを使用

			m_timeIcon.scaled(timeIconScale).drawAt(barPos.x - 40, barPos.y + barHeight / 2.0, Palette::White);

			const double sceneWidth = Scene::Width();
			const double scorePanelWidth = 500.0;
			const double scorePanelHeight = 150.0;
			// 画面上部中央、Y=10px の位置にパネルを配置
			const Vec2 scorePanelPos = { (sceneWidth - scorePanelWidth), 10.0 };

			// スコアパネルの背景（半透明な濃い青/紫）
			RoundRect(scorePanelPos.x, scorePanelPos.y, scorePanelWidth, scorePanelHeight, 10)
				.draw(ColorF(0.1, 0.1, 0.3, 0.8));

			// スコアパネルのフレーム（宇宙の輝きをイメージした淡い光）
			RoundRect(scorePanelPos.x, scorePanelPos.y, scorePanelWidth, scorePanelHeight, 10)
				.drawFrame(2.0, ColorF{ 0.7, 0.9, 1.0, 0.9 });


			const double scoreCenterX = scorePanelPos.x + scorePanelWidth / 2.0;

			// "スコア" ラベル表示
			m_font64(U"スコア").drawAt({ scoreCenterX, scorePanelPos.y + 30 }, Palette::Lightgray);

			// スコア表示
			m_font64(U"{:08}"_fmt(m_score)).drawAt({ scoreCenterX, scorePanelPos.y + 100 }, ColorF{ 1.0, 0.8, 0.0 });
		}
	}



	// ステータス表示
	if (m_status == GameStatus::Ready)
	{
		const double centerX = Scene::Width() / 2.0;
		const double centerY = Scene::Height() / 2.0;

		Print << U"MiniGame Scene 0";
		Print << U"Click to Start!";
		// ゲーム説明
		m_font24(U"マウスを上下に動かして、地球のピッチを合わせよう！")
			.drawAt(40, { centerX + 200, centerY - 100 }, ColorF{ 0.7, 0.9, 1.0 });

		m_font24(U"ノーツが判定ラインに来たら、同じ高さでキープ！")
			.drawAt(40, { centerX + 200, centerY - 50 }, ColorF{ 0.7, 0.9, 1.0 });

		// 開始指示 (目立つように)
		m_font64(U"マウスを左クリックでゲーム開始！")
			.drawAt(60, { centerX + 200, centerY + 150 }, ColorF{ 1.0, 0.8, 0.0 }); // オレンジ色で強調
		m_mouseImage.drawAt(Scene::Width() / 4.0, Scene::Height() / 2.0);
	}
	else if (m_status == GameStatus::Playing)
	{
		// スコアと時間表示をそのまま残す
		Print << U"Time: " << currentTime;
		Print << U"Score: " << m_score;
	}
	else if (m_status == GameStatus::Result)
	{
		Print << U"Result! Score: " << m_score;
		Print << U"Click to return to Game Scene";
	}
	else if (m_status == GameStatus::Countdown)
	{
		const double elapsed = Scene::Time() - m_gameStartTime;
		const double countdownDuration = 2.0;
		const int count = (int)Ceil(countdownDuration - elapsed);

		const double centerX = Scene::Width() * 0.5;
		const double centerY = Scene::Height() * 0.5;

		// カウントダウン表示 (3, 2, 1, GO! - 2秒なので 2, 1, GO!)
		if (count >= 1)
		{
			m_font64(count).drawAt({ centerX, centerY }, Palette::Yellow);
		}
		else if (count == 0)
		{
			m_font64(U"GO!").drawAt({ centerX, centerY }, Palette::Orange);
		}
	}
}
