#pragma once
#include "OptionBase.hpp"

class TitleOption : public OptionBase
{
private:
	bool m_isExit;

	Font m_exit;

public:
	TitleOption(std::shared_ptr<Settings> settings, std::shared_ptr<AudioManager> audio);
	~TitleOption();

	void GameInit() override;
	void Update() override;
	void Draw() const override;

	bool IsExit() const { return m_isExit; }
};
