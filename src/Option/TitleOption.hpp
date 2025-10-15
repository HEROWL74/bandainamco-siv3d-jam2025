#pragma once
#include "OptionBase.hpp"

class TitleOption : public OptionBase
{
private:


public:
	TitleOption(std::shared_ptr<Settings> settings, std::shared_ptr<AudioManager> audio);
	~TitleOption();

	void Update() override;
	void Draw() const override;
};
