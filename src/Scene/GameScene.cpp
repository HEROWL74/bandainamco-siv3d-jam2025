#include "GameScene.hpp"
#include "../Core/Application.hpp"
#include "../Map/MapLoader.hpp"	

// コンストラクタ
GameScene::GameScene(const InitData& init)
	:IScene(init)
	, m_playerTexture(U"assets/player_spritesheet.png")
	, m_player(m_playerTexture)
	, m_gameOption(nullptr)
{
	SystemInit();
	GameInit();
}

// デストラクタ
GameScene::~GameScene()
{
	Release();
}

bool GameScene::SystemInit()
{
	auto& data = getData();							// 共有データの所得

	m_gameOption = std::make_unique<GameOption>(data.settings, data.audio);
	if (m_gameOption == nullptr) return false;

	// gameBGMのロード
	if (data.audio)
	{
		data.audio->PreLoadBGM(U"GameBGM", U"example/test.mp3");
	}

	m_optionIcon = Texture{ U"⚙️"_emoji };

	return true;
}

void GameScene::GameInit()
{
	// BGM再生
	auto& data = getData();
	if (data.audio)
	{
		data.audio->PlayBGM(U"GameBGM", true);
	}

	// プレイヤー初期化
	m_player.SetPosition(Vec2{0,0});
	m_player.Init();

	// カメラ初期化
	m_MainCamera.SetPosition(m_player.GetPosition());

	// マップ初期化
	m_mapCollisions = MapLoader::LoadCollisions(U"map_01");

	const Vec2 drawCenter = { 0, -150.0 };
	const Vec2 hitBoxCenter = { 0.0, 80.0 };
	const Vec2 hitBoxSize = { 150.0, 150.0 };
	const FilePath doorPath = U"assets/props/door_test.png";
	m_door = std::make_unique<Door>(drawCenter, hitBoxCenter, hitBoxSize, doorPath);

	// ゲームの状態
	m_gameState = GameState::Game;

	m_gameOption->GameInit();

	// オプションボタンの座標
	const double buttonX = 750.0;
	const double buttonY = 550.0;
	const double buttonW = 40;
	const double ronded = 6;

	m_optionButton = RectF{ Arg::center(buttonX, buttonY), buttonW, buttonW }.rounded(ronded);
}

void GameScene::update()
{
	switch (m_gameState)
	{
	case GameState::Game:	// ゲーム画面
		// エンターキーでシーンをresultへ
		if (KeyEnter.down())
		{
			changeScene(SceneState::RESULT);
			getData().audio->StopBGM(1s);
		}

		break;

	case GameState::Option:												// オプション画面
		m_gameOption->Update();
		if (m_gameOption->IsClosed())
		{
			m_gameState = GameState::Game;
			m_gameOption->GameInit();
		}
		break;
	}

	if (m_optionButton.leftClicked())
	{
		m_gameState = GameState::Option;
	}


	// プレイヤー更新
	m_player.Update();

	m_player.TryMove(m_mapCollisions);

	//カメラ更新
	m_MainCamera.SetTarget(m_player.GetPosition());
	m_MainCamera.Update();

	HandleDoorTransition();
}

void GameScene::draw() const
{
	const auto& data = getData();
	ColorF backgroundColor;

	// nextMiniGame の値に基づいて背景色を決定
	switch (data.nextMiniGame)
	{
	case 0: // 例: ミニゲーム0 の時
		backgroundColor = ColorF{ 1.0, 0.0, 0.0 }; // 赤
		break;
	case 1: // 例: ミニゲーム1 の時
		backgroundColor = ColorF{ 0.0, 0.0, 1.0 }; // 青
		break;
	case 2: // 例: ミニゲーム2 の時
		backgroundColor = ColorF{ 0.0, 1.0, 0.0 }; // 緑
		break;
	case 3: // 例: ミニゲーム3 の時
		backgroundColor = ColorF{ 1.0, 1.0, 0.6 }; // 薄いマゼンタ
		break;
	default:
		backgroundColor = ColorF{ 1.0, 1.0, 1.0 }; // デフォルト（白）
		break;
	}

	Scene::SetBackground(backgroundColor); // 決定した背景色を設定

	const ScopedRenderStates2D blend{ SamplerState::ClampNearest };

	{
		const auto t = m_MainCamera.GetTransformer();
		m_door->draw();
		const Circle playerCol = m_player.GetCollision().getWorldShape(m_player.GetPosition());
#ifdef _DEBUG

		// グリッド線で地面を可視化
		//for (int y = -5; y <= 5; ++y)
		//{
		//	for (int x = -5; x <= 5; ++x)
		//	{
		//		RectF{ x * 64.0 - 32, y * 64.0 - 32, 64, 64 }
		//		.draw(ColorF{ (x + y) % 2 == 0 ? 1.0 : 0.0 });
		//	}
		//}
		// コリジョンを赤枠で描画
		for (const auto& mapCol : m_mapCollisions)
		{
			mapCol.drawFrame((3,3),ColorF{ 1.0, 0.0, 0.0 });
		}
		// 緑枠でプレイヤー用の円形コリジョンを描画
		m_player.GetCollision().getWorldShape(m_player.GetPosition())
			.drawFrame(2, 0, ColorF{ 0.0, 1.0, 0.0 });
#endif
	}

	{
		ScopedSpotlight target{ m_spotlight, ColorF{ 0.05, 0.05, 0.1 } };
		m_player.DrawLight(m_MainCamera);
	}

	m_spotlight.draw();

	{
		const auto t = m_MainCamera.GetTransformer();
		m_player.DrawCharacter(m_MainCamera);
	}

	//描画処理
	Circle{ mCirclePos.x, mCirclePos.y, 50 }.draw(Palette::Orange);

	// 設定ボタンの描画	
	m_optionButton.draw(Palette::Silver);

	// マウスが図形の上に来たら影ができる
	if (m_optionButton.mouseOver())
	{
		m_optionButton.drawShadow(Vec2{ 2, 2 }, 12, 1).draw(ColorF{ 0.9, 0.8, 0.6 });
	}
	// 歯車マーク
	double iconX = 750.0;
	double iconY = 550.0;
	m_optionIcon.scaled(0.3).drawAt(iconX, iconY);

	// オプション画面の描画
	if (m_gameState == GameState::Option)
	{
		m_gameOption->Draw();
	}
}



bool GameScene::Release()
{


	return true;
}

void GameScene::HandleDoorTransition()
{
	const Circle playerCol = m_player.GetCollision().getWorldShape(m_player.GetPosition());

	if (m_door->isNear(playerCol))
	{
		if (KeySpace.down())
		{
			// 共有データから次に遷移するミニゲーム番号を取得して遷移
			auto& data = getData();

			switch (data.nextMiniGame)
			{
			case 0:
				changeScene(SceneState::MINIGAME_0);
				break;
			case 1:
				changeScene(SceneState::MINIGAME_1);
				break;
			case 2:
				changeScene(SceneState::MINIGAME_2);
				break;
			case 3:
				changeScene(SceneState::MINIGAME_3);
				break;
			default:
				changeScene(SceneState::MINIGAME_0);
				break;
			}

			if (data.audio) data.audio->StopBGM(1s);
		}
	}
}
