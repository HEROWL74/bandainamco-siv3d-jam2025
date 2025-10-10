#include "MainCamera.hpp"

MainCamera::MainCamera()
	: m_position(Scene::Center()), m_target(Scene::Center())
{
}

void MainCamera::Update()
{
	m_position += (m_target - m_position) * (m_lerpFactor * Scene::DeltaTime());
}

Vec2 MainCamera::WorldToScreen(const Vec3& worldPos) const
{
	constexpr double tilt = 0.6;   // 見下ろし角度（0=横,1=真上）
	constexpr double scaleZ = 0.9; // 高さをどの程度潰すか

	Vec3 relative = worldPos - Vec3{ m_position.x, m_position.y, 0.0 };

	Vec2 projected = {
		relative.x,
		relative.y * (1.0 - tilt) - relative.z * scaleZ
	};

	return projected + Scene::Center();
}

void MainCamera::SetTarget(const Vec2& targetPosition)
{
	m_target = targetPosition;
}

Transformer2D MainCamera::GetTransformer() const
{
	return Transformer2D(Mat3x2::Translate(-m_position + Scene::Center()));
}

Mat3x2 MainCamera::GetViewMatrix() const
{
	return Mat3x2::Translate(-m_position + Scene::Center());
}
