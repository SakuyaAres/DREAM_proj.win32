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
	//const char* testpath = FileUtils::getInstance()->getWritablePath().c_str();
	//const char* fullPath = FileUtils::getInstance()->fullPathForFilename(pFilename).c_str();
	const char* filepath = pFilename.c_str();
	Sound* pSound = nullptr;
	FMOD_MODE mode = FMOD_HARDWARE | FMOD_2D;
	//FMOD_RESULT result = pSystem->createSound(
	//	filepath,
	//	sndtype == LOOP_BGM ? mode | FMOD_LOOP_NORMAL : mode | FMOD_LOOP_OFF,
	//	0,
	//	sndtype == LOOP_BGM ? &pCurBGM : nullptr
	//	);
	//sndtype == LOOP_BGM ?
	//	pCurBGM->setSoundGroup(sndtype == SFX ? pSFXGroup : pBGMGroup) :
	//	pSound->setSoundGroup(pSFXGroup);
	FMOD_RESULT result = pSystem->createSound(filepath, FMOD_HARDWARE | FMOD_2D | FMOD_LOOP_OFF, 0, &pCurBGM);
}

void AudioSystem::playBGM(int bgmIndex)
{
	//Sound* pBGM;
	//pBGMGroup->getSound(bgmIndex, &pBGM);
	FMOD_RESULT result = pSystem->playSound(
		FMOD_CHANNEL_REUSE,
		pCurBGM,
		true,
		&pBGMChannel
		);
	pBGMChannel->setPaused(false);
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

long AudioSystem::getBgmPosition()
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
