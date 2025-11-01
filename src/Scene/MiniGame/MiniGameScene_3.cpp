#include "MiniGameScene_3.hpp"

// コンストラクタ
MiniGameScene_3::MiniGameScene_3(const InitData& init)
	:IScene(init)
	, m_playerLine(nullptr)
	, m_shapeManager(nullptr)
	, m_shapeIndex(0)
	, m_needRecalc(false)
{
	for (int i = 0; i < 300; ++i) // 300個の星を作成
	{
		const double H = Random(0.0, 360.0);
		const double S = Random(0.7, 1.0);
		const double V = Random(0.6, 1.0);

		ColorF starColor = HSV(H, S, V);

		m_stars.push_back({
			{ Random(0.0, (double)Scene::Width()), Random(0.0, (double)Scene::Height()) }, // 全画面にランダム配置
			Random(0.0, 0.1), // 遠景の星はゆっくり動かす (0.0:遠い - 1.0:近い)
			Random(2.0, 4.0), // 星のサイズ
			starColor // HSVで生成したカラフルな色を設定
		});
	}

	SystemInit();
	GameInit();
}


bool MiniGameScene_3::SystemInit()
{
	m_playerLine = std::make_unique<PlayerLine>();
	m_shapeManager = std::make_unique<ShapeManager>();
	m_movingBG = std::make_unique<MovingBackground>();

	m_font = Font{ FontMethod::MSDF, 100, Typeface::Heavy };

	// BGMのロード
	if (getData().audio)
	{
		getData().audio->PreLoadBGM(U"MiniGame3BGM", U"example/test.mp3");
	}

	// 画像のロード
	m_earth = Texture{ U"Assets/Image/Earth.png" };

	m_movingBG->SystemInit();

	return true;
}

void MiniGameScene_3::GameInit()
{
	const Vec2 center = Scene::Center();
	const double size = Scene::Height() / 2.5;
	m_shapeManager->GameInit(center, size);

	m_playerLine->GameInit();

	m_movingBG->GameInit();
	// 背景に出てくる画像達の設定
	m_movingBG->SetSpawnCount(12);
	m_movingBG->SetSpeedRange(30.0, 160.0);
	m_movingBG->SetAngularRange(-30_deg, 30_deg);
	m_movingBG->SetScaleRange(0.4, 1.2);

	m_state = PlayerState::Idle;

	m_shapeIndex = 0;
	m_needRecalc = false;
	m_time = m_timeLimit;

	m_gameStartTime = Scene::Time();

	m_earthRotateAngle = 0.0;
	m_earthRotateSpeed = 5_deg;

	m_isTimeOver = false;
	m_unpainted = false;

	// BGM再生
	getData().audio->PlayBGM(U"MiniGame3BGM", true);
}

void MiniGameScene_3::update()
{
	m_movingBG->Update();

	// 地球の回転する速度
	m_earthRotateAngle += m_earthRotateSpeed * Scene::DeltaTime();
	m_earthRotateAngle = std::fmod(m_earthRotateAngle, Math::TwoPi);			// 2πで余剰をとって値が大きくなりすぎないようにする

	// ストップウォッチ（3分で終了）
	m_isTimeOver = (m_stopwatch.s() >= 180);

	switch (m_state)
	{
	case PlayerState::Idle:
		IdleUpdate();
		break;

	case PlayerState::Playing:
		PlayingUpdate();
		break;

	case PlayerState::Clear:
		ClearUpdate();
		break;

	case PlayerState::Finish:
		FinishUpdate();
		break;	
	}
}

void MiniGameScene_3::draw() const
{
	ClearPrint();
	Scene::SetBackground(ColorF{ 0.05, 0.05, 0.15 });

	const double currentTime = Scene::Time() - m_gameStartTime; // 星の動きに使う時間

	// 星の描画（パララックス効果）
	const double baseOffsetX = currentTime * 50.0; // 時間経過で右から左へ動くベースのオフセット（速度50.0）
	for (const auto& star : m_stars)
	{
		// speedRatioに応じて移動速度を調整
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
		.draw(ColorF{ star.color.r, star.color.g, star.color.b, 0.1 + star.speedRatio * 0.7 });
	}
	//Print << U"MiniGame Scene 3: Click to return to Game Scene";

	// 背景の動くもの
	m_movingBG->Draw();

	// 下方に地球を描画
	m_earth.rotated(m_earthRotateAngle).drawAt(Scene::Width() / 2.0, Scene::Height() * 6.0 / 5.0);
		

	switch (m_state)
	{
	case PlayerState::Idle:
		IdleDraw();
		break;

	case PlayerState::Playing:
		PlayingDraw();
		break;

	case PlayerState::Clear:
		ClearDraw();
		break;

	case PlayerState::Finish:
		FinishDraw();
		break;
	}
}

// アイドル状態の時の処理関数
void MiniGameScene_3::IdleUpdate()
{
	if (KeyS.down())
	{
		m_stopwatch.start();
		StartGame();
	}
}

