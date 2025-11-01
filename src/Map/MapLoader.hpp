#pragma once
#include <Siv3D.hpp>

class MapLoader
{
public:
	// JSONファイルはApp/Map/ フォルダに配置し、map_00.json のように命名することを想定
	static Array<RectF> LoadCollisions(const String& mapName);

private:
	// インスタンス化を防止
	MapLoader() = delete;
};
