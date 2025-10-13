#include "MapLoader.hpp"

// JSONファイルから RectF を読み込む処理の実装
Array<RectF> MapLoader::LoadCollisions(const String& mapName)
{
	Array<RectF> collisions;

	// JSON ファイルのパスを構築
	const FilePath mapPath = U"Map/" + mapName + U".json";

	// ファイルを読み込む
	const JSON json = JSON::Load(mapPath);

	if (json.isNull())
	{
		// 読み込みに失敗した場合
		Logger<<(U"マップファイル {} の読み込みに失敗しました。パスを確認してください: {}"_fmt(mapName, mapPath));
		return collisions;
	}

	// ルート要素から "collisions" という名前の配列要素を取得
	const auto& jsonCollisions = json[U"collisions"];

	// JSONの "collisions" 要素が配列ではない、または存在しない場合のエラーチェック
	if (!jsonCollisions.isArray())
	{
		Logger << (U"マップファイル {} に 'collisions' 配列が見つかりません。"_fmt(mapName));
		return collisions;
	}

	// 配列をループし、各コリジョンデータを読み取る
	for (const auto& rectData : jsonCollisions.arrayView())
	{
		// "type" が "rect" であることを確認
		if (rectData[U"type"].getString() == U"rect")
		{
			try
			{
				// 各 RectF の要素を double 型として取得
				double x = rectData[U"x"].get<double>();
				double y = rectData[U"y"].get<double>();
				double w = rectData[U"w"].get<double>();
				double h = rectData[U"h"].get<double>();

				// RectF を構築して配列に追加
				collisions << RectF{ x, y, w, h };
			}
			catch (const Error& e)
			{
				// データ型変換やキーが見つからない場合のエラー処理
				Logger << (U"マップ {} のコリジョンデータ解析中にエラーが発生しました: {}"_fmt(mapName, e.what()));
			}
		}
		else
		{
			Logger << (U"マップ {} の未知のコリジョンタイプをスキップしました: {}"_fmt(mapName, rectData[U"type"].getString()));
		}
	}

	Logger << (U"マップ {} のコリジョン {} 個を読み込みました。"_fmt(mapName, collisions.size()));

	return collisions;
}
