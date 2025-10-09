#pragma once
#include <Siv3D.hpp>
#include "SceneTransition.hpp"
#include <vector>


class TitleScene : public App::Scene
{
private:

public:
	TitleScene(const InitData& init);
	~TitleScene();

	bool SystemInit();
	void GameInit();
	void update() override;
	void draw() const override;
	bool Release();


};
