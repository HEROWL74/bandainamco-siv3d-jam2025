#include "MiniGameScene_1.hpp"
#include "../../Core/Application.hpp" // getData()を使うため追加
#include "../../Effect/BubbleEffect.hpp"

// コンストラクタ
MiniGameScene_1::MiniGameScene_1(const InitData& init)
	:IScene(init)
	// フォント初期化
	, m_font20(20)
	, m_font30(30)
	, m_font24(24)
	, m_font64(64, Typeface::Bold)
	, m_timeIcon(U"🕒"_emoji)
	, m_keyboardTexture(U"assets/Image/Device/Keybord.png")
	, m_BackgroundTexture(U"assets/Image/Background/DinosaurBackground.jpg")
{
	m_laneAudios[0] = Audio{ GMInstrument::TaikoDrum, PianoKey::C3, 0.5s };
	m_laneAudios[1] = Audio{ GMInstrument::TaikoDrum, PianoKey::D3, 0.5s };
	m_laneAudios[2] = Audio{ GMInstrument::TaikoDrum, PianoKey::E3, 0.5s };
	m_laneAudios[3] = Audio{ GMInstrument::TaikoDrum, PianoKey::G3, 0.5s };

	m_missAudio = Audio{ GMInstrument::Piano1, PianoKey::C2, 0.3s };

	loadBGMAndNotes();
	m_dinosaurNextSpawnTime = Scene::Time() + m_dinosaurSpawnInterval;
}

// デストラクタ
MiniGameScene_1::~MiniGameScene_1()
{
}

