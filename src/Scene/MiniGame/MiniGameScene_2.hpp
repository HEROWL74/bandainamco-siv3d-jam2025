#pragma once

#include <Siv3D.hpp>
#include "../SceneTransition.hpp"

class MiniGameScene_2 : public App::Scene
{
public:
	MiniGameScene_2(const InitData& init);
	~MiniGameScene_2();

	void update() override;
	void draw() const override;
};
