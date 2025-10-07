#pragma once
#include "CharacterBase.hpp"

class Player : public CharacterBase
{
public:
	Player(const Texture& texture);

	void Update() override;
	void Draw() const override;

	void InitAnimation() override;
};
