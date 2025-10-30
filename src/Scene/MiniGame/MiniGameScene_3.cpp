#include "MiniGameScene_3.hpp"

// コンストラクタ
MiniGameScene_3::MiniGameScene_3(const InitData& init)
	:IScene(init)
	, m_playerLine(nullptr)
	, m_shapeManager(nullptr)
	, m_shapeIndex(0)
	, m_currentHausdorff(Math::Inf)
	, m_needRecalc(false)
{
	SystemInit();
	GameInit();
}


bool MiniGameScene_3::SystemInit()
{
	m_playerLine = std::make_unique<PlayerLine>();
	m_shapeManager = std::make_unique<ShapeManager>();

	m_state = PlayerState::Idle;

	m_font = Font{ FontMethod::MSDF, 24, Typeface::Heavy };

	// BGMのロード
	if (getData().audio)
	{
		getData().audio->PreLoadBGM(U"MiniGame3BGM", U"example/test.mp3");
	}

	return true;
}

void MiniGameScene_3::GameInit()
{
	const Vec2 center = Scene::Center();
	const double size = Scene::Height() / 5.0;
	m_shapeManager->GameInit(center, size);

	m_playerLine->GameInit();

	m_shapeIndex = 0;
	m_currentHausdorff = Math::Inf;
	m_needRecalc = false;
	m_time = m_timeLimit;

	// BGM再生
	getData().audio->PlayBGM(U"MiniGame3BGM", true);
}

void MiniGameScene_3::update()
{
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
	Scene::SetBackground(ColorF{ 0.0, 0.7, 0.6 }); // シアン色
	Print << U"MiniGame Scene 3: Click to return to Game Scene";

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
		m_currentHausdorff = Math::Inf;
		m_needRecalc = false;
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

	}

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
		m_currentHausdorff = Geometry2D::HausdorffDistance(base, m_playerLine->GetLine());
		m_needRecalc = false;
	}

	// しきい値以下なら次の図形へ
	const auto& userLine = m_playerLine->GetLine();
	if (userLine.size() >= static_cast<size_t>(m_minDist) && MouseL.up())
	{
		// 判定用に閉じたラインを作る（必要なら）
		LineString closedUser = userLine;
		closedUser << closedUser.front();

		const LineString& base = m_shapeManager->GetDensifiedBase(m_shapeIndex);

		// 1) 従来の hausdorff も保持しておく（任意）
		m_currentHausdorff = Geometry2D::HausdorffDistance(base, closedUser);

		// 2) 新しい厳密判定（カバー＋連続性）
		bool ok = IsStrokeVaildAsShape(closedUser, base);

		if (ok && IsFinite(m_currentHausdorff) && m_currentHausdorff <= m_hausdorffThreshold)
		{
			// 成功
			m_shapeIndex++;
			getData().audio->SetBGMPitch(U"MiniGame3BGM", 0.0);				// ピッチを元に戻す
			if (m_shapeIndex >= m_shapeManager->Count())
			{
				m_state = PlayerState::Finish;
				return;
			}
			else
			{
				m_state = PlayerState::Clear;
			}
		}
		else
		{
			// 失敗
			m_playerLine->LineClear();
			m_currentHausdorff = Math::Inf;
			m_needRecalc = false;
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
		m_currentHausdorff = Math::Inf;
		m_needRecalc = false;
		StartTimer();
		m_state = PlayerState::Playing;
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
	poly.outline().drawClosed(22, ColorF{ 0.7 });

	// プレイヤーが描く線
	m_playerLine->Draw();

	// UI
	m_font(U"Shape: {}/{}"_fmt(m_shapeIndex + 1, m_shapeManager->Count())).draw(20, Vec2{ 20, 20 });			// 現在の図形 / 最大図形数
	m_font(U"Time: {:.1f}"_fmt(GetRemainingTime())).draw(20, Vec2{ 20, 56 });									// 時間制限
	if (IsFinite(m_currentHausdorff))
	{
		m_font(U"Hausdorff: {:.1f}"_fmt(m_currentHausdorff)).draw(20, Vec2{ 20, 92 });
	}
}

// 図完成時の描画処理
void MiniGameScene_3::ClearDraw() const
{
	m_font(U"クリア！左クリックでつぎへ").drawAt(Scene::Center());
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
	m_currentHausdorff = Math::Inf;
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

	// 3) baseの各点がuserLineのどれに近いか判定
	Array<char32> visited(baseCount, 0);
	int32 visitedCount = 0;
	// userLineの点をループして近いbase点を探す
	for (const auto& u : userLine)
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
		// 近ければそのbase点を訪問済みにする
		if (bestIndex >= 0 && bestDist <= m_hausdorffThreshold)
		{
			if (!visited[bestIndex])
			{
				visited[bestIndex] = 1;
				++visitedCount;
			}
		}
	}

	const double coverage = static_cast<double>(visitedCount) / static_cast<double>(baseCount);
	if (coverage < m_coverageThreshold) return false;

	// 4) 連続カバー判定
	Array<int32> visitedIndices;
	for (int i = 0; i < baseCount; ++i)
	{
		if (visited[i]) visitedIndices << i;
	}
	if (visitedIndices.empty()) return false;

	visitedIndices.sort();	
	visitedIndices.erase(std::unique(visitedIndices.begin(), visitedIndices.end()), visitedIndices.end());		// 念のため

	// 連続して訪れたインデックス間の最大ギャップ（循環）
	int32 maxGap = 0;
	for (size_t i = 0; i < visitedIndices.size(); ++i)
	{
		int32 cur = visitedIndices[i];
		int32 nxt = visitedIndices[(i + 1) % visitedIndices.size()];
		// 円周上の方向で現在位置から次の位置までの距離を計算し、端点は含めない
		int32 gap = ((nxt - cur - 1) + baseCount) % baseCount;
		if (gap > maxGap) maxGap = gap;
	}
	int32 contiguousVisited = baseCount - maxGap;
	double contiguousRatio = static_cast<double>(contiguousVisited) / static_cast<double>(baseCount);
	if (contiguousRatio < m_contiguousThreshold) return false;

	// すべての条件をみたしたらOK
	return true;
}
