#include "AudioManager.hpp"

AudioManager::AudioManager(std::shared_ptr<Settings> settings)
	:m_settings(settings)
{
	if (m_settings)
	{
		m_masterVolume = m_settings->masterVolume;
		m_busVolume[static_cast<int>(Bus::BGM)] = m_settings->bgmVolume;
		m_busVolume[static_cast<int>(Bus::SE)] = m_settings->seVolume;
	}
}

AudioManager::~AudioManager()
{
	StopBGM();
	m_bgmMap.clear();
	m_seMap.clear();
}

bool AudioManager::SystemInit()
{


	return true;
}

void AudioManager::Update()			// フェード/ダッキング処理用
{

}

// ロード系
bool AudioManager::PreLoadBGM(const String& id, const FilePath& path)
{
	auto [it, inserted] = m_bgmMap.try_emplace(id, Audio::Stream, path);	// it: 挿入先のイテレータ、inserted: 新規挿入が行われたかを表すフラグ

	// 既に存在していた場合
	if (!inserted) return true;

	// 挿入に成功した要素に追加初期化を行う
	it->second.setVolume(m_busVolume[static_cast<int>(Bus::BGM)]);			// itのfirst: String, second: Audio

	return true;
}

bool AudioManager::PreLoadSE(const String& id, const FilePath& path)
{
	auto [it, inserted] = m_seMap.try_emplace(id, path);

	if (!inserted) return true;

	it->second.setVolume(m_busVolume[static_cast<int>(Bus::SE)]);
}


// 再生（bus指定）
void AudioManager::PlayBGM(const String& id, bool loop)
{
	// 登録されていない/再生中の場合は何にもしない
	if (!m_bgmMap.contains(id)) return;
	if (m_currentBGMId == id && m_bgmMap[id].isPlaying()) return;

	// 再生処理
	m_currentBGMId = id;
	m_bgmMap[id].setLoop(loop);
	m_bgmMap[id].play();
}

void AudioManager::StopBGM(Seconds fade)
{
	if (m_currentBGMId.isEmpty()) return;

	m_bgmMap[m_currentBGMId].stop(fade);
	m_currentBGMId.clear();
}

void AudioManager::PlaySE(const String& id)
{
	if (!m_seMap.contains(id)) return;

	// 再生処理
	m_seMap[id].playOneShot();
}

// マスター/バスの音量制御
void AudioManager::SetMasterVolume(double v)
{
	m_masterVolume = v;
	GlobalAudio::SetVolume(m_masterVolume);

	// 変更された音量をsettingsに反映させる
	if (m_settings) m_settings->masterVolume = m_masterVolume;
}

double AudioManager::GetMasterVolume() const
{
	return m_masterVolume;
}

void AudioManager::SetBusVolume(Bus bus, double v)
{
	const int index = static_cast<int>(bus);
	m_busVolume[index] = v;

	// 変更された音量をsettingsに反映させる
	if (m_settings)
	{
		if (bus == Bus::BGM)
		{
			GlobalAudio::BusSetVolume(MixBus0, m_busVolume[index]);
			m_settings->bgmVolume = m_busVolume[index];
		}
		if (bus == Bus::SE)
		{
			GlobalAudio::BusSetVolume(MixBus1, m_busVolume[index]);
			m_settings->seVolume = m_busVolume[index];
		}
	}
}

double AudioManager::GetBusVolume(Bus bus) const
{
	return m_busVolume[static_cast<int>(bus)];
}

// 補助
bool AudioManager::IsLoaded(const String& id) const
{
	return !m_currentBGMId.isEmpty() && m_bgmMap.contains(m_currentBGMId) && m_bgmMap.at(m_currentBGMId).isPlaying();
}
