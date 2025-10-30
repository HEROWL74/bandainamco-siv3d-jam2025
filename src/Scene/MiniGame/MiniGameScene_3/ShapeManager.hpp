#pragma once
#include <Siv3D.hpp>

class ShapeManager
{
private:
	const int32 m_densifyStep;					// 密度の間隔（大きい程粗くなる）

	Array<Polygon> m_shapes;
	Array<LineString> m_bases;					// 判定用キャッシュ

public:
	ShapeManager();
	~ShapeManager() = default;

	void GameInit(const Vec2& center, double size);											// MiniGameScene_3側で位置とサイズを決めることができる

	int Count() const { return static_cast<int32>(m_shapes.size()); }
	const Polygon& GetPolygon(int32 index) const { return m_shapes.at(index); }
	const LineString& GetDensifiedBase(int index) const { return m_bases.at(index); }

	void RecreateForCenter(const Vec2& center, double size);								// ウィンドウサイズ変更時に再設定する場合に使う
};