// プレイヤーが描いてる状態の時の処理関数
void MiniGameScene_3::PlayingUpdate()
{
	// 右クリックで時間以外リセット
	if (MouseR.down())
	{
		m_playerLine->LineClear();
		m_needRecalc = false;
	}

	// Rキーでクリアに状態遷移
	if (KeyR.down())
	{
		m_shapeIndex++;
		m_state = PlayerState::Clear;		
	}

	// 残り時間を減らす
	m_time -= Scene::DeltaTime();
	// 制限時間が0になった時
	if (m_time <= 0.0)
	{
		m_time = 0.0;

		// 音のピッチを変える演出
		const double currentPitch = getData().audio->GetBGMPitch(U"MiniGame3BGM");				// 現在の BGM ピッチを取得
		const double targetPitch = -2.5;														// 変更するピッチの目標値
		const double newPitch = Math::Lerp(currentPitch, targetPitch, 0.02);					// ピッチを徐々に目標値に近づける
		getData().audio->SetBGMPitch(U"MiniGame3BGM", newPitch);

		// 地球の回転も遅くする
		m_earthRotateSpeed = 2_deg;

	}

	// 判定用に使う userLine = merged など
	LineString userDense = m_playerLine->GetMergeLine().densified(m_shapeManager->GetDensifyStep());
	const LineString& base = m_shapeManager->GetDensifiedBase(m_shapeIndex);

	m_lastCoverage = ComputeCoverage(userDense, base, m_hausdorffThreshold);

	// 点が追加されたら再計算
	const Point pos = Cursor::Pos();
	bool added = m_playerLine->Update(pos, m_minDist);
	if (added)
	{
		m_needRecalc = true;
	}

	if (m_needRecalc && !m_playerLine->GetLine().empty())
	{
		const LineString& base = m_shapeManager->GetDensifiedBase(m_shapeIndex);
		m_needRecalc = false;
	}

	// しきい値以下なら次の図形へ
	const auto& userLine = m_playerLine->GetMergeLine();
	if (userLine.size() >= static_cast<size_t>(m_minDist) && MouseL.up())
	{
		// 判定用に閉じたラインを作る
		LineString closedUser = userLine;
		closedUser << closedUser.front();

		const LineString& base = m_shapeManager->GetDensifiedBase(m_shapeIndex);

		// なぞった率85％でクリア
		bool ok = IsStrokeVaildAsShape(closedUser, base);
		if (ok)
		{
			// 成功
			m_shapeIndex++;
			getData().audio->SetBGMPitch(U"MiniGame3BGM", 0.0);				// ピッチを元に戻す

			// 地球の速度を元に戻す
			m_earthRotateSpeed = 5_deg;

			m_state = PlayerState::Clear;
		}
		else
		{
			m_unpainted = true;
		}
	}
}

// クリア時の処理関数
void MiniGameScene_3::ClearUpdate()
{
	if (MouseL.down())
	{
		// 次の図形へ
		m_playerLine->LineClear();
		m_needRecalc = false;
		m_unpainted = false;
		StartTimer();

		if (m_shapeIndex >= m_shapeManager->Count() || m_isTimeOver)
		{
			m_state = PlayerState::Finish;
			return;
		}
		else
		{
			m_state = PlayerState::Playing;
		}
	}
}

// 図形が完成してから次の図形までの処理関数
void MiniGameScene_3::FinishUpdate()
{
	// クリックでゲームへ戻る
	if (MouseL.down())
	{		
		changeScene(SceneState::GAME);
	}
}

// 待機時の描画処理
void MiniGameScene_3::IdleDraw() const
{
	m_font(U"Sキーでスタート").drawAt(Scene::Center());
}

// プレイ時の描画処理
void MiniGameScene_3::PlayingDraw() const
{
	const Polygon& poly = m_shapeManager->GetPolygon(m_shapeIndex);
	poly.outline().drawClosed(70, ColorF{ 0.7 });

	// プレイヤーが描く線
	m_playerLine->Draw();

	// UI
	m_font(U"じかん: {:.1f}"_fmt(GetRemainingTime())).draw(80, Vec2{ 20, 45});									// 時間制限

	// 判定用に使う userLine = merged など
	const LineString& base = m_shapeManager->GetDensifiedBase(m_shapeIndex);

	// UI 表示
	ColorF fontColor = (m_lastCoverage.coverage * 100 >= 85) ? ColorF{ Palette::Yellow } : ColorF{ Palette::White };
	m_font(U"クリアまで: {:}%"_fmt(static_cast<int32>(m_lastCoverage.coverage * 100))).draw(80, Vec2{ 20, 128 }, fontColor);

	// 通るべき点（通った点）の描画
	if (m_lastCoverage.baseCount == static_cast<int32>(base.size()))
	{
		for (int i = 0; i < m_lastCoverage.baseCount; ++i)
		{
			const Vec2 p = base[i];
			if (m_lastCoverage.visited[i])
			{
				// 訪問済みは目立たせる（塗りつぶし）
				Circle{ p, 6 }.draw(ColorF{ 0.2, 0.9, 1.0, 1.0 }); // シアン、不透明
			}
			else
			{
				// 未訪問は薄めに表示
				Circle{ p, 5 }.draw(ColorF{ 0.5, 0.5, 0.5, 0.4 });
			}
		}
	}

	if (m_unpainted)
	{
		const Vec2 fontPos{ 20, 220.0 };
		m_font(U"塗り足りない！").draw(60, fontPos, ColorF{Palette::Orange});
	}
}

