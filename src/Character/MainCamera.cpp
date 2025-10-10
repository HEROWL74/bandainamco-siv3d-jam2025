#include "MainCamera.hpp"

MainCamera::MainCamera()
	: m_position(Scene::Center()), m_target(Scene::Center())
{
}

void MainCamera::Update()
{
	m_position += (m_target - m_position) * (m_lerpFactor * Scene::DeltaTime());
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
