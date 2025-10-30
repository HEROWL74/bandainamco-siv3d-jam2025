#include "Puzzle.hpp"
#include "../../../Effect/BubbleEffect.hpp"

Puzzle::Puzzle()
	:m_helpFont(20)
	, m_hintFont(18)
{

}

void Puzzle::GameInit(const FilePath& path)
{
	m_img = Image(path);
	if (!m_img) return;

	m_gridN = 4;
	pieces.clear();
	m_grabbedIndex = -1;
	m_tileSizeScreen = 200;
	m_margin = 2;
	m_snapRange = 36.0;

	// 元画像を正方形に切り抜く（中央トリミング）
	const int32 sourceSize = Min(m_img.width(), m_img.height());
	const int32 ox = (m_img.width() - sourceSize) / 2;
	const int32 oy = (m_img.height() - sourceSize) / 2;
	Image square = m_img.clipped(Rect(ox, oy, sourceSize, sourceSize));

	// ボードの位置をXに対して中央に配置
	const double boardWidth = m_gridN * (m_tileSizeScreen + m_margin) - m_margin;
	const double boardHeight = boardWidth;
	m_boardOrigin = Vec2{ (Scene::Width() - boardWidth) / 2.0, 80.0 };

	// 完成図をボードの背景にするための画像設定
	Image boardPreview = square.scaled(static_cast<int>(boardWidth), static_cast<int>(boardHeight));
	m_previewTex = Texture(boardPreview);

	// パズルボードのスクリーン座標上の矩形を計算
	const s3d::RectF boardRect(m_boardOrigin.x, m_boardOrigin.y, boardWidth, boardHeight);

	// 1ピースのサイズ
	m_piecePx = sourceSize / m_gridN;

	// ピース生成
	for (int y = 0; y < m_gridN; ++y) {
		for (int x = 0; x < m_gridN; ++x) {
			const Rect pieceSrc(x * m_piecePx, y * m_piecePx, m_piecePx, m_piecePx);
			Image sub = square.clipped(pieceSrc);
			Texture tex(sub); // GPU に転送（起動時一度だけ）

			// 正しい中心座標
			Vec2 correct = m_boardOrigin + Vec2(x * (m_tileSizeScreen + m_margin) + m_tileSizeScreen / 2.0,
											  y * (m_tileSizeScreen + m_margin) + m_tileSizeScreen / 2.0);

			Vec2 initPos;
			bool conflicted = true;
			int attempts = 0;
			const int maxAttempts = 100; // 試行回数の上限

			while (conflicted && attempts < maxAttempts) {
				attempts++;
				conflicted = false;

				const int area = Random(0, 2); // 0: 左、1: 上、2: 下

				if (area == 0) // 左側 (ボードの左)
				{
					initPos = Vec2(Random(150.0, boardRect.x - m_tileSizeScreen - 60.0),
								   Random(200.0, Scene::Height() - 400.0));
				}
				else if (area == 1) // 上側 (ボードの上)
				{
					initPos = Vec2(Random(150.0, Scene::Width() - 150.0),
								   Random(150.0, m_boardOrigin.y - m_tileSizeScreen - 60.0));
				}
				else // 下側 (ボードの下)
				{
					initPos = Vec2(Random(150.0, Scene::Width() - 150.0),
								   Random(boardRect.y + boardRect.h + 60.0, Scene::Height() - 400.0));
				}


				// ピースが画面外に出ないように微調整
				initPos.x = Clamp(initPos.x, m_tileSizeScreen / 2.0, Scene::Width() - m_tileSizeScreen / 2.0);
				initPos.y = Clamp(initPos.y, m_tileSizeScreen / 2.0, Scene::Height() - m_tileSizeScreen / 2.0);

				// 新しいピースの矩形
				const RectF newPieceRect(initPos.x - m_tileSizeScreen / 2.0, initPos.y - m_tileSizeScreen / 2.0, m_tileSizeScreen, m_tileSizeScreen);

				// 既存のピースと重なっていないかチェック
				for (const auto& existingPiece : pieces) {
					// 既存のピースがスナップ済み（ボード上）であれば無視
					if (existingPiece.m_isPlaced) continue;

					const RectF existingPieceRect(existingPiece.m_pos.x - m_tileSizeScreen / 2.0, existingPiece.m_pos.y - m_tileSizeScreen / 2.0, m_tileSizeScreen, m_tileSizeScreen);

					// 矩形同士が重なっているかチェック
					if (newPieceRect.intersects(existingPieceRect)) {
						conflicted = true; // 衝突あり、再試行
						break;
					}
				}
			}

			// 衝突せず、または試行回数が上限に達した場合、その位置を採用
			pieces.emplace_back(tex, correct, initPos);
		}
	}
}

void Puzzle::Update()
{
	// 入力: マウス押し下げ / 押しっぱなし / 離したをそれぞれ処理
	if (MouseL.down()) {
		MouseDown();
	}
	if (MouseL.pressed()) {
		MouseDrag();
	}
	if (MouseL.up()) {
		MouseUp();
	}

	// 滑らかにスナップする（必要なら補間で処理）
	for (auto& p : pieces) {
		if (p.m_isPlaced) {
			// 確実に正位置に補正（もし補間したければここに実装）
			p.m_pos = p.m_correctPos;
		}
	}

	// キー入力（外部からも呼べる）
#ifdef _DEBUG
	if (KeyS.down()) ShufflePieces();
	if (KeyR.down()) ResetPuzzle();
#endif // DEBUG
}

