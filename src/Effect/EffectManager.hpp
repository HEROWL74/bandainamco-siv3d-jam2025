#pragma once

#include <Siv3D.hpp>
#include "EffectBase.hpp"

class MainCamera;

/// @brief エフェクト管理クラス
class EffectManager
{
public:
	EffectManager() = default;
	~EffectManager() = default;

	// 修正: lifeSpanSec を引数に追加し、Type のコンストラクタに渡す
	template <class Type, class... Args>
	void Add(const Vec2& position, double lifeSpanSec, Args&&... args)
	{
		static_assert(std::is_base_of_v<EffectBase, Type>, "Type must derive from EffectBase");

		// 派生クラスのコンストラクタ (lifeSpanSec, position, Args...) を呼び出す
		m_effects.push_back(
			std::make_unique<Type>(position, lifeSpanSec, std::forward<Args>(args)...)
		);
	}

	// 状態更新（update 内でエフェクトを進め、寿命の切れたものを削除）
	void Update();

	// 描画（メインゲーム: カメラあり）
	void Draw(const MainCamera& camera) const;

	// 描画（ミニゲーム: カメラなし）
	void Draw() const;

private:
	Array<std::unique_ptr<EffectBase>> m_effects;
};
