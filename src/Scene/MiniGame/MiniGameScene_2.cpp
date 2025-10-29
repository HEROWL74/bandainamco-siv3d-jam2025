#include "MiniGameScene_2.hpp"

// コンストラクタ
MiniGameScene_2::MiniGameScene_2(const InitData& init)
	:IScene(init)
	,m_puzzle(nullptr)
	,m_state(State::Idle)
	,m_bigFont(48)
	,m_puzzleIndex(0)
	,m_timer(0.0)
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

	// 使う画像をロード
	m_puzzleImages = {
		U"Assets/Beethoven.jpg",
		U"Assets/TestPuzzle.jpeg",
		U"Assets/TestPuzzle1.jpg",
	};

	if (getData().audio)
	{
		getData().audio->PreLoadBGM(U"MiniGame2BGM", U"assets/sound/bgm/No9_3rd.mp3");
	}

	return true;
}

void MiniGameScene_2::GameInit()
{
	m_state = State::Idle;
	m_puzzleIndex = 0;
	m_timer = 0.0;

	if (!m_puzzleImages.empty())
	{
		m_puzzle->GameInit(m_puzzleImages[m_puzzleIndex]);
	}

	// BGMの再生を追加
	if (getData().audio)
	{
		getData().audio->PlayBGM(U"MiniGame2BGM", true);
	}
}

void MiniGameScene_2::update()
{
	switch (m_state)
	{
	case State::Idle:
		IdleUpdate();
		break;

	case State::Playing:
		PlayingUpdate();
		break;

	case State::Clear:
		ClearUpdate();
		break;

	case State::Finish:
		FinishUpdate();
		break;
	}
}

void MiniGameScene_2::draw() const
{
	ClearPrint();
	Scene::SetBackground(ColorF{ 0.8, 0.7, 0.0 }); // 黄色

	switch (m_state)
	{
	case State::Idle:
		IdleDraw();
		break;

	case State::Playing:
		PlayingDraw();
		break;

	case State::Clear:
		ClearDraw();
		break;

	case State::Finish:
		FinishDraw();
		break;
	}
}

// 待機状態の更新処理
void MiniGameScene_2::IdleUpdate()
{
	if (KeyS.down())
	{
		m_state = State::Playing;
	}
}

// プレイ状態の更新処理
void MiniGameScene_2::PlayingUpdate()
{
	m_puzzle->Update();

	// 絵が完成したら状態を遷移
	if (m_puzzle->IsClear())
	{
		m_state = State::Clear;
		m_timer = 0.0;
	}
}

// 絵が完成してる状態の更新処理
void MiniGameScene_2::ClearUpdate()
{
	m_timer += Scene::DeltaTime();

	if (m_timer > 0.5 && MouseL.down())
	{
		++m_puzzleIndex;

		// まだ画像が残っている場合
		if (m_puzzleIndex < static_cast<int32>(m_puzzleImages.size()))
		{
			m_puzzle->GameInit(m_puzzleImages[m_puzzleIndex]);
			m_state = State::Playing;
		}
		// 画像が残ってない場合
		else
		{
			m_state = State::Finish;
		}
	}
}

// ゲームクリアした状態の更新処理
void MiniGameScene_2::FinishUpdate()
{
	// クリックでタイトルへ戻る
	if (MouseL.down())
	{
		if (getData().audio)
		{
			getData().audio->StopBGM(1s); // フェードアウトしながら停止 (フェード時間は任意)
		}

		changeScene(SceneState::GAME);
	}
}

// 待機状態の描画処理
void MiniGameScene_2::IdleDraw() const
{

}

// プレイ状態の描画処理
void MiniGameScene_2::PlayingDraw() const
{
	m_puzzle->Draw();
}

// 絵が完成してる状態の描画処理
void MiniGameScene_2::ClearDraw() const
{
	const Texture& preview = m_puzzle->GetPrevTexture();
	const Vec2 center = Scene::Center();
	preview.scaled(0.6).draw(center, ColorF{ 1.0, 1.0, 1.0, 0.9 });

	m_bigFont(U"クリア！クリックで次へ").drawAt(center.movedBy(0, 250), ColorF{ Palette::White });
}

// ゲームクリアした状態の更描画処理
void MiniGameScene_2::FinishDraw() const
{
	Print << U"MiniGame Scene 2: Click to return to Game Scene";

	const Vec2 fontPos = { Scene::Width() / 8.0, Scene::Height() / 4.0 };
	m_bigFont(U"ゲームクリア！").drawAt(fontPos, Palette::White);
}
