#pragma once

#include <Siv3D.hpp>
#include "../SceneTransition.hpp"

class MiniGameScene_3 : public App::Scene
{
public:
	MiniGameScene_3(const InitData& init);
	~MiniGameScene_3();

	void update() override;
	void draw() const override;
};
