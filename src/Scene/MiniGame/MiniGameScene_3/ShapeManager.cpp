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
	m_shapes << Shape2D::Star(size, center);
	m_shapes << Shape2D::Heart(size, center);
	m_shapes << Shape2D::Hexagon(size, center);

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
