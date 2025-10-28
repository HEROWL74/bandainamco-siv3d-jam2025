#include "CharacterBase.hpp"

CharacterBase::CharacterBase(const Texture& texture)
    : m_texture(texture)
{
}

void CharacterBase::AddAnimation(const String& name, const Animation& anim)
{
    m_animations[name] = anim;
}

void CharacterBase::PlayAnimation(const String& name)
{
    if (m_currentAnimationName != name && m_animations.contains(name))
    {
        m_currentAnimationName = name;
        m_animationTimer = 0.0;
        m_currentFrame = 0;

        // 排他アニメーションならイベントフラグON
		if (m_animations.at(name).isExclusive) {
			m_isEvent = true;
		}
    }
}

bool CharacterBase::IsExclusiveAnimation(const String& name) const
{
	return m_animations.contains(name) && m_animations.at(name).isExclusive;
}

void CharacterBase::Update()
{
    //m_position += m_velocity;
    // アニメーション更新
    if (m_animations.contains(m_currentAnimationName)) // アニメーションが登録されているか
    {
        const Animation& anim = m_animations.at(m_currentAnimationName); // 現在のアニメーション情報取得
        m_animationTimer += Scene::DeltaTime();

        while (m_animationTimer > anim.durations[m_currentFrame]) // フレーム切り替え
        {
            m_animationTimer -= anim.durations[m_currentFrame]; // フレーム時間を引く
            m_currentFrame++;
            if (m_currentFrame > (anim.endFrame - anim.startFrame)) // 最終フレームを超えたら
            {
                OnAnimationEnd(m_currentAnimationName); // アニメーション終了コールバック
                m_currentFrame = 0; //最初のフレームに戻る
            }
        }
    }
}

void CharacterBase::Draw(const MainCamera& camera) const
{
	if (m_animations.contains(m_currentAnimationName))
	{
		const Animation& anim = m_animations.at(m_currentAnimationName);
		int frame = anim.startFrame + m_currentFrame;
		int frameX = frame * m_framewidth;
		int frameY = anim.row * m_frameheight;

		m_texture(frameX, frameY, m_framewidth, m_frameheight)
			.scaled(12)
			.drawAt(m_position);
	}
}