void MiniGameScene_1::loadBGMAndNotes()
{
	if (getData().audio)
	{
		getData().audio->PreLoadBGM(U"MiniGame1BGM", U"assets/sound/bgm/No9_2nd.mp3");
	}

	// 第二楽章 ピアノ譜面（約180秒・静かで穏やかなテンポ）
	m_notes.clear();

	// --- 0〜30秒：静かな導入 ---
	m_notes.push_back({ 2.0, 0.0, 1 });
	m_notes.push_back({ 4.0, 1.0, 2 });
	m_notes.push_back({ 7.0, 0.0, 0 });
	m_notes.push_back({ 9.5, 0.0, 3 });
	m_notes.push_back({ 12.0, 0.5, 1 });
	m_notes.push_back({ 15.0, 1.0, 2 });
	m_notes.push_back({ 18.0, 0.0, 0 });
	m_notes.push_back({ 21.0, 0.0, 1 });
	m_notes.push_back({ 24.0, 1.5, 2 });
	m_notes.push_back({ 28.0, 0.0, 3 });

	// --- 30〜90秒：中盤、少し動きが出る ---
	m_notes.push_back({ 31.0, 0.0, 0 });
	m_notes.push_back({ 31.5, 0.0, 2 });
	m_notes.push_back({ 32.0, 1.0, 1 });
	m_notes.push_back({ 34.0, 0.0, 3 });
	m_notes.push_back({ 35.0, 0.0, 2 });
	m_notes.push_back({ 36.0, 0.8, 1 });
	m_notes.push_back({ 38.0, 0.0, 0 });
	m_notes.push_back({ 38.5, 0.0, 3 });
	m_notes.push_back({ 39.0, 1.0, 2 });
	m_notes.push_back({ 41.5, 0.0, 1 });
	m_notes.push_back({ 42.0, 0.0, 3 });
	m_notes.push_back({ 43.0, 0.5, 0 });
	m_notes.push_back({ 45.0, 0.0, 1 });
	m_notes.push_back({ 46.0, 1.0, 2 });
	m_notes.push_back({ 48.0, 0.0, 3 });
	m_notes.push_back({ 49.0, 0.0, 1 });
	m_notes.push_back({ 50.0, 1.0, 0 });
	m_notes.push_back({ 52.0, 0.0, 2 });
	m_notes.push_back({ 53.0, 0.0, 3 });
	m_notes.push_back({ 54.0, 0.8, 1 });
	m_notes.push_back({ 56.0, 0.0, 0 });
	m_notes.push_back({ 56.5, 0.0, 2 });
	m_notes.push_back({ 57.0, 0.5, 3 });
	m_notes.push_back({ 59.0, 0.0, 1 });
	m_notes.push_back({ 60.0, 1.0, 0 });
	m_notes.push_back({ 62.0, 0.0, 2 });
	m_notes.push_back({ 63.0, 0.0, 3 });
	m_notes.push_back({ 64.0, 1.0, 1 });
	m_notes.push_back({ 66.0, 0.0, 0 });
	m_notes.push_back({ 67.0, 0.0, 2 });
	m_notes.push_back({ 68.0, 0.8, 3 });
	m_notes.push_back({ 70.0, 0.0, 1 });
	m_notes.push_back({ 71.0, 0.0, 0 });
	m_notes.push_back({ 72.0, 1.0, 2 });
	m_notes.push_back({ 74.0, 0.0, 3 });
	m_notes.push_back({ 75.0, 0.0, 1 });
	m_notes.push_back({ 76.0, 0.5, 0 });
	m_notes.push_back({ 78.0, 0.0, 2 });
	m_notes.push_back({ 79.0, 1.0, 3 });
	m_notes.push_back({ 81.0, 0.0, 1 });
	m_notes.push_back({ 82.0, 0.0, 0 });
	m_notes.push_back({ 83.0, 0.5, 2 });
	m_notes.push_back({ 85.0, 1.0, 3 });
	m_notes.push_back({ 88.0, 0.0, 0 });
	m_notes.push_back({ 89.0, 0.0, 2 });

	// --- 90〜150秒：静かな再現部、低音中心 ---
	m_notes.push_back({ 91.0, 0.0, 0 });
	m_notes.push_back({ 92.0, 0.0, 1 });
	m_notes.push_back({ 93.0, 1.0, 2 });
	m_notes.push_back({ 95.0, 0.0, 3 });
	m_notes.push_back({ 96.5, 0.0, 1 });
	m_notes.push_back({ 98.0, 1.0, 0 });
	m_notes.push_back({ 100.0, 0.0, 2 });
	m_notes.push_back({ 101.0, 0.0, 3 });
	m_notes.push_back({ 102.0, 1.0, 1 });
	m_notes.push_back({ 104.0, 0.0, 0 });
	m_notes.push_back({ 105.0, 0.0, 2 });
	m_notes.push_back({ 106.0, 1.0, 3 });
	m_notes.push_back({ 108.0, 0.0, 1 });
	m_notes.push_back({ 109.0, 0.0, 0 });
	m_notes.push_back({ 110.0, 1.0, 2 });
	m_notes.push_back({ 112.0, 0.0, 3 });
	m_notes.push_back({ 113.0, 0.0, 1 });
	m_notes.push_back({ 114.0, 0.5, 0 });
	m_notes.push_back({ 116.0, 0.0, 2 });
	m_notes.push_back({ 117.0, 1.0, 3 });
	m_notes.push_back({ 119.0, 0.0, 1 });
	m_notes.push_back({ 120.0, 0.0, 0 });
	m_notes.push_back({ 121.0, 0.5, 2 });
	m_notes.push_back({ 123.0, 1.0, 3 });
	m_notes.push_back({ 125.0, 0.0, 0 });
	m_notes.push_back({ 126.0, 0.0, 1 });
	m_notes.push_back({ 127.0, 1.0, 2 });
	m_notes.push_back({ 129.0, 0.0, 3 });
	m_notes.push_back({ 130.0, 0.0, 1 });
	m_notes.push_back({ 131.0, 1.0, 0 });
	m_notes.push_back({ 133.0, 0.0, 2 });
	m_notes.push_back({ 134.0, 0.0, 3 });
	m_notes.push_back({ 135.0, 1.0, 1 });
	m_notes.push_back({ 137.0, 0.0, 0 });
	m_notes.push_back({ 138.0, 0.0, 2 });
	m_notes.push_back({ 139.0, 1.0, 3 });
	m_notes.push_back({ 141.0, 0.0, 1 });
	m_notes.push_back({ 142.0, 0.0, 0 });
	m_notes.push_back({ 143.0, 1.0, 2 });
	m_notes.push_back({ 145.0, 0.0, 3 });
	m_notes.push_back({ 146.0, 0.0, 1 });
	m_notes.push_back({ 147.0, 0.5, 0 });
	m_notes.push_back({ 149.0, 1.0, 2 });

	// --- 150〜180秒：穏やかに終わるフェードアウト部 ---
	m_notes.push_back({ 150.0, 0.0, 1 });
	m_notes.push_back({ 151.0, 0.0, 2 });
	m_notes.push_back({ 152.0, 1.0, 3 });
	m_notes.push_back({ 154.0, 0.0, 0 });
	m_notes.push_back({ 155.0, 0.0, 2 });
	m_notes.push_back({ 156.0, 1.0, 1 });
	m_notes.push_back({ 158.5, 0.0, 0 });
	m_notes.push_back({ 160.0, 0.0, 3 });
	m_notes.push_back({ 161.0, 0.0, 2 });
	m_notes.push_back({ 162.0, 1.0, 1 });
	m_notes.push_back({ 165.0, 0.0, 0 });
	m_notes.push_back({ 166.5, 0.0, 2 });
	m_notes.push_back({ 168.0, 2.0, 3 });

	m_status = GameStatus::Ready;
}

