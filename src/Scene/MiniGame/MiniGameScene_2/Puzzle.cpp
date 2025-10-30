#include "Puzzle.hpp"

Puzzle::Puzzle()
	:m_helpFont(20)
	, m_hintFont(18)
{

}

void Puzzle::GameInit(const FilePath& path)
{
	m_img = Image(path);
	if (!m_img) return;

	m_gridN = 3;
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
			// 初期位置は左側にばらまく
			Vec2 initPos = Vec2(Random(0.0, Scene::Width() / 2.0), Random(150.0, Scene::Height() - 80.0));

			pieces.emplace_back(tex, correct, initPos);			// コンストラクタの引数を直接渡して新しい要素を構築する
			// tex:ピースの画像、correct:そのピースの正解の位置、initPos:ランダムに設定される初期位置
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
	if (KeyS.down()) ShufflePieces();
	if (KeyR.down()) ResetPuzzle();
}

void Puzzle::Draw() const
{
	// ヘルプ
	m_helpFont(U"ドラッグではめる。S: シャッフル, R: リセット").draw(10, 50, Palette::White);
	m_hintFont(U"ピースが光ったら正しい位置に近づいています").draw(10, 70, Palette::Aqua);

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

	// ピース描画（先頭が下、末尾が手前）
	for (size_t i = 0; i < pieces.size(); ++i) {
		const Piece& p = pieces[i];

		// ハイライト（近ければ）
		if (!p.m_isPlaced && p.IsNear(m_snapRange)) {
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
	if (m_grabbedIndex >= 0 && m_grabbedIndex < static_cast<int>(pieces.size())) {
		if (pieces[m_grabbedIndex].IsNear(m_snapRange)) {
			// はめ込む
			pieces[m_grabbedIndex].SnapToCorrect();
			// ここで効果音を鳴らすと気持ちいい（Sound を事前ロードして再生）
		}
		pieces[m_grabbedIndex].StopDrag();
		m_grabbedIndex = -1;
	}
}