// 図完成時の描画処理
void MiniGameScene_3::ClearDraw() const
{	
	const Polygon& poly = m_shapeManager->GetPolygon(m_shapeIndex - 1);					// なぞった図形を取得
	const Vec2 pos{ Scene::Width() / 2.0, 0.0 };										// 描画する図形の位置
	const double s = 0.7;																// 描画する図形の縮小率
	poly.scaledAt(pos, s).draw(ColorF{Palette::Yellow});								// 図形を描画

	const Vec2 fontPos{ Scene::Width() / 2.0, Scene::Height() * 2 / 3};
	m_font(U"クリア！左クリックでつぎへ").drawAt(fontPos);
}

// ゲームクリア時の描画処理
void MiniGameScene_3::FinishDraw() const
{
	m_font(U"ゲームクリア！おめでとう！").drawAt(Scene::Center());
}


// -----------------
// ヘルパー関数たち
// -----------------
void MiniGameScene_3::StartGame()
{
	m_shapeIndex = 0;
	m_playerLine->LineClear();
	m_needRecalc = false;
	m_state = PlayerState::Playing;
	StartTimer();
}

void MiniGameScene_3::StartTimer()
{
	m_time = m_timeLimit;
}

double MiniGameScene_3::GetRemainingTime() const
{
	return Max(0.0, m_time);
}

bool MiniGameScene_3::IsStrokeVaildAsShape(const LineString& userLine, const LineString& base)
{
	const int32 baseCount = static_cast<int32>(base.size());
	if (baseCount == 0) return false;

	// 1) 最低点数チェック
	if (static_cast<int32>(userLine.size()) < m_minDist) return false;

	// 2) 始点と終点の距離チェック
	const Vec2 firstPoint = userLine.front();
	const Vec2 lastPoint = userLine.back();
	const double endDist = firstPoint.distanceFrom(lastPoint);
	if (endDist > m_minDist) return false;

	// 3) coverageを計算
	CoverageResult cr = ComputeCoverage(userLine, base, m_hausdorffThreshold);
	if (cr.coverage < m_coverageThreshold) return false;

	// すべての条件をみたしたらOK
	return true;
}


CoverageResult MiniGameScene_3::ComputeCoverage(const LineString& userLine, const LineString& base, double radius)
{ 
	CoverageResult res;
	const int32 baseCount = static_cast<int32>(base.size());
	res.baseCount = baseCount;
	if (baseCount == 0) return res;

	LineString user = userLine;

	res.visited.assign(baseCount, 0);
	int32 visitedCount = 0;

	// 各 user 点について最も近い base 点を探す
	for (const auto& u : user)
	{
		double bestDist = DBL_MAX;
		int bestIndex = -1;
		for (int i = 0; i < baseCount; ++i)
		{
			const double d = base[i].distanceFrom(u);
			if (d < bestDist)
			{
				bestDist = d;
				bestIndex = i;
			}
		}
		if (bestIndex >= 0 && bestDist <= radius)
		{
			if (!res.visited[bestIndex])
			{
				res.visited[bestIndex] = 1;
				++visitedCount;
			}
		}
	}

	res.visitedCount = visitedCount;
	res.coverage = static_cast<double>(visitedCount) / static_cast<double>(baseCount);

	// 連続カバー判定（既存ロジックを踏襲）
	Array<int32> visitedIndices;
	for (int i = 0; i < baseCount; ++i) if (res.visited[i]) visitedIndices << i;
	if (visitedIndices.empty())
	{
		res.maxGap = baseCount;
		return res;
	}

	visitedIndices.sort();
	visitedIndices.erase(std::unique(visitedIndices.begin(), visitedIndices.end()), visitedIndices.end());

	int32 maxGap = 0;
	for (size_t k = 0; k < visitedIndices.size(); ++k)
	{
		int32 cur = visitedIndices[k];
		int32 nxt = visitedIndices[(k + 1) % visitedIndices.size()];
		int32 gap = ((nxt - cur - 1) + baseCount) % baseCount;
		if (gap > maxGap) maxGap = gap;
	}
	res.maxGap = maxGap;
	int32 contiguousVisited = baseCount - maxGap;

	return res;
}
