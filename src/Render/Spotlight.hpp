// Spotlight.hpp
#pragma once
#include <Siv3D.hpp>

class Spotlight
{
public:
	const RenderTexture renderTexture;

	Spotlight(const Size& size = Scene::Size())
		: renderTexture{ size } {
	}

	void draw() const
	{
		const ScopedRenderStates2D blend{ BlendState::Multiplicative};
		renderTexture.draw();
	}
};

// ライト描画時に使うスコープ管理クラス
class ScopedSpotlight
{
public:
	ScopedSpotlight(const Spotlight& spotlight, const ColorF& color = ColorF{ 0 })
		: target{ spotlight.renderTexture.clear(color) } {
	}

private:
	const ScopedRenderTarget2D target;
	const ScopedRenderStates2D blend{ BlendState::Additive };
};
