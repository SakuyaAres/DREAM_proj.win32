#ifndef  _APP_DELEGATE_H_
#define  _APP_DELEGATE_H_

#include "cocos2d.h"
#include "Audio.h"
#include "UI.h"
#include <fstream>
#include "GL\wglew.h"

/**
@brief    The cocos2d Application.

The reason for implement as private inheritance is to hide some interface call by Director.
*/
USING_NS_CC;

class  AppDelegate : private cocos2d::Application
{
	public:
	AppDelegate();
	virtual ~AppDelegate();

	int maxFps;
	int swapInterval;

	virtual void initGLContextAttrs();

	virtual void setGLVsync(bool enabled);

	/**
	@brief    Implement Director and Scene init code here.
	@return true    Initialize success, app continue.
	@return false   Initialize failed, app terminate.
	*/
	virtual bool applicationDidFinishLaunching();

	/**
	@brief  The function be called when the application enter background
	@param  the pointer of the application
	*/
	virtual void applicationDidEnterBackground();

	/**
	@brief  The function be called when the application enter foreground
	@param  the pointer of the application
	*/
	virtual void applicationWillEnterForeground();
};

extern Vec2 originPoint;
extern Size visibleSize;
extern AudioSystem* pAudioSystem;
extern UI* pUI;
extern Scene* pSceneMainMenu;
extern Scene* pSceneFallingSolo;

#endif // _APP_DELEGATE_H_

