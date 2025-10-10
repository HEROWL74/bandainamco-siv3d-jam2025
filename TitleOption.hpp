#pragma once
#include "OptionBase.hpp"

class TitleOption : public OptionBase
{
private:


public:
	TitleOption();
	~TitleOption();

	void Update() override;
	void Draw() const override;
};
