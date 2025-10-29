#pragma once
#include <Siv3D.hpp>
#include "Piece.hpp"
#include "../../../Effect/EffectManager.hpp"

class Puzzle
{
public:
	Puzzle();
	~Puzzle() = default;

	void GameInit(const FilePath& path);
	void Update();
	void Draw() const;

	bool IsClear();
	void ShufflePieces();
	void ResetPuzzle();
	const Texture& GetPrevTexture() const { return m_previewTex; }

	void SetSE(const Audio& pick, const Audio& snap) { m_pickSE = pick; m_snapSE = snap; }
	void PlayPickSE() const { if (m_pickSE) m_pickSE.playOneShot(); }
	void PlaySnapSE() const { if (m_snapSE) m_snapSE.playOneShot(); }

	void SetEffectManager(EffectManager* em) { m_effectManager = em; }
private:

	// 操作系の関数
	void MouseDown();
	void MouseDrag();
	void MouseUp();

	// 設定
	int32 m_gridN{ 3 };								// パズルの縦横ピース数
	int32 m_tileSizeScreen{ 200 };					// タイルのスクリーン上のサイズ
	int32 m_margin{ 10 };							// タイル間のマージン
	int32 m_piecePx{ 0 };							// 元画像の1ピースのピクセルサイズ
	double m_snapRange{ 36.0 };						// スナップ範囲
	Vec2 m_boardOrigin{ 300, 100 };					// ボードの左上座標

	// フォント
	Font m_helpFont;
	Font m_hintFont;

	Image m_img;
	Texture m_previewTex;

	// ピース群
	Array<Piece> pieces;

	// 入力管理
	int32 m_grabbedIndex{ -1 };						// 掴んでいるピースのインデックス

	// SE
	Audio m_pickSE;									// ピースを拾う時のSE
	Audio m_snapSE;									// ピースがスナップする時のSE

	EffectManager* m_effectManager = nullptr;
};
