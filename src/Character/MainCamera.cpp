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
	Vec2 relative = Vec2{ worldPos.x, worldPos.y } - m_position;
	return relative + Scene::Center();
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
