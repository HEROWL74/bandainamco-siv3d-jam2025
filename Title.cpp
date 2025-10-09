#include "Title.hpp"


// コンストラクタ
Title::Title(const InitData& init)
	:IScene(init)
{
	SystemInit();
	GameInit();
}

// デストラクタ
Title::~Title()
{
	Release();
}


bool Title::SystemInit()
{


	return true;
}

void Title::GameInit()
{

}

void Title::update()
{
	if (KeyEnter.down() || MouseL.down())
	{
		changeScene(State::GAME);
	}
}

void Title::draw() const
{
	Scene::SetBackground(ColorF{ 0.0, 0.2, 0.8 });

	
}

bool Title::Release()
{


	return true;
}

