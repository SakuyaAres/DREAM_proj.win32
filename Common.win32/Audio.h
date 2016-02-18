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
		BGM = 0,
		SFX = 1,
		LOOP_BGM = 2
	};

	struct WaveData
	{
		float L = 0;
		float R = 0;
		float MIX = 0;
	};

	public:
	void loadSoundFromFile(const std::string& pFilename, SOUNDTYPE sndtype);
	void playBGM(int bgmIndex);
	void playSFX(int sfxIndex);
	long getBgmPosition();
	WaveData getWaveData();

	private:
	System* pSystem;
	Sound* pCurBGM;
	SoundGroup* pBGMGroup;
	SoundGroup* pSFXGroup;
	Channel* pBGMChannel;

	private:

};

