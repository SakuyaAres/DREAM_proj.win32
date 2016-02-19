#include "Audio.h"

using namespace FMOD;

AudioSystem::AudioSystem() :
	pBGMChannel(nullptr)
{
	auto result = System_Create(&pSystem);
	pSystem->init(32, FMOD_INIT_NORMAL, 0);
	pSystem->createSoundGroup("bgm", &pBGMGroup);
	pSystem->createSoundGroup("sfx", &pSFXGroup);
}

AudioSystem::~AudioSystem()
{
	SAFE_RELEASE_FMOD_COMPONENT(pBGMGroup);
	SAFE_RELEASE_FMOD_COMPONENT(pSFXGroup);
	SAFE_RELEASE_FMOD_COMPONENT(pSystem);
}

void AudioSystem::loadSoundFromFile(const std::string & pFilename, SOUNDTYPE sndtype)
{
	const char* filepath = pFilename.c_str();
	Sound* pSound = nullptr;
	FMOD_MODE mode = FMOD_HARDWARE | FMOD_2D;
	FMOD_RESULT result;
	switch (sndtype)
	{
		case BGM:
			result = pSystem->createStream(filepath, FMOD_HARDWARE | FMOD_2D | FMOD_LOOP_OFF, 0, &pCurBGM);
			break;
		case SFX:
			result = pSystem->createSound(filepath, FMOD_HARDWARE | FMOD_2D | FMOD_LOOP_OFF, 0, &pCurBGM);
			break;
		case LOAD_BGM:
			result = pSystem->createSound(filepath, FMOD_HARDWARE | FMOD_2D | FMOD_LOOP_OFF, 0, &pCurBGM);
			break;
		default:
			break;
	}
	pCurBGM->getLength(&bgmLength, FMOD_TIMEUNIT_MS);
}

void AudioSystem::playBGM()
{
	FMOD_RESULT result = pSystem->playSound(
		FMOD_CHANNEL_REUSE,
		pCurBGM,
		true,
		&pBGMChannel
		);
	pBGMChannel->setPaused(false);
}

void AudioSystem::pauseBGM()
{
	pBGMChannel->setPaused(true);
}

void AudioSystem::resumeBGM()
{
	pBGMChannel->setPaused(false);
}

void AudioSystem::stopBGM()
{
	FMOD_RESULT result = pBGMChannel->stop();
}

void AudioSystem::playSFX(int sfxIndex)
{
	Sound* pSFX;
	pSFXGroup->getSound(sfxIndex, &pSFX);
	pSystem->playSound(
		FMOD_CHANNEL_FREE,
		pSFX,
		false,
		nullptr
		);
}

unsigned int AudioSystem::getBgmLength()
{
	return bgmLength;
}

unsigned int AudioSystem::getBgmPosition()
{
	unsigned int posi = 0;
	pBGMChannel->getPosition(&posi, FMOD_TIMEUNIT_MS);
	return posi;
}

AudioSystem::WaveData AudioSystem::getWaveData()
{
	float tempData[1];
	WaveData data;
	pSystem->getWaveData(tempData, 1, 0);
	data.L = tempData[0];
	data.MIX = abs(tempData[0]);
	pSystem->getWaveData(tempData, 1, 1);
	data.R = tempData[0];
	data.MIX += abs(tempData[0]);
	data.MIX /= 2;
	return data;
}
