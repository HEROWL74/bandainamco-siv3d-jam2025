#include "GameOption.hpp"

GameOption::GameOption(std::shared_ptr<Settings> settings, std::shared_ptr<AudioManager> audio)
	:OptionBase(settings, audio)
{

}

GameOption::~GameOption()
{

}

void GameOption::Update()
{
	OptionBase::Update();
}

void GameOption::Draw() const
{
	OptionBase::Draw();
}
