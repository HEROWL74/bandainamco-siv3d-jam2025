#pragma once
#include <Siv3D.hpp>
#include "SceneTransition.hpp"

class Result : public App::Scene
{
private:

public:
	Result(const InitData& init);
	~Result();

	bool SystemInit();
	void GameInit();
	void update() override;
	void draw() const override;
	bool Release();

};
