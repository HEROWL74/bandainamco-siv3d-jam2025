#include "MiniGameScene_1.hpp"
#include "../../Core/Application.hpp" // getData()を使うため追加
#include "../../Effect/BubbleEffect.hpp"

// コンストラクタ
MiniGameScene_1::MiniGameScene_1(const InitData& init)
	:IScene(init)
	// メンバー変数初期化リストでフォントを一度だけ作成
	, m_font20(20)
	, m_font30(30)
	, m_font24(24)
	, m_font40(40, Typeface::Bold) // コンボ用に太字フォントを追加
{
	loadBGMAndNotes();
}

// デストラクタ
MiniGameScene_1::~MiniGameScene_1()
{
	// BGMはAudioManagerに任せる
}

void MiniGameScene_1::loadBGMAndNotes()
{
	if (getData().audio)
	{
		// 新しいBGMをロード (必要に応じて変更)
		getData().audio->PreLoadBGM(U"MiniGame1BGM", U"assets/sound/bgm/Beethoven-Symphony-No9-2nd-2023.mp3");
	}

	// 4レーン用の簡単な譜面
	// MiniGameScene_0 の Note 構造体を使っているため、3つ目の引数は 'pitch' ではなく 'lane' の意味で使います
	m_notes.push_back({ 1.0, 0.0, 0 }); // レーン0 単発
	m_notes.push_back({ 1.5, 0.0, 1 }); // レーン1 単発
	m_notes.push_back({ 2.0, 0.0, 2 }); // レーン2 単発
	m_notes.push_back({ 2.5, 0.0, 3 }); // レーン3 単発
	m_notes.push_back({ 3.0, 1.0, 1 }); // レーン1 長押し
	m_notes.push_back({ 4.5, 0.0, 0 }); // レーン0 単発
	m_notes.push_back({ 5.0, 0.0, 3 }); // レーン3 単発
	m_notes.push_back({ 5.5, 0.5, 2 }); // レーン2 長押し
	m_notes.push_back({ 6.5, 0.0, 1 }); // レーン1 単発
	m_notes.push_back({ 7.0, 0.0, 0 }); // レーン0 単発
	m_notes.push_back({ 8.0, 0.0, 2 }); // レーン2 単発
	m_notes.push_back({ 8.5, 0.0, 3 }); // レーン3 単発
	m_notes.push_back({ 9.0, 1.5, 0 }); // レーン0 長押し
	m_notes.push_back({ 11.0, 0.0, 1 }); // レーン1 単発

	m_status = GameStatus::Ready;
}

void MiniGameScene_1::updateReady()
{
	if (MouseL.down())
	{
		if (getData().audio)
		{
			// MiniGame1BGMを再生開始
			getData().audio->PlayBGM(U"MiniGame1BGM", true);
		}

		m_gameStartTime = Scene::Time();
		m_status = GameStatus::Playing;
		m_currentNoteIndex = 0;
		m_score = 0;
		m_combo = 0;
		m_activeNotes.clear();
	}
}

