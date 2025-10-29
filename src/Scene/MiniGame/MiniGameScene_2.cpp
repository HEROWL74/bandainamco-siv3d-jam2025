#include "MiniGameScene_2.hpp"

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

	m_animals.emplace_back(Texture(animal1Path));
	m_animals.emplace_back(Texture(animal2Path));
	m_animals.emplace_back(Texture(animal3Path));

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

	// 使う画像をロード
	m_puzzleImages = {
		U"Assets/Beethoven.jpg",
		U"Assets/TestPuzzle.jpeg",
		U"Assets/TestPuzzle1.jpg",
	};

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

			// PuzzleにSEインスタンスを渡す
			m_puzzle->SetSE(pickSE, snapSE);
		}
		catch (const Error& e)
		{
			// SEファイルのロードに失敗した場合の処理
			// このメッセージはデバッグ時に役立ちます。
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

	if (m_animals.size() >= 3)
	{
		// 1. 青い鳥 (右へ移動)
		m_animals[0].position = { -50, 100 }; // 画面左外から開始
		m_animals[0].velocity = { 80, 0 }; // 右に80px/秒
		m_animals[0].scale = 0.5;
		m_animals[0].isMoving = true;

		// 2. 馬 (左へ移動)
		m_animals[1].position = { Scene::Width() + 50, Scene::Height() - 100 }; // 画面右外から開始
		m_animals[1].velocity = { -50, 0 }; // 左に50px/秒
		m_animals[1].scale = 0.8;
		m_animals[1].isMoving = true;

		// 3. 牛 (斜め下へ移動)
		m_animals[2].position = { 50, Scene::Height() / 2 };
		m_animals[2].velocity = { 30, 30 }; // 右下方向に30px/秒
		m_animals[2].scale = 1.0;
		m_animals[2].isMoving = true;
	}
}

void MiniGameScene_2::update()
{
	const double deltaTime = Scene::DeltaTime();

	for (auto& animal : m_animals)
	{
		if (!animal.isMoving) continue;

		animal.position += animal.velocity * deltaTime;

		// サイズを計算 (テクスチャがロードされていることを前提)
		const double halfWidth = animal.texture.width() * animal.scale / 2.0;
		const double halfHeight = animal.texture.height() * animal.scale / 2.0;

		// 画面の左右端で反対側から出現させる (ワープ処理)
		if (animal.position.x < -halfWidth)
		{
			animal.position.x = Scene::Width() + halfWidth;
		}
		else if (animal.position.x > Scene::Width() + halfWidth)
		{
			animal.position.x = -halfWidth;
		}

		// 画面の上下端で反対側から出現させる (ワープ処理)
		if (animal.position.y < -halfHeight)
		{
			animal.position.y = Scene::Height() + halfHeight;
		}
		else if (animal.position.y > Scene::Height() + halfHeight)
		{
			animal.position.y = -halfHeight;
		}
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
	for (const auto& animal : m_animals)
	{
		// texture.scaled(scale).drawAt(position)で描画
		animal.texture.scaled(animal.scale).drawAt(animal.position, ColorF(1.0, 1.0, 1.0, 1.0));
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
