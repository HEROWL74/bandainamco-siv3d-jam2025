#pragma once
#include <Siv3D.hpp>

class MainCamera
{
public:
	MainCamera();

	void Update(const Vec2& targetPosition);
	void SetTarget(const Vec2& targetPosition);
	Transformer2D GetTransformer() const;
	Mat3x2 GetViewMatrix() const;
	const Vec2& GetPosition() const;

private:
	Vec2 m_position;
	Vec2 m_target;
	double m_lerpFactor = 5.0; // 追尾の速さ
};
