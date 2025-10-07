#pragma once
# include <Siv3D.hpp>

struct Animation
{
	int startFrame = 0; // アニメーションの開始フレーム
	int endFrame = 0;   // アニメーションの終了フレーム
	int row = 0;        // スプライトシートの行
	Array<double> durations;
};

namespace AnimationKeys
{
	// Idle
	const String Idle_D = U"idle_D";
	const String Idle_U = U"idle_U";
	const String Idle_L = U"idle_L";
	const String Idle_R = U"idle_R";

	// Walk
	const String Walk_D = U"walk_D";
	const String Walk_U = U"walk_U";
	const String Walk_L = U"walk_L";
	const String Walk_R = U"walk_R";
}