void MiniGameScene_1::updatePlaying()
{
	const double currentTime = Scene::Time() - m_gameStartTime; // ゲーム開始からの経過時間
	const double sceneWidth = Scene::Width();
	// GameAreaWidthはMiniGameScene_1.hppで定義されている定数
	const double gameAreaOffsetX = (sceneWidth - GameAreaWidth) / 2.0;
	// 判定処理
	for (int i = 0; i < m_notes.size(); ++i)
	{
		auto& note = m_notes[i];
		// 判定が確定したノーツは、これ以上の判定処理をスキップ
		if (note.state == Note::State::Hit || note.state == Note::State::Miss) continue;

		const double arrivalTime = note.startTime;
		const double noteEndTime = note.startTime + note.duration;

		const int lane = note.pitch;

		// 判定開始時間（単発ノーツ用: 判定ライン到達前後100ms）
		const double judgmentWindow = 0.1;
		const double judgmentStart = arrivalTime - judgmentWindow;
		const double judgmentEnd = arrivalTime + judgmentWindow;

		// 単発ノーツの判定
		if (note.duration == 0.0)
		{
			// ノーツが判定エリアに到達
			if (currentTime >= judgmentStart && currentTime <= judgmentEnd)
			{
				if (note.state == Note::State::None)
				{
					note.state = Note::State::Active; // 演出用だが、単発なので通常は即時Hit/Miss
				}

				if (m_judgmentKeys[lane].down())
				{
					// 成功判定
					note.state = Note::State::Hit;
					m_score += 100;
					m_combo++;

					const double laneCenterX = (m_laneRelativeXPositions[lane] + m_laneRelativeXPositions[lane + 1]) / 2.0 + gameAreaOffsetX;
					const Vec2 effectPos = { laneCenterX, m_judgmentLineY };

					m_effectManager.Add<BubbleEffect>(effectPos, 0.3, Random(180.0, 300.0)); // ★ エフェクト生成
				}
			}
		}
		// 長押しノーツの開始判定
		else // note.duration > 0.0
		{
			// ノーツ開始時間 (arrivalTime) 以降、終了時間 (noteEndTime) まで、キーの down() を監視する
			if (currentTime >= arrivalTime - judgmentWindow && currentTime < noteEndTime)
			{
				if (m_judgmentKeys[lane].down())
				{
					// Activeに遷移し、継続判定を開始する
					if (note.state == Note::State::None)
					{
						note.state = Note::State::Active;
						m_activeNotes.push_back(i); // アクティブなノーツのインデックスを保持
					}
				}
			}
		}


		// --- 2. 長押しノーツの継続/終了判定 ---
		if (note.duration > 0.0 && note.state == Note::State::Active)
		{
			if (currentTime < noteEndTime)
			{
				if (m_judgmentKeys[lane].pressed())
				{
					// 継続して押されている場合、スコア加算（フレーム毎）
					m_score += 10;

					const double laneCenterX = (m_laneRelativeXPositions[lane] + m_laneRelativeXPositions[lane + 1]) / 2.0 + gameAreaOffsetX;
					const Vec2 effectPos = { laneCenterX, m_judgmentLineY };

					// 毎フレームではなく、一定確率 (ここでは 30%) でエフェクトを生成し、負荷を軽減
					if (RandomBool(0.3))
					{
						m_effectManager.Add<BubbleEffect>(effectPos, 0.3, Random(180.0, 300.0));
					}
				}
				else
				{
					// 継続中にキーを離したらMiss確定
					note.state = Note::State::Miss;
					m_combo = 0;
				}
			}
			else // ノーツ終了時間 (noteEndTime) 到達
			{
				if (m_judgmentKeys[lane].pressed())
				{
					// 終了時間まで押し続けたらHit
					note.state = Note::State::Hit;
					m_score += 200

						; // 終了ボーナス
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

		// --- 3. Miss判定（単発ノーツ、および長押しノーツの最終的なMiss判定）---

		if (note.state == Note::State::None)
		{
			// 単発ノーツのMiss判定: 判定時間を過ぎた場合
			if (note.duration == 0.0 && currentTime > judgmentEnd)
			{
				note.state = Note::State::Miss;
				m_combo = 0;
			}
			// 長押しノーツのMiss判定: 終了時間を過ぎても押されなかった場合
			else if (note.duration > 0.0 && currentTime > noteEndTime)
			{
				note.state = Note::State::Miss;
				m_combo = 0;
			}
		}
	}

	// アクティブリストのクリーンアップ (今回は簡略化のためスキップ。完全な実装では必要)

	// 全ノーツが終了し、BGMも終了したらリザルトへ
	bool allNotesJudged = true;
	for (const auto& note : m_notes)
	{
		if (note.state == Note::State::None || note.state == Note::State::Active)
		{
			allNotesJudged = false;
			break;
		}
	}

	const double lastNoteEnd = m_notes.back().startTime + m_notes.back().duration;
	if (currentTime > lastNoteEnd + 1.0) // 最後のノーツの2秒後に遷移
	{
		if (getData().audio)
		{
			getData().audio->SetBGMPitch(U"MiniGame1BGM", 0.0);
			getData().audio->StopBGM(1s);
		}
		m_status = GameStatus::Result;
	}
}

void MiniGameScene_1::updateResult()
{
	// クリックでゲームへ戻る
	if (MouseL.down())
	{
		// 共有データの取得
		auto& data = getData();
		data.nextMiniGame = (data.nextMiniGame % 4) + 1; // 次のミニゲームへ (0->1->2->3->0...)

		changeScene(SceneState::GAME);
		if (getData().audio)
		{
			getData().audio->PlayBGM(U"GameBGM", true); // ゲームシーンのBGMを再生
		}
	}
}

void MiniGameScene_1::update()
{
	// MiniGameScene_0を参考に、カーソルを非表示に
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

	m_effectManager.Update();
}

RectF MiniGameScene_1::getLaneRect(int lane, double timeToArrival, double duration) const
{
	// 画面上端Y座標と判定ラインY座標 (垂直降下型に変更)
	const double topY = 0.0;
	const double nearY = m_judgmentLineY;

	// 補間率 t (0.0: 判定ライン, 1.0: 画面上端)
	double t = Clamp(timeToArrival / m_approachTime, 0.0, 1.0);

	// ノーツの先端（下端）のY座標
	const double noteStartY = Math::Lerp(nearY, topY, t);

	// レーンのX座標と幅
	const double gameAreaOffsetX = (Scene::Width() - GameAreaWidth) / 2.0; // MiniGameScene_1.hppで定義した定数を使用

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
	// 修正: note.lane は存在しないため、note.pitch を使用します。
	const int lane = note.pitch;

	// 補間率 t (0.0: 判定ライン, 1.0: 画面上端)
	double t = Clamp(timeToArrival / m_approachTime, 0.0, 1.0);

	// 画面上端Y座標と判定ラインY座標 (farY=100.0 から topY=0.0 に変更)
	const double topY = 0.0;
	const double nearY = m_judgmentLineY;

	const double gameAreaOffsetX = (Scene::Width() - GameAreaWidth) / 2.0;

	// レーンのX座標と幅 (パース計算を削除)
	// const double laneXStart = m_laneXPositions[lane]; // ← 削除 (または置き換え)
	// const double laneXEnd = m_laneXPositions[lane + 1]; // ← 削除 (または置き換え)
	const double laneXStart = m_laneRelativeXPositions[lane] + gameAreaOffsetX;
	const double laneXEnd = m_laneRelativeXPositions[lane + 1] + gameAreaOffsetX;

	const double currentWidth = laneXEnd - laneXStart;

	// ノーツの先端（下端）のY座標 (線形補間)
	const double noteStartY = Math::Lerp(nearY, topY, t);

	// ノーツの矩形
	RectF noteRect;
	noteRect.w = currentWidth;
	noteRect.x = laneXStart; // レーンの開始X座標

	// 長押しノーツ
	if (note.duration > 0.0)
	{
		const double noteEndRelativeTime = timeToArrival + note.duration;
		// ノーツの奥側（上側）のY座標 (線形補間)
		const double noteEndY = Math::Lerp(nearY, topY, Clamp(noteEndRelativeTime / m_approachTime, 0.0, 1.0));

		noteRect.y = noteEndY; // 矩形の上端
		noteRect.h = noteStartY - noteEndY; // 矩形の高さ

		// 判定ラインを過ぎたら長さを0にする
		if (noteRect.h < 0) noteRect.h = 0;
	}
	else // 単発ノーツ
	{
		const double noteHeight = 30.0; // 単発ノーツの固定の高さ
		noteRect.h = noteHeight;
		noteRect.y = noteStartY - noteHeight; // ノーツの下端が noteStartY になるように調整
	}


	// ノーツの状態に応じた色 (変更なし)
	ColorF noteColor;
	switch (note.state)
	{
	case Note::State::None:
		noteColor = ColorF{ 0.3, 0.7, 1.0, 0.8 }; // 青
		break;
	case Note::State::Active:
		noteColor = ColorF{ 1.0, 1.0, 0.3, 0.9 }; // 黄色（より鮮やかに）
		break;
	case Note::State::Hit:
		noteColor = ColorF{ 0.3, 1.0, 0.3, 0.9 }; // 緑（より鮮やかに）
		break;
	case Note::State::Miss:
		noteColor = ColorF{ 1.0, 0.3, 0.3, 0.9 }; // 赤（より鮮やかに）
		break;
	}

	// 描画 
	noteRect.draw(noteColor);

	// 判定ラインに近いノーツは少し明るくするなどの演出も可能
	if (t < 0.2)
	{
		noteRect.drawFrame(1, ColorF(1.0, 1.0, 1.0, 1.0 - t * 5.0));
	}
}


void MiniGameScene_1::draw() const
{
	ClearPrint();
	Scene::SetBackground(ColorF{ 0.1, 0.1, 0.2 }); // 暗い背景
	if (m_status == GameStatus::Playing)
	{
		const ScopedRenderStates2D blend{ BlendState::Additive };
		m_effectManager.Draw(); // ★ カメラ引数なしの Draw() を呼び出す
	}
	const double currentTime = Scene::Time() - m_gameStartTime;
	const double sceneWidth = Scene::Width();
	const double sceneHeight = Scene::Height();

	const double gameAreaOffsetX = (sceneWidth - GameAreaWidth) / 2.0; // 800.0 は MiniGameScene_1.hpp で定義した GameAreaWidth

	// 演奏レーンの描画 (垂直型に修正)
	// レーンの上端Y座標
	const double laneTopY = 0.0; // 画面上端
	const double nearY = m_judgmentLineY; // 判定ラインY座標

	// 各レーンの描画
	for (int i = 0; i < NumLanes; ++i)
	{
		const double laneX1 = m_laneRelativeXPositions[i] + gameAreaOffsetX;
		const double laneX2 = m_laneRelativeXPositions[i + 1] + gameAreaOffsetX;
		const double laneWidth = laneX2 - laneX1;

		// レーンの矩形
		const RectF laneRect(laneX1, laneTopY, laneWidth, nearY - laneTopY);

		// レーンの背景
		laneRect.draw(ColorF{ 0.05, 0.05, 0.1, 0.7 }); // 濃い青

		// レーンの境界線（手前と奥の線）を白く描画
		// 左側の境界線
		Line{ laneX1, laneTopY, laneX1, nearY }.draw(2, ColorF(0.5, 0.5, 0.5, 1.0));

		if (i == NumLanes - 1)
		{
			// 右端の境界線
			Line{ laneX2, laneTopY, laneX2, nearY }.draw(2, ColorF(0.5, 0.5, 0.5, 1.0));
		}
	}

	if (m_status == GameStatus::Playing)
	{
		// ノーツの描画
		for (int i = m_notes.size() - 1; i >= 0; --i)
		{
			const auto& note = m_notes[i];
			const double timeToArrival = note.startTime - currentTime;

			// Hit または Miss 状態のノーツは、判定ラインを一定時間過ぎたら描画をスキップ
			if (note.state == Note::State::Hit || note.state == Note::State::Miss)
			{
				const double noteEndRelativeTime = timeToArrival - note.duration;
				if (noteEndRelativeTime < -0.2)
				{
					continue;
				}
			}

			// 画面外のノーツは描画しない（判定ラインより遠い位置を対象）
			if (timeToArrival > m_approachTime + 0.5 || timeToArrival < -note.duration - 0.5) continue;

			drawNote(note, timeToArrival);
		}

		const double lineXStart = m_laneRelativeXPositions.front() + gameAreaOffsetX;
		const double lineXEnd = m_laneRelativeXPositions.back() + gameAreaOffsetX;
		Line{ lineXStart, m_judgmentLineY, lineXEnd, m_judgmentLineY }.draw(4, Palette::Red);

		// 判定キーの表示と強調
		for (int i = 0; i < NumLanes; ++i)
		{
			const String keyName = m_judgmentKeys[i].name();
			// === 変更点: オフセットを加えて中心 X 座標を計算 ===
			const double x = (m_laneRelativeXPositions[i] + m_laneRelativeXPositions[i + 1]) / 2.0 + gameAreaOffsetX;

			// キーが押されているレーンの強調表示
			if (m_judgmentKeys[i].pressed())
			{
				// 判定ライン付近を白く強調（より鮮明に）
				// === 変更点: 強調表示の X 座標にオフセットを適用 ===
				RectF(m_laneRelativeXPositions[i] + gameAreaOffsetX, m_judgmentLineY - 10, LaneWidth, 20)
					.draw(ColorF{ 1.0, 1.0, 1.0, 0.6 });
			}

			// 判定キーの文字
			m_font20(keyName).drawAt(x, m_judgmentLineY + 35, Palette::White);
		}
	}


	// ステータス表示
	if (m_status == GameStatus::Ready)
	{
		// 画面中央のX座標
		const double centerX = sceneWidth / 2.0;
		// 黒い半透明な背景パネル
		RectF(0, sceneHeight * 0.3, sceneWidth, sceneHeight * 0.4).draw(ColorF(0.0, 0.0, 0.0, 0.5));

		// タイトル
		// 修正: drawAt の第一引数に {x, y} の形式で Vec2 を渡す
		m_font30(U"MiniGame Scene 1 (Rhythm Game)").drawAt({ centerX, sceneHeight * 0.4 }, Palette::White);
		// 操作説明
		m_font20(U"Keys: {}"_fmt(m_judgmentKeys[0].name(), m_judgmentKeys[1].name(), m_judgmentKeys[2].name(), m_judgmentKeys[3].name())).drawAt({ centerX, sceneHeight * 0.5 }, Palette::White);
		// 開始メッセージ
		m_font30(U"Click to Start!").drawAt({ centerX, sceneHeight * 0.6 }, Palette::Yellow);
	}
	else if (m_status == GameStatus::Playing)
	{
		// スコアとコンボを画面上端の中央付近にまとめて配置

		// スコアパネル
		RectF(sceneWidth * 0.3, 0, sceneWidth * 0.4, 100).draw(ColorF(0.0, 0.0, 0.0, 0.7));

		// SCORE
		const double scoreCenterX = sceneWidth * 0.5;
		m_font24(U"SCORE").drawAt({ scoreCenterX, 25 }, Palette::Lightgray);
		m_font30(U"{} "_fmt(m_score)).drawAt({ scoreCenterX, 60 }, Palette::White);

		// COMBO (画面下部、レーン判定ラインの少し上に表示)
		if (m_combo > 0)
		{
			const double comboY = m_judgmentLineY - 50;
			m_font40(U"COMBO: {}"_fmt(m_combo)).drawAt({ sceneWidth / 2.0, comboY }, Palette::Yellow);
		}

		// 経過時間（左上に小さく）
		// m_font20(U"Time: {:.2f}"_fmt(currentTime)).draw(10, 10, Palette::White); // draw() を使用しているため問題なし
	}
	else if (m_status == GameStatus::Result)
	{
		// 画面中央のX座標
		const double centerX = sceneWidth / 2.0;
		// 黒い半透明な背景パネル
		RectF(0, sceneHeight * 0.3, sceneWidth, sceneHeight * 0.4).draw(ColorF(0.0, 0.0, 0.0, 0.5));

		// 結果
		m_font30(U"Result!").drawAt({ centerX, sceneHeight * 0.4 }, Palette::Lightgray);
		m_font40(U"Final Score: {}"_fmt(m_score)).drawAt({ centerX, sceneHeight * 0.5 }, Palette::Yellow);
		// 終了メッセージ
		m_font20(U"Click to return to Game Scene").drawAt({ centerX, sceneHeight * 0.6 }, Palette::White);
	}
}