void MiniGameScene_1::updateReady()
{
	if (KeySpace.down())
	{
		m_gameStartTime = Scene::Time();
		m_status = GameStatus::Countdown;
		m_currentNoteIndex = 0;
		m_score = 0;
		m_combo = 0;
		m_activeNotes.clear();
		m_isFullCombo = true; // フルコンボフラグをリセット
	}
}

void MiniGameScene_1::updateCountdown()
{
	const double countdownDuration = 2.0; // 待機時間 2秒
	const double elapsed = Scene::Time() - m_gameStartTime;

	if (elapsed >= countdownDuration)
	{
		if (getData().audio)
		{
			// BGMの再生開始
			getData().audio->PlayBGM(U"MiniGame1BGM", true);
			m_clearSE = Audio(U"assets/sound/se/PuzzlePiace_Clear.mp3");
		}

		// ゲーム本編開始
		m_gameStartTime = Scene::Time(); // ゲーム開始時刻を再設定
		m_status = GameStatus::Playing; // Playing に遷移
	}
}

void MiniGameScene_1::updatePlaying()
{
	const double currentTime = Scene::Time() - m_gameStartTime; // ゲーム開始からの経過時間
	const double sceneWidth = Scene::Width();
	const double gameAreaOffsetX = (sceneWidth - GameAreaWidth) / 2.0;

	const double perfectWindow = 0.3; // Perfect判定の境界線
	const double totalWindow = 0.5; // Miss判定の境界線

	// ノーツの判定処理
	for (int i = 0; i < m_notes.size(); ++i)
	{
		auto& note = m_notes[i];
		// 判定が確定したノーツはスキップ
		if (note.state == Note::State::Hit || note.state == Note::State::Miss) continue;

		const double arrivalTime = note.startTime;
		const double noteEndTime = note.startTime + note.duration;
		const int lane = note.pitch;

		const double perfectStart = arrivalTime - perfectWindow / 2.0;
		const double totalEnd = arrivalTime + totalWindow / 2.0;

		// 単発ノーツの判定
		if (note.duration == 0.0)
		{
			// ノーツが判定可能エリア内にある場合
			if (currentTime >= perfectStart && currentTime <= totalEnd)
			{
				// 判定可能エリアに入ったらActive_Perfectに遷移
				if (note.state == Note::State::None)
				{
					note.state = Note::State::Active_Perfect;
				}

				// Active_Perfectの状態でキーが押されたらHit
				if (m_judgmentKeys[lane].down())
				{
					// 判定成功 (Perfectとして扱う)
					note.state = Note::State::Hit;
					m_combo++;
					m_score += 300; // 一律高得点

					// サウンド再生
					m_laneAudios[lane].playOneShot(2.0);

					// エフェクト生成
					const double laneCenterX = (m_laneRelativeXPositions[lane] + m_laneRelativeXPositions[lane + 1]) / 2.0 + gameAreaOffsetX;
					const Vec2 effectPos = { laneCenterX, m_judgmentLineY };
					m_effectManager.Add<BubbleEffect>(effectPos, 0.3, Random(180.0, 300.0));
				}
			}
			// ノーツが判定時間を過ぎた場合のMiss判定
			else if (currentTime > totalEnd && note.state != Note::State::Hit && note.state != Note::State::Miss)
			{
				note.state = Note::State::Miss;
				m_combo = 0;
				m_isFullCombo = false;
				m_missAudio.playOneShot(1.0);
			}
		}
		// 長押しノーツの開始判定
		else // note.duration > 0.0
		{
			// ノーツ開始判定時間内
			if (currentTime >= perfectStart && currentTime < noteEndTime)
			{
				if (m_judgmentKeys[lane].down())
				{
					// Active_Perfectに遷移し、継続判定を開始
					if (note.state == Note::State::None)
					{
						note.state = Note::State::Active_Perfect;
						m_activeNotes.push_back(i);
					}
				}
			}
			// 終了時間を過ぎても押されなかった場合のMiss判定
			else if (currentTime > noteEndTime && note.state == Note::State::None)
			{
				note.state = Note::State::Miss;
				m_combo = 0;
				m_isFullCombo = false;
				m_missAudio.playOneShot(1.0);
			}
		}


		// 長押しノーツの継続判定
		if (note.duration > 0.0 && note.state == Note::State::Active_Perfect)
		{
			if (currentTime < noteEndTime)
			{
				if (m_judgmentKeys[lane].pressed())
				{
					// 継続して押されている場合、スコア加算（フレーム毎）
					m_score += 10;

					const double gameTime = Scene::Time();
					if (gameTime - m_lastHoldEffectTime >= m_holdEffectInterval)
					{
						m_laneAudios[lane].playOneShot(1.5);

						const double laneCenterX = (m_laneRelativeXPositions[lane] + m_laneRelativeXPositions[lane + 1]) / 2.0 + gameAreaOffsetX;
						const Vec2 effectPos = { laneCenterX, m_judgmentLineY };
						m_effectManager.Add<BubbleEffect>(effectPos, 0.3, Random(180.0, 300.0));

						// 最後に発生した時間を更新
						m_lastHoldEffectTime = gameTime;
					}
				}
				else
				{
					// 継続中にキーを離したらコンボを切ってActive_Missに遷移
					note.state = Note::State::Active_Miss;
					m_combo = 0;

					m_missAudio.playOneShot(1.0);
				}
			}
			else // ノーツ終了時間 (noteEndTime) 到達
			{
				if (m_judgmentKeys[lane].pressed())
				{
					// 終了時間まで押し続けたらHit
					note.state = Note::State::Hit;
					m_score += 200; // 終了ボーナス
					m_combo++;
				}
				else
				{
					// 終了時点で離していたらMiss
					note.state = Note::State::Miss;
					m_combo = 0;
				}
			}
		}
		// 長押しノーツのActive_Miss中の判定
		else if (note.duration > 0.0 && note.state == Note::State::Active_Miss)
		{
			// Active_Missに遷移したら、キーを押し直してもMissのまま。
			if (currentTime > noteEndTime)
			{
				note.state = Note::State::Miss;
			}
		}
	}

	// 全ノーツが終了し、指定時間経過でリザルトへ
	const double lastNoteEnd = m_notes.back().startTime + m_notes.back().duration;
	if (currentTime > lastNoteEnd + 1.0) // 最後のノーツの1秒後に遷移
	{
		m_status = GameStatus::Result;
	}
}

