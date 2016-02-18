#ifndef __FALLINGSOLO_SCENE_H__
#define __FALLINGSOLO_SCENE_H__

#include "cocos2d.h"
#include "WStringSupport.h"
#include "Audio.h"
#include "debug.h"
#include "UI.h"

#define WAVE_EFFECT_TAG 100
USING_NS_CC;

class SceneFallingSolo : public cocos2d::Layer
{
	public:

	private:
	AudioSystem::WaveData waveDataPrev;
	Debug* debug;

	public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	//Scene Part
	void createBG();
	void createHud();
	void playBGM();

	//implement the "static create()" method manually
	CREATE_FUNC(SceneFallingSolo);

	void update(float dt) override;

	void debugMsg();
};

#endif
