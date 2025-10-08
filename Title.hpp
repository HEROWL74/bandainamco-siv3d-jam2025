#pragma once
#include <Siv3D.hpp>
#include "SceneTransition.hpp"
#include <vector>


class Title : public App::Scene
{
private:

public:
	Title(const InitData& init);
	~Title();

	bool SystemInit();
	void GameInit();
	void update() override;
	void draw() const override;
	bool Release();


};
