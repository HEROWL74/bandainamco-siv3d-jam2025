#pragma once

#include <Siv3D.hpp>
#include "../SceneTransition.hpp"

class MiniGameScene_0 : public App::Scene
{
public:
	MiniGameScene_0(const InitData& init);
	~MiniGameScene_0();

	void update() override;
	void draw() const override;
};
