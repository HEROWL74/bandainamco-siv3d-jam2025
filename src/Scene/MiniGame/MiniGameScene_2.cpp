#include "MiniGameScene_2.hpp"

// コンストラクタ
MiniGameScene_2::MiniGameScene_2(const InitData& init)
	:IScene(init)
	,m_puzzle(nullptr)
	,m_bigFont(48)
{
	SystemInit();
	GameInit();
}

// デストラクタ
MiniGameScene_2::~MiniGameScene_2()
{
}

bool MiniGameScene_2::SystemInit()
{
	m_puzzle = std::make_unique<Puzzle>();
	if (m_puzzle == nullptr) return false;

	if (!m_puzzle->SystemInit()) return false;
	m_puzzle->GameInit();

	return true;
}

void MiniGameScene_2::GameInit()
{
	m_fontPos = { Scene::Width() / 8.0, Scene::Height() / 4.0 };
}

void MiniGameScene_2::update()
{
	m_puzzle->Update();

	if (m_puzzle->IsClear())
	{
		// クリックでタイトルへ戻る
		if (MouseL.down())
		{
			changeScene(SceneState::GAME);
		}
	}
}

void MiniGameScene_2::draw() const
{
	ClearPrint();
	Scene::SetBackground(ColorF{ 0.8, 0.7, 0.0 }); // 黄色
	

	m_puzzle->Draw();
	if (m_puzzle->IsClear())
	{
		Print << U"MiniGame Scene 2: Click to return to Game Scene";
		m_bigFont(U"ゲームクリア！").drawAt(m_fontPos, Palette::White);
	}
}
