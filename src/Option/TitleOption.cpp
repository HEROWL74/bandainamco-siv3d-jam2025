#include "TitleOption.hpp"

TitleOption::TitleOption()
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