void MiniGameScene_1::updateResult()
{
	if (!m_clearEffectPlayed)
	{
		// エフェクトを生成
		m_effectManager.Add<GameClearEffect>(Scene::Center(), 1.5);

		// SEを再生
		m_clearSE.playOneShot();

		m_clearEffectPlayed = true;
	}
	// クリックでゲームへ戻る
	if (MouseL.down())
	{
		// 共有データの更新
		auto& data = getData();
		data.nextScene = (data.nextScene % 5) + 1;

		changeScene(SceneState::GAME);
	}
}

void MiniGameScene_1::update()
{
	// カーソル非表示
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

	m_effectManager.Update();

	const double deltaTime = Scene::DeltaTime();
	const double sceneWidth = Scene::Width();

	if (m_status == GameStatus::Playing)
	{
		// 恐竜の移動更新
		for (auto& dino : m_movingDinosaurs)
		{
			if (dino.movingRight)
			{
				dino.x += dino.speed * deltaTime;
			}
			else
			{
				dino.x -= dino.speed * deltaTime;
			}
		}

		// 画面外に出た恐竜の削除
		m_movingDinosaurs.remove_if([sceneWidth](const MovingDinosaur& dino)
		{
			// 移動方向に応じて画面外判定
			if (dino.movingRight)
			{
				// 右移動: 右端を完全に超えたら削除
				return dino.x > sceneWidth;
			}
			else
			{
				// 左移動: 左端を完全に超えたら削除
				return dino.x < -dino.texture.width() * dino.scale;
			}
		});

		if (Scene::Time() >= m_dinosaurNextSpawnTime)
		{
			if (m_movingDinosaurs.empty())
			{
				MovingDinosaur newDino; // 恐竜の種類をインデックスで決定 (ローテーション)

				const int typeCount = 9;
				newDino.type = static_cast<DinosaurType>(m_dinosaurOrderIndex);
				m_dinosaurOrderIndex = (m_dinosaurOrderIndex + 1) % typeCount;
				m_nextDinosaurType = static_cast<DinosaurType>(m_dinosaurOrderIndex); // 次の出現予定タイプ

				newDino.movingRight = RandomBool(); // 移動方向
				newDino.speed = Random(400.0, 500.0);
				newDino.scale = 2.0; // デフォルトスケール
				newDino.spawnTime = Scene::Time();

				// 種類ごとの設定
				switch (newDino.type)
				{
				case DinosaurType::Tyrannosaurus:
					newDino.texture = dinos[0]; break;
				case DinosaurType::Triceratops:
					newDino.texture = dinos[1]; break;
				case DinosaurType::Tyrannosaurus_hair:
					newDino.texture = dinos[2]; break;
				case DinosaurType::Trex_kokkaku:
					newDino.texture = dinos[3]; break;
				case DinosaurType::Pteranodon:
					newDino.texture = dinos[4]; break;
				case DinosaurType::Pachycephalosaurus:
					newDino.texture = dinos[5]; break;
				case DinosaurType::Stegosaurus:
					newDino.texture = dinos[6]; newDino.scale = 2.4; break;
				case DinosaurType::Brachiosaurus:
					newDino.texture = dinos[7]; newDino.scale = 1.6; break;
				case DinosaurType::Iguanodon:
					newDino.texture = dinos[8]; newDino.scale = 2.4; break;
				default:
					newDino.texture = dinos[0]; break;
				}

				newDino.x = newDino.movingRight
					? -newDino.texture.width() * newDino.scale   // 左端
					: sceneWidth;                                // 右端

				m_movingDinosaurs.push_back(newDino);

				// 次の出現タイミング
				m_dinosaurNextSpawnTime = Scene::Time() + m_dinosaurSpawnInterval + Random(-1.0, 3.0);
			}
			else
			{
				// 恐竜が残ってる場合は少し待つ
				m_dinosaurNextSpawnTime = Scene::Time() + 0.5;
			}
		}

	}
}