void Puzzle::Draw() const
{
	// ヘルプ
	m_helpFont(U"マウスを左クリックしてパズルををつかむよ！").draw(20, 50, Palette::Red);
	m_hintFont(U"ピースが光ったら正しい位置に近づいているよ！").draw(20, 70, Palette::Aqua);

	// ボード背景を完成図にする
	const double boardW = m_gridN * (m_tileSizeScreen + m_margin) - m_margin;
	const double boardH = boardW;
	RectF boardRect(m_boardOrigin.x, m_boardOrigin.y, boardW, boardH);

	m_previewTex.drawAt(boardRect.center(), ColorF{ 1.0, 1.0, 1.0, 0.5 });			// 完成図の描画

	boardRect.draw(ColorF(0.95, 0.95, 0.95, 0.35)); // 半透明の白を軽く重ねる
	boardRect.drawFrame(4, 0, Palette::Black);


	// マス枠
	for (int y = 0; y < m_gridN; ++y) {
		for (int x = 0; x < m_gridN; ++x) {
			RectF cell(m_boardOrigin.x + x * (m_tileSizeScreen + m_margin),
					   m_boardOrigin.y + y * (m_tileSizeScreen + m_margin),
					   m_tileSizeScreen, m_tileSizeScreen);
			cell.drawFrame(2, 0, Palette::Gray);
		}
	}

	for (const auto& p : pieces)
	{
		if (p.m_isPlaced)
		{
			// 描画
			p.Draw(m_tileSizeScreen, m_piecePx);

			// 枠
			RectF frameRect(p.m_pos.x - m_tileSizeScreen / 2.0, p.m_pos.y - m_tileSizeScreen / 2.0, m_tileSizeScreen, m_tileSizeScreen);
			frameRect.drawFrame(2, 0, Palette::Black);
		}
	}

	for (size_t i = 0; i < pieces.size(); ++i) {
		const Piece& p = pieces[i];

		if (!p.m_isPlaced)
		{
			// ハイライト（近ければ）
			if (p.IsNear(m_snapRange)) {
				RectF rect(p.m_pos.x - m_tileSizeScreen / 2.0, p.m_pos.y - m_tileSizeScreen / 2.0, m_tileSizeScreen, m_tileSizeScreen);
				rect.draw(ColorF(0.9, 1.0, 0.9, 0.8));
				rect.drawFrame(0, 4, Palette::Greenyellow);
			}

			// 描画
			p.Draw(m_tileSizeScreen, m_piecePx);

			// 枠
			RectF frameRect(p.m_pos.x - m_tileSizeScreen / 2.0, p.m_pos.y - m_tileSizeScreen / 2.0, m_tileSizeScreen, m_tileSizeScreen);
			frameRect.drawFrame(2, 0, Palette::Black);
		}
	}
}
bool Puzzle::IsClear()
{
	// クリア判定
	if (pieces.empty()) return false;
	for (auto& p : pieces)
	{
		if (!p.m_isPlaced) return false;
	}

	return true;
}

void Puzzle::ShufflePieces()
{
	for (auto& p : pieces) {
		p.m_isPlaced = false;
		p.m_pos = Vec2(Random(800.0, Scene::Width() - 100.0), Random(150.0, Scene::Height() - 80.0));
	}
	m_grabbedIndex = -1;
}

void Puzzle::ResetPuzzle()
{
	for (auto& p : pieces) {
		p.SnapToCorrect();
	}
	m_grabbedIndex = -1;
}

// 操作系の関数
void Puzzle::MouseDown()
{
	// 末尾から調べる（手前優先）
	for (int i = static_cast<int>(pieces.size()) - 1; i >= 0; --i) {
		if (pieces[i].m_isPlaced) continue; // はめ込まれているピースは掴めない
		RectF area(pieces[i].m_pos.x - m_tileSizeScreen / 2.0, pieces[i].m_pos.y - m_tileSizeScreen / 2.0,
				   m_tileSizeScreen, m_tileSizeScreen);
		if (area.intersects(Cursor::PosF())) {
			// 掴む
			m_grabbedIndex = i;
			pieces[i].StartDrag(Cursor::PosF());

			// 見た目を前面にするために末尾にムーブ
			pieces.emplace_back(pieces[i]);
			pieces.remove_at(i);
			m_grabbedIndex = static_cast<int>(pieces.size()) - 1;
			m_pickSE.playOneShot();
			break;
		}
	}
}

void Puzzle::MouseDrag()
{
	if (m_grabbedIndex >= 0 && m_grabbedIndex < static_cast<int>(pieces.size())) {
		pieces[m_grabbedIndex].DragTo(Cursor::PosF());
	}
}

void Puzzle::MouseUp()
{
	if (m_grabbedIndex == -1) return;

	// 掴んでいたピースを取得
	Piece& p = pieces[m_grabbedIndex];

	// スナップ判定（正しい位置の近くか）
	if (p.IsNear(m_snapRange))
	{
		if (p.SnapToCorrect())
		{
			PlaySnapSE(); // スナップ音を鳴らす

			if (m_effectManager)
			{
				m_effectManager->Add<BubbleEffect>(p.m_correctPos, 1.0, Random(360.0));
			}
		}
	}

	// ドラッグ状態を解除
	p.StopDrag();
	m_grabbedIndex = -1;
}
