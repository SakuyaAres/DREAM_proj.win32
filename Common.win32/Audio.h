#pragma once
#include "fmod.hpp"
#include "fmod_errors.h"
#include "cocos2d.h"
#include <string>

#define SAFE_RELEASE_FMOD_COMPONENT(__COM__) { if((__COM__)) (__COM__)->release(); (__COM__) = nullptr; }
using namespace FMOD;
USING_NS_CC;

class AudioSystem
{
	public:
	AudioSystem();
	~AudioSystem();

	enum SOUNDTYPE
	{
		BGM,
		SFX,
		LOOP_BGM,
		LOAD_BGM
	};

	struct WaveData
	{
		float L = 0;
		float R = 0;
		float MIX = 0;
	};

	private:
	System* pSystem;
	Sound* pCurBGM;
	SoundGroup* pBGMGroup;
	SoundGroup* pSFXGroup;
	Channel* pBGMChannel;
	unsigned int bgmLength;

	public:
	void loadSoundFromFile(const std::string& pFilename, SOUNDTYPE sndtype);
	void playBGM();
	void pauseBGM();
	void resumeBGM();
	void stopBGM();
	void playSFX(int sfxIndex);
	unsigned int getBgmLength();
	unsigned int getBgmPosition();
	WaveData getWaveData();

};