// レーンの描画
void MiniGameScene_1::drawLanes() const
{
	const double sceneWidth = Scene::Width();
	const double gameAreaOffsetX = (sceneWidth - GameAreaWidth) / 2.0;

	// レーンの上端Y座標
	const double laneTopY = 0.0;
	const double nearY = m_judgmentLineY; // 判定ラインY座標

	// 各レーンの描画
	for (int i = 0; i < NumLanes; ++i)
	{
		const double laneX1 = m_laneRelativeXPositions[i] + gameAreaOffsetX;
		const double laneX2 = m_laneRelativeXPositions[i + 1] + gameAreaOffsetX;
		const double laneWidth = laneX2 - laneX1;

		// レーンの矩形
		const RectF laneRect(laneX1, laneTopY, laneWidth, nearY - laneTopY);

		// レーンの背景 (グラデーション)
		laneRect.draw(Arg::top = ColorF{ 0.05, 0.0, 0.1, 0.8 },
					  Arg::bottom = ColorF{ 0.15, 0.05, 0.2, 0.9 });

		// 左側の境界線
		Line{ laneX1, laneTopY, laneX1, nearY }.draw(4, ColorF(0.0, 1.0, 0.7, 0.3));

		if (i == NumLanes - 1)
		{
			// 右端の境界線
			Line{ laneX2, laneTopY, laneX2, nearY }.draw(4, ColorF(0.0, 1.0, 0.7, 0.3));
		}
	}
}

RectF MiniGameScene_1::getLaneRect(int lane, double timeToArrival, double duration) const
{
	// 画面上端Y座標と判定ラインY座標
	const double topY = 0.0;
	const double nearY = m_judgmentLineY;

	// 補間率 t (0.0: 判定ライン, 1.0: 画面上端)
	double t = Clamp(timeToArrival / m_approachTime, 0.0, 1.0);

	// ノーツの先端（下端）のY座標
	const double noteStartY = Math::Lerp(nearY, topY, t);

	// レーンのX座標と幅
	const double gameAreaOffsetX = (Scene::Width() - GameAreaWidth) / 2.0;

	const double laneXStart = m_laneRelativeXPositions[lane] + gameAreaOffsetX;
	const double laneXEnd = m_laneRelativeXPositions[lane + 1] + gameAreaOffsetX;

	const double currentWidth = laneXEnd - laneXStart;

	RectF noteRect;
	noteRect.w = currentWidth;
	noteRect.x = laneXStart;

	if (duration > 0.0) // 長押しノーツ
	{
		const double noteEndRelativeTime = timeToArrival + duration;
		// ノーツの奥側（上側）のY座標
		const double noteEndY = Math::Lerp(nearY, topY, Clamp(noteEndRelativeTime / m_approachTime, 0.0, 1.0));

		noteRect.y = noteEndY; // 上側の座標
		noteRect.h = noteStartY - noteEndY; // 長さ
		if (noteRect.h < 0) noteRect.h = 0;
	}
	else // 単発ノーツ
	{
		const double noteHeight = 30.0;
		noteRect.h = noteHeight;
		noteRect.y = noteStartY - noteHeight; // 下端が noteStartY になるように調整
	}

	return noteRect;
}

