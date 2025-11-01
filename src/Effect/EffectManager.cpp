#include "EffectManager.hpp"
#include <Siv3D.hpp>

// 更新処理: 各エフェクトに経過時間を渡して update() を呼ぶ
void EffectManager::Update()
{
	for (auto& e : m_effects)
	{
		const double elapsed = Scene::Time() - e->GetStartTime();
		e->update(elapsed); // update は状態更新のみ（描画しない）
	}

	// 寿命が尽きたエフェクトを除去
	m_effects.remove_if([](const std::unique_ptr<EffectBase>& ef) {
		return !ef->IsAlive();
	});
}

// 描画 (カメラあり)
void EffectManager::Draw(const MainCamera& camera) const
{
	for (const auto& e : m_effects)
	{
		e->Draw(camera);
	}
}

// 描画 (カメラなし)
void EffectManager::Draw() const
{
	for (const auto& e : m_effects)
	{
		e->Draw();
	}
}
