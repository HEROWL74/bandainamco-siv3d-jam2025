#include "MiniGameScene_2.hpp"
#include "../../Effect/GameClearEffect.hpp"

// コンストラクタ
MiniGameScene_2::MiniGameScene_2(const InitData& init)
	:IScene(init)
	, m_puzzle(nullptr)
	, m_state(State::Idle)
	, m_bigFont(48)
	, m_puzzleIndex(0)
	, m_timer(0.0)
	, m_background(U"assets/Image/Background/2050036.png") // 背景画像テクスチャのロード
{
	const FilePath animal1Path = U"assets/Image/Animal/bird_aoitori_bluebird.png";
	const FilePath animal2Path = U"assets/Image/Animal/eto_uma_furikaeri.png";
	const FilePath animal3Path = U"assets/Image/Animal/eto_remake_ushi.png";

	m_animals.emplace_back(Texture(animal1Path), AnimalBoundary::UpperHalf, AnimalBoundary::FullWidth, 60.0);
	m_animals.emplace_back(Texture(animal2Path), AnimalBoundary::LowerHalf, AnimalBoundary::LeftHalf, 60.0);
	m_animals.emplace_back(Texture(animal3Path), AnimalBoundary::LowerHalf, AnimalBoundary::RightHalf, 60.0);

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

	m_puzzle->SetEffectManager(&m_effectManager);

	// 使う画像をロード(歴史順)
	m_puzzleImages = {
		U"Assets/Image/Portrait/bach.jpg",
		U"Assets/Image/Portrait/Mozart.jpg",
		U"Assets/Image/Portrait/Beethoven.jpg",
	};

	m_mouseImage = Texture(U"assets/Image/Mouse_LightClick.png");

	// BGMのプリロード
	if (getData().audio)
	{
		getData().audio->PreLoadBGM(U"MiniGame2BGM", U"assets/sound/bgm/No9_3rd.mp3");

		try
		{
			// ピースを拾う音
			const Audio pickSE(U"assets/sound/se/PuzzlePiece_Pick.mp3");

			// ピースを嵌める音
			const Audio snapSE(U"assets/sound/se/PuzzlePiece_Snap.mp3");

			// パズルクリア音
			m_clearSE = Audio(U"assets/sound/se/PuzzlePiace_Clear.mp3");

			// PuzzleにSEインスタンスを渡す
			m_puzzle->SetSE(pickSE, snapSE);
		}
		catch (const Error& e)
		{
			Print << U"Error loading SE files: " << e.what();
		}
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

	const double halfScreenX = Scene::Width() / 2.0;
	const double halfScreenY = Scene::Height() / 2.0;

	if (m_animals.size() >= 3)
	{
		// 鳥
		m_animals[0].position() = { Random(100.0, Scene::Width() - 100.0), Random(100.0, halfScreenY - 100.0) };
		m_animals[0].setScale(0.5);

		// 馬
		m_animals[1].position() = { Random(100.0, halfScreenX - 100.0), Random(halfScreenY + 100.0, Scene::Height() - 100.0) };
		m_animals[1].setScale(0.8);

		//牛
		m_animals[2].position() = { Random(halfScreenX + 100.0, Scene::Width() - 100.0), Random(halfScreenY + 100.0, Scene::Height() - 100.0) };
		m_animals[2].setScale(1.0);
	}
}

void MiniGameScene_2::update()
{
	const double deltaTime = Scene::DeltaTime();

	for (auto& animal : m_animals)
	{
		animal.update(deltaTime);
	}

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

	m_effectManager.Update();
}

void MiniGameScene_2::draw() const
{
	ClearPrint();

	// 背景の描画
	m_background.scaled(1.3).drawAt(Scene::Center());

	// 動物の描画処理を背景の次に実行
	DrawAnimals();

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

	m_effectManager.Draw();
}

// 動物の描画処理の実装
void MiniGameScene_2::DrawAnimals() const
{
	const double elapsedTime = Scene::Time(); // 現在のシーン経過時間

	for (const auto& animal : m_animals)
	{
		animal.draw(elapsedTime);
	}
}


// 待機状態の更新処理
void MiniGameScene_2::IdleUpdate()
{
	if (MouseL.down())
	{
		m_state = State::Playing;
		m_stopwatch.restart();
	}
}

// プレイ状態の更新処理
void MiniGameScene_2::PlayingUpdate()
{
	m_puzzle->Update();

	// 絵が完成したら状態を遷移
	if (m_puzzle->IsClear())
	{
		// パズル完成SEの再生
		if (m_clearSE)
		{
			m_clearSE.playOneShot(0.8);
		}

		m_effectManager.Add<GameClearEffect>(Scene::Center(), 1.5);

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
		// 180秒以上経過しているか判定
		const bool isTimeOver = (m_stopwatch.s() >= 180); // 180秒以上経過しているか

		++m_puzzleIndex;

		// まだ画像が残っている場合、かつ2分経過していない場合
		if (m_puzzleIndex < static_cast<int32>(m_puzzleImages.size()) && !isTimeOver)
		{
			m_puzzle->GameInit(m_puzzleImages[m_puzzleIndex]);
			m_state = State::Playing;
		}
		// 画像が残ってない、または2分以上経過している場合
		else
		{
			// 2分経過していたら、強制的にゲームクリア状態 (Finish) へ
			// 2分経過していなくても、最後のパズルをクリアしたら Finish へ
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
		const Vec2 center = Scene::Center();

		m_bigFont(U"マウスを左クリックしてパズルのピースをつかむよ！").drawAt(60,center.movedBy(0, -300), Palette::Black);

		if (m_mouseImage)
		{
			m_mouseImage.scaled(0.8).drawAt(center);
		}

		m_bigFont(U"左クリックでスタート").drawAt(center.movedBy(0, 300), Palette::Red);
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
	preview.scaled(1.2).drawAt(center, ColorF{ 1.0, 1.0, 1.0, 0.9 });

	m_bigFont(U"クリア！クリックで次へ").drawAt(center.movedBy(0, 0), ColorF{ Palette::Red });
}

// ゲームクリアした状態の更描画処理
void MiniGameScene_2::FinishDraw() const
{
	const Vec2 fontPos = { Scene::Width() / 8.0, Scene::Height() / 4.0 };
	const Vec2 center = Scene::Center();
	m_bigFont(U"ゲームクリア！").drawAt(center, Palette::Red);
	m_bigFont(U"左クリックで戻ろう！").drawAt(center.movedBy(0, 250), Palette::Red);
}