// ノーツの描画
void MiniGameScene_1::drawNote(const Note& note, double timeToArrival) const
{
	const int lane = note.pitch;
	double t = Clamp(timeToArrival / m_approachTime, 0.0, 1.0);
	const double topY = 0.0;
	const double nearY = m_judgmentLineY;
	const double gameAreaOffsetX = (Scene::Width() - GameAreaWidth) / 2.0;
	const double laneXStart = m_laneRelativeXPositions[lane] + gameAreaOffsetX;
	const double laneXEnd = m_laneRelativeXPositions[lane + 1] + gameAreaOffsetX;
	const double currentWidth = laneXEnd - laneXStart;
	const double noteStartY = Math::Lerp(nearY, topY, t);

	RectF noteRect;
	noteRect.w = currentWidth;
	noteRect.x = laneXStart;

	// ノーツの状態に応じた色を設定
	ColorF noteColor;
	if (note.duration > 0.0) // 長押しノーツの場合
	{
		switch (note.state)
		{
		case Note::State::None:
			noteColor = ColorF{ 1.0, 0.8, 0.2, 1.0 }; // 通常色: オレンジ
			break;
		case Note::State::Active_Perfect:
			noteColor = ColorF{ 1.0, 1.0, 0.2, 1.0 }; // 黄色
			break;
		case Note::State::Active_Miss:
			noteColor = ColorF{ 0.8, 0.3, 0.8, 1.0 }; // 赤紫
			break;
		case Note::State::Hit:
			noteColor = ColorF{ 0.3, 1.0, 0.3, 1.0 }; // 緑
			break;
		case Note::State::Miss:
			noteColor = ColorF{ 1.0, 0.2, 0.2, 1.0 }; // 赤
			break;
		}
	}
	else // 単発ノーツの場合
	{
		switch (note.state)
		{
		case Note::State::None:
			noteColor = ColorF{ 0.2, 0.6, 1.0, 1.0 }; // 通常色: 青
			break;
		case Note::State::Active_Perfect:
			noteColor = ColorF{ 1.0, 1.0, 0.2, 1.0 }; // 黄色
			break;
		case Note::State::Active_Miss:
			noteColor = ColorF{ 0.8, 0.3, 0.8, 1.0 }; // 赤紫
			break;
		case Note::State::Hit:
			noteColor = ColorF{ 0.3, 1.0, 0.3, 1.0 }; // 緑
			break;
		case Note::State::Miss:
			noteColor = ColorF{ 1.0, 0.2, 0.2, 1.0 }; // 赤
			break;
		}
	}

	const double cornerRadius = 10.0; // 角丸の半径

	if (note.duration > 0.0) // 長押しノーツ
	{
		const double noteEndRelativeTime = timeToArrival + note.duration;
		const double noteEndY = Math::Lerp(nearY, topY, Clamp(noteEndRelativeTime / m_approachTime, 0.0, 1.0));

		noteRect.y = noteEndY;
		noteRect.h = noteStartY - noteEndY;

		if (noteRect.h > 0)
		{
			// 本体を角丸矩形で描画 (透明度 50%)
			RoundRect(noteRect, cornerRadius).draw(noteColor * 0.5);

			// 枠線
			RoundRect(noteRect, cornerRadius).drawFrame(2, 0, noteColor);

			// 先端（下端）の強調 (角丸矩形)
			RoundRect(laneXStart, noteStartY - 10, currentWidth, 10, cornerRadius)
				.draw(noteColor);
		}
	}
	else // 単発ノーツ
	{
		const double noteHeight = 80.0; // ノーツの高さ
		noteRect.h = noteHeight;
		noteRect.y = noteStartY - noteHeight;

		// 本体を角丸矩形で描画
		RoundRect(noteRect, cornerRadius).draw(noteColor);

		// 枠線 (判定ラインに近いノーツは白く光る)
		RoundRect(noteRect, cornerRadius)
			.drawFrame(1, 0, ColorF(1.0, 1.0, 1.0, 1.0 - t * 3.0));
	}
}


