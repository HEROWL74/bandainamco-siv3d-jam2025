#pragma once
#include <Siv3D.hpp>

class MainCamera
{
public:
	MainCamera();

	void Update();
	void SetTarget(const Vec2& targetPosition);
	[[nodiscard]] Transformer2D GetTransformer() const;
	[[nodiscard]] Mat3x2 GetViewMatrix() const;
	[[nodiscard]] const Vec2& GetPosition() const noexcept { return m_position; }

private:
	Vec2 m_position;
	Vec2 m_target;
	double m_lerpFactor = 5.0; // 追尾の速さ
};
