#pragma once
#include "OptionBase.hpp"
#include "../Option/Settings.hpp"
#include "../Option/AudioManager.hpp"

class GameOption : public OptionBase
{
private:
	bool m_isTitle;

	Font m_returnTitle;


public:
	GameOption(std::shared_ptr<Settings> settings, std::shared_ptr<AudioManager> audio);
	~GameOption();

	void GameInit() override;
	void Update() override;
	void Draw() const override;

	bool IsTitle() const { return m_isTitle; }
};
