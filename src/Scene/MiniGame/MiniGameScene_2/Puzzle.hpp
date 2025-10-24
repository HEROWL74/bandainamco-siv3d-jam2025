#pragma once
#include <Siv3D.hpp>
#include "Piece.hpp"


class Puzzle
{
public:
	Puzzle();
	~Puzzle() = default;

	bool SystemInit();			// 重いリソースの初期化
	void GameInit();			// ゲーム開始時の初期化
	void Update();
	void Draw() const;

	bool IsClear();
	void ShufflePieces();
	void ResetPuzzle();

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

};
