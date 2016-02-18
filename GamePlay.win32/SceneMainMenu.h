#ifndef __MAINMENU_SCENE_H__
#define __MAINMENU_SCENE_H__

#include "cocos2d.h"
#include "WStringSupport.h"
#include "Audio.h"
#include "debug.h"
#include "UI.h"

#define WAVE_EFFECT_TAG 100
USING_NS_CC;

class SceneMainMenu : public cocos2d::Layer
{
	public:

	private:
	AudioSystem::WaveData waveDataPrev;
	Debug* debug;

	public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	//Scene scheme
	void createBG();
	void createMenu();
	void playBGM();

	//Effect
	void waveEffect();

	//Menu item callback
	void menuCloseCallback(Event* event);
	void menuMouseMoveCallback(Event* event);

	//implement the "static create()" method manually
	CREATE_FUNC(SceneMainMenu);

	void update(float dt) override;
	
	//Scene Shift Action
	void gotoSoloScene();

	//debug
	void debugMsg();
};

#endif