#pragma once
#include "CharacterBase.hpp"
#include "Player.hpp"

class  Enemy : public CharacterBase
{
public:Enemy(const Texture& texture, const Player& player);

	  void Update() override;
	  void Draw() const override;
	  void InitAnimation() override;

private:
	const Player& m_player; // プレイヤー参照
	void OnAnimationEnd(const String& animName) override;
};
