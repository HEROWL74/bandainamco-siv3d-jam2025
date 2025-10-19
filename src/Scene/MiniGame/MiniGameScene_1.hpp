#pragma once

#include <Siv3D.hpp>
#include "../SceneTransition.hpp"

class MiniGameScene_1 : public App::Scene
{
public:
	MiniGameScene_1(const InitData& init);
	~MiniGameScene_1();

	void update() override;
	void draw() const override;
};
