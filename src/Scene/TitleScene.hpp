#pragma once
#include <Siv3D.hpp>
#include "SceneTransition.hpp"
#include "../Option/TitleOption.hpp"
#include <vector>

enum class TitleState
{
	Title,
	Option,
	Exit,
};

class TitleScene : public App::Scene
{
private:
	Texture m_optionIcon;
	RoundRect m_optionButton;

	TitleState m_titleState;

	std::unique_ptr<TitleOption> m_titleOption;

public:
	TitleScene(const InitData& init);
	~TitleScene();

	bool SystemInit();
	void GameInit();
	void update() override;
	void draw() const override;
	bool Release();


};
