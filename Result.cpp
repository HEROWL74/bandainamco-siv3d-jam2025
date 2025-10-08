#include "Result.hpp"


// コンストラクタ
Result::Result(const InitData& init)
	:IScene(init)
{
	SystemInit();
	GameInit();
}

// デストラクタ
Result::~Result()
{
	Release();
}

bool Result::SystemInit()
{


	return true;
}

void Result::GameInit()
{

}

void Result::update()
{
	if (KeyEnter.down() || MouseL.down())
	{
		changeScene(State::TITLE);
	}
}

void Result::draw() const
{
	Scene::SetBackground(ColorF{ 0.2, 0.8, 0.0 });
}

bool Result::Release()
{


	return true;
}
