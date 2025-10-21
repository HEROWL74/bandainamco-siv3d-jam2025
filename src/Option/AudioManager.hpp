#pragma once
#include <Siv3D.hpp>
#include <unordered_map>
#include <memory>
#include "Settings.hpp"


enum class Bus
{
	BGM,
	SE,
	Count,
};


class AudioManager
{
public:
	AudioManager(std::shared_ptr<class Settings> settings);
	~AudioManager();

	bool SystemInit();
	void Update();			// フェード/ダッキング処理用

	// ロード系
	bool PreLoadBGM(const String& id, const FilePath& path);
	bool PreLoadSE(const String& id, const FilePath& path);

	// 再生（bus指定）
	void PlayBGM(const String& id, bool loop = true);
	void StopBGM(Seconds fade = 0s);
	void PlaySE(const String& id);
	void PlaySEPan(const String& id, const Vec2& sourcePos, const Vec2& listenerPos);		// 位置によってパンを変更する関数

	// マスター/バスの音量制御
	void SetMasterVolume(double v);
	double GetMasterVolume() const;
	void SetBusVolume(Bus bus, double v);
	double GetBusVolume(Bus bus) const;

	// BGMのピッチ（再生速度）制御
	void SetBGMPitch(const String& id, double pitch);
	double GetBGMPitch(const String& id) const;

	// 状態チェック
	bool IsLoaded(const String& id) const;

private:
	std::shared_ptr<class Settings> m_settings;

	// 音源の実体
	std::unordered_map<String, Audio> m_bgmMap;
	std::unordered_map<String, Audio> m_seMap;

	String m_currentBGMId;

	// 音量
	double m_masterVolume = 0.0;
	double m_busVolume[static_cast<int>(Bus::Count)] = {};

	double m_bgmPitchShift = 0.0;
};
