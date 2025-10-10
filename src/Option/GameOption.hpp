#pragma once
#include "OptionBase.hpp"

class GameOption : public OptionBase
{
private:


public:
	GameOption();
	~GameOption();

	void Update() override;
	void Draw() const override;
};
