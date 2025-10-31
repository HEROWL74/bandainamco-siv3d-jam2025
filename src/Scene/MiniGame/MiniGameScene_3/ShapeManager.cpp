#include "ShapeManager.hpp"

ShapeManager::ShapeManager()
	:m_densifyStep(8)
{

}

void ShapeManager::GameInit(const Vec2& center, double size)
{
	m_shapes.clear();
	m_bases.clear();

	//コンテナに図形を格納
	m_shapes << RectF{Arg::center(center), size * 2, size}.asPolygon();								// 長方形	
	m_shapes << Shape2D::Rhombus(size * 2, size * 2, center);										// ひし形
	m_shapes << Triangle{ center, Math::Sqrt3 * size }.asPolygon();									// 正三角形
	m_shapes << Shape2D::Pentagon(size, center);													// 正五角形
	m_shapes << Shape2D::Star(size, center);														// 星
	m_shapes << Shape2D::Heart(size, center);														// ハート
	m_shapes << Shape2D::Hexagon(size, center);														// 正六角形
	m_shapes << Circle{ center, size }.asPolygon(64);												// 円
	m_shapes << Shape2D::Stairs(center + Vec2{ size, size }, size * 2, size * 2, 3, true);			// 階段
	m_shapes << Shape2D::NStar(6, size, size * 2 / 3, center);										// ギザギザの円

	m_bases.reserve(m_shapes.size());
	for (const auto& shape : m_shapes)
	{
		LineString contour = shape.outline();
		contour << contour.front();
		m_bases << contour.densified(m_densifyStep);
	}
}

void ShapeManager::RecreateForCenter(const Vec2& center, double size)
{
	GameInit(center, size);
}
