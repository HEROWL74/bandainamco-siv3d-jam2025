#pragma once
#include <Siv3D.hpp>
#include "SceneTransition.hpp"

class ResultScene : public App::Scene
{
private:

public:
	ResultScene(const InitData& init);
	~ResultScene();

	bool SystemInit();
	void GameInit();
	void update() override;
	void draw() const override;
	bool Release();

};
