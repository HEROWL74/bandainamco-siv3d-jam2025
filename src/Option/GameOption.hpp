#pragma once
#include "OptionBase.hpp"
#include "../Option/Settings.hpp"
#include "../Option/AudioManager.hpp"

class GameOption : public OptionBase
{
private:


public:
	GameOption(std::shared_ptr<Settings> settings, std::shared_ptr<AudioManager> audio);
	~GameOption();

	void Update() override;
	void Draw() const override;
};
