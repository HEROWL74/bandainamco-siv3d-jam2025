#include "TitleOption.hpp"

TitleOption::TitleOption(std::shared_ptr<Settings> settings, std::shared_ptr<AudioManager> audio)
	:OptionBase(settings, audio)
{

}

TitleOption::~TitleOption()
{

}

void TitleOption::Update()
{
	OptionBase::Update();		// 基底クラスの処理
}

void TitleOption::Draw() const
{
	OptionBase::Draw();			// 基底クラスの描画
}