void MiniGameScene_1::draw() const
{
	ClearPrint();
	if (m_BackgroundTexture)
	{
		m_BackgroundTexture.scaled(Scene::Width() / m_BackgroundTexture.width()).draw();
	}

	const double currentTime = Scene::Time() - m_gameStartTime;
	const double sceneWidth = Scene::Width();
	const double sceneHeight = Scene::Height();

	const double gameAreaOffsetX = (sceneWidth - GameAreaWidth) / 2.0;

	// レーン、判定ライン、鍵盤UIの描画
	if (m_status == GameStatus::Playing || m_status == GameStatus::Countdown)
	{
		if (m_status == GameStatus::Playing)
		{
			// 恐竜の描画と移動アニメーション
			for (const auto& dino : m_movingDinosaurs)
			{
				// 恐竜の上下移動の計算
				const double elapsedTime = Scene::Time() - dino.spawnTime;
				const double frequency = 5.0;
				const double amplitude = 10.0 * dino.scale; // 振幅
				const double sinValue = std::sin(elapsedTime * frequency);
				const double yOffset = sinValue * amplitude;

				// 回転角度の計算
				const double maxAngle = 2.5_deg * dino.scale;
				double rotationAngle = sinValue * maxAngle;

				const Vec2 textureSize = dino.texture.size() * dino.scale;

				double drawY;

				if (dino.type == DinosaurType::Pteranodon)
				{
					// 画面上部を基準に描画
					const double flyHeightBase = Scene::Height() * 0.2;
					const double flyAmplitude = Scene::Height() * 0.05;

					drawY = flyHeightBase + sinValue * flyAmplitude;

					// 飛行感を強調するための回転
					rotationAngle = sinValue * 8.0_deg * dino.scale;
				}
				else
				{
					// 地上を歩く恐竜のY座標 (画面下端に合わせる)
					const double groundY = sceneHeight - dino.texture.height() * dino.scale;
					drawY = groundY + yOffset;
				}

				const Vec2 center =
				{
					dino.x + textureSize.x / 2.0,
					// 描画中心座標の調整
					(dino.type == DinosaurType::Pteranodon) ? (drawY + textureSize.y - 420) : (drawY + textureSize.y - 350)
				};

				// 移動方向に応じて回転角度を反転
				if (dino.movingRight)
				{
					rotationAngle = -rotationAngle;
				}

				if (dino.movingRight)
				{
					dino.texture.scaled(dino.scale).rotated(rotationAngle).drawAt(center);
				}
				else
				{
					dino.texture.scaled(dino.scale).mirrored().rotated(rotationAngle).drawAt(center);
				}
			}
		}

		// 各レーンの描画
		drawLanes();

		if (m_status == GameStatus::Playing)
		{
			// ノーツの描画
			for (int i = m_notes.size() - 1; i >= 0; --i)
			{
				const auto& note = m_notes[i];
				const double timeToArrival = note.startTime - currentTime;

				// 判定確定ノーツの描画スキップ判定
				if (note.state == Note::State::Hit || note.state == Note::State::Miss)
				{
					const double noteEndRelativeTime = timeToArrival - note.duration;
					if (noteEndRelativeTime < -0.2)
					{
						continue;
					}
				}

				// 画面外のノーツは描画しない
				if (timeToArrival > m_approachTime + 0.5 || timeToArrival < -note.duration - 0.5) continue;

				drawNote(note, timeToArrival);
			}
		}

		// 判定ラインの描画
		const double lineXStart = m_laneRelativeXPositions.front() + gameAreaOffsetX;
		const double lineXEnd = m_laneRelativeXPositions.back() + gameAreaOffsetX;
		Line{ lineXStart, m_judgmentLineY, lineXEnd, m_judgmentLineY }.draw(4, Palette::Red);


		// 鍵盤UIの描画

		const double KeyHeight = 70.0;
		const double KeyY = m_judgmentLineY + 10.0;
		const double KeyFrameThickness = 2.0;


		// 判定キーの表示と鍵盤の描画
		for (int i = 0; i < NumLanes; ++i)
		{
			const String keyName = m_judgmentKeys[i].name();
			const double xCenter = (m_laneRelativeXPositions[i] + m_laneRelativeXPositions[i + 1]) / 2.0 + gameAreaOffsetX;
			const double laneXStart = m_laneRelativeXPositions[i] + gameAreaOffsetX;

			const bool isPressed = m_judgmentKeys[i].pressed();


			// 1. レーン強調表示 (判定ライン上)
			if (isPressed)
			{
				const double laneTopY = 0.0;
				const double highlightHeight = m_judgmentLineY;

				RectF(laneXStart, laneTopY, LaneWidth, highlightHeight)
					// 上端: 暗く, 下端: 明るく
					.draw(Arg::top = ColorF{ 1.0, 1.0, 1.0, 0.1 },
						  Arg::bottom = ColorF{ 1.0, 1.0, 1.0, 0.7 });
			}

			// 2. 鍵盤（白鍵）の描画
			const ColorF keyColor = isPressed ? Palette::Pink : Palette::White;
			const ColorF textColor = isPressed ? Palette::Black : Palette::Gray;

			// 鍵盤本体
			RectF(laneXStart, KeyY, LaneWidth, KeyHeight)
				.draw(keyColor);

			// 鍵盤の枠線
			RectF(laneXStart, KeyY, LaneWidth, KeyHeight)
				.drawFrame(KeyFrameThickness, 0, Palette::Black);

			// 鍵盤上のキー名
			m_font64(keyName).drawAt(xCenter, KeyY + KeyHeight / 2.0, textColor);
		}
	}


	// ステータス表示
	if (m_status == GameStatus::Ready)
	{
		// 画面中央のX座標
		const double centerX = sceneWidth / 2.0;

		// 黒い半透明な背景パネル
		RectF(0, sceneHeight * 0.3, sceneWidth, sceneHeight * 0.4).draw(ColorF(0.0, 0.0, 0.0, 0.5));
		m_keyboardTexture.scaled(0.5).drawAt({ centerX, sceneHeight * 0.3 });

		// 開始メッセージ
		m_font64(U"スペースキーを押すと始まるよ！").drawAt({ centerX, sceneHeight * 0.62 }, Palette::Yellow);
	}
	// カウントダウン表示
	else if (m_status == GameStatus::Countdown)
	{
		const double elapsed = Scene::Time() - m_gameStartTime;
		const double countdownDuration = 2.0;
		const int count = (int)Ceil(countdownDuration - elapsed);

		const double centerX = sceneWidth * 0.5;
		const double centerY = m_judgmentLineY - 150;

		// カウントダウン表示 (3, 2, 1, GO!)
		if (count >= 1)
		{
			m_font64(count).drawAt({ centerX, centerY }, Palette::Yellow);
		}
		else if (count == 0)
		{
			m_font64(U"GO!").drawAt({ centerX, centerY }, Palette::Orange);
		}
	}

	else if (m_status == GameStatus::Playing)
	{
		// スコア表示
		const double scoreBaseY = 30.0;
		const double centerX = sceneWidth * 0.5;

		// 画面中央上部にパネルを描画
		RectF(sceneWidth * 0.3, 0, sceneWidth * 0.4, 120).draw(ColorF(0.0, 0.0, 0.0, 0.6));

		// SCORE ラベル
		m_font24(U"スコア").drawAt({ centerX, scoreBaseY + 10 }, Palette::Lightgray);

		// スコア本体
		m_font64(U"{} "_fmt(m_score)).draw(Arg::center = Vec2(centerX, scoreBaseY + 55), ColorF(1.0, 0.7, 0.1));

		// COMBO 表示 (画面下部)
		if (m_combo > 0)
		{
			const double comboY = m_judgmentLineY + 120;

			const String comboText = U"コンボ : {}"_fmt(m_combo);

			// テキストのサイズを取得
			const double textWidth = m_font64(comboText).region().w;
			const double textHeight = m_font64(comboText).region().h;

			const double padding = 20.0; // パネルの余白

			// パネルの矩形を計算
			const double panelWidth = textWidth + padding * 2;
			const double panelHeight = textHeight + padding;
			const double panelX = sceneWidth / 2.0 - panelWidth / 2.0;
			const double panelY = comboY - panelHeight / 2.0;

			// パネル（黒い半透明な角丸矩形）を描画
			RoundRect(panelX, panelY, panelWidth, panelHeight, 10.0)
				.draw(ColorF(0.0, 0.0, 0.0, 0.6))
				.drawFrame(2, 0, ColorF(0.5, 1.0, 1.0));

			// コンボテキスト本体
			m_font64(comboText).drawAt({ sceneWidth / 2.0, comboY }, Palette::Yellow);
		}

		// TimeBar の描画
		const double lastNoteEnd = m_notes.back().startTime + m_notes.back().duration;
		const double TotalGameDuration = lastNoteEnd + 1.0;

		const double progress = Clamp(currentTime / TotalGameDuration, 0.0, 1.0); // 進行度
		const double barHeight = 50.0;
		const double barY = 50.0;
		const double barX = 80.0;
		const double barWidth = 400.0;
		const double emojiSize = 70.0;
		const double emojiPadding = 5.0;

		m_timeIcon
			.scaled(emojiSize / m_timeIcon.height())
			.draw(
				barX - emojiSize - emojiPadding,
				barY + barHeight / 2.0 - emojiSize / 2.0
			);

		// バーの背景と進捗部分の描画
		RoundRect(barX, barY, barWidth, barHeight, 5.0).draw(ColorF(0.1, 0.1, 0.1, 0.8));
		RoundRect(barX, barY, barWidth * progress, barHeight, 5.0).draw(ColorF(0.0, 1.0, 0.5));

		const double timeTextCenterX = barX + barWidth / 2.0;
		const double drawY = barY + barHeight + 12;
	}
	else if (m_status == GameStatus::Result)
	{
		// 画面中央のX座標
		const double centerX = sceneWidth / 2.0;
		// 黒い半透明な背景パネル
		RectF(0, sceneHeight * 0.3, sceneWidth, sceneHeight * 0.4).draw(ColorF(0.0, 0.0, 0.0, 0.5));

		// 結果
		m_font30(U"ゲームクリア！").drawAt({ centerX, sceneHeight * 0.4 }, Palette::Lightgray);
		m_font64(U"スコア : {}"_fmt(m_score)).drawAt({ centerX, sceneHeight * 0.5 }, Palette::Yellow);

		// フルコンボ表示
		if (m_isFullCombo)
		{
			m_font64(U"フルコンボ！").drawAt({ centerX, sceneHeight * 0.45 }, Palette::Orange);
		}

		// 終了メッセージ
		m_font20(U"マウスを左クリックすると戻れるよ！").drawAt({ centerX, sceneHeight * 0.6 }, Palette::White);
	}

	// エフェクトの描画
	m_effectManager.Draw();
}
