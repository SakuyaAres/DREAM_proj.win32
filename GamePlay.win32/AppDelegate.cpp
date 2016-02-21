#include "AppDelegate.h"
#include "SceneMainMenu.h"

#define _GLFW_USE_DWM_SWAP_INTERVAL 0
#define _GLEW_USE_DWM_SWAP_INTERVAL 0

USING_NS_CC;
using namespace std;

static cocos2d::Size designResolutionSize = cocos2d::Size(1280, 720);
static cocos2d::Size smallResolutionSize = cocos2d::Size(640, 360);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(1280, 720);
static cocos2d::Size largeResolutionSize = cocos2d::Size(1920, 1080);

typedef void (APIENTRY *PFNWGLEXTSWAPCONTROLPROC) (int);
typedef int(*PFNWGLEXTGETSWAPINTERVALPROC) (void);
PFNWGLEXTSWAPCONTROLPROC wglSwapIntervalEXTb = NULL;
PFNWGLEXTGETSWAPINTERVALPROC wglGetSwapIntervalEXTb = NULL;

Vec2 originPoint;
Size visibleSize;
AudioSystem* pAudioSystem;
UI* pUI;
Scene* pSceneMainMenu;
Scene* pSceneFallingSolo;
ofstream plogfile;

AppDelegate::AppDelegate() :
	maxFps(1200),
	swapInterval(0)
{
	plogfile.open("dream.log", ios::out);
	plogfile << "logtest\n";
}

AppDelegate::~AppDelegate()
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
	//set OpenGL context attributions,now can only set six attributions:
	//red,green,blue,alpha,depth,stencil
	GLContextAttrs glContextAttrs = { 8, 8, 8, 8, 24, 8 };

	GLView::setGLContextAttrs(glContextAttrs);
}

void AppDelegate::setGLVsync(bool enabled)
{
	// Detect vSync
	char* extensions = (char*)glGetString(GL_EXTENSIONS);
	bool isVsyncInit = false;
	if (strstr(extensions, "WGL_EXT_swap_control")!=NULL)
	{
		wglSwapIntervalEXTb = (PFNWGLEXTSWAPCONTROLPROC)wglGetProcAddress("wglSwapIntervalEXT");
		wglGetSwapIntervalEXTb = (PFNWGLEXTGETSWAPINTERVALPROC)wglGetProcAddress("wglGetSwapIntervalEXT");
		isVsyncInit = true;
	}
	//if (enabled&&isVsyncInit) wglSwapIntervalEXT(0);
	//else wglSwapIntervalEXT(1);
	if (wglSwapIntervalEXT == NULL) return;
	if (enabled) 
	{
		if(isVsyncInit) wglSwapIntervalEXTb(1);
		else wglSwapIntervalEXT(1);
	}
	else
	{
		if (isVsyncInit) wglSwapIntervalEXTb(0);
		else wglSwapIntervalEXT(0);
	}
}

// If you want to use packages manager to install more packages, 
// don't modify or remove this function
static int register_all_packages()
{
	return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
	// initialize director
	plogfile << "logtest1\n";


	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	if (!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
		glview = GLViewImpl::createWithRect("DreaMus", Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
#else
		glview = GLViewImpl::create("DreaMus");
#endif
		director->setOpenGLView(glview);
	}
	// turn on display FPS
	director->setDisplayStats(true);
	setGLVsync(false);
	plogfile << "logtest2\n";
	plogfile.close();

	// set FPS. the default value is 1.0/60 if you don't call this
	director->setAnimationInterval(1.0 / maxFps);


	// Set Clear Color
	glClearColor(255.0, 255.0, 255.0, 1.0);

	// Set the design resolution
	glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::NO_BORDER);
	Size frameSize = glview->getFrameSize();
	// if the frame's height is larger than the height of medium size.
	if (frameSize.height > mediumResolutionSize.height)
	{
		director->setContentScaleFactor(MIN(largeResolutionSize.height / designResolutionSize.height, largeResolutionSize.width / designResolutionSize.width));
	}
	// if the frame's height is larger than the height of small size.
	else if (frameSize.height > smallResolutionSize.height)
	{
		director->setContentScaleFactor(MIN(mediumResolutionSize.height / designResolutionSize.height, mediumResolutionSize.width / designResolutionSize.width));
	}
	// if the frame's height is smaller than the height of medium size.
	else
	{
		director->setContentScaleFactor(MIN(smallResolutionSize.height / designResolutionSize.height, smallResolutionSize.width / designResolutionSize.width));
	}
	plogfile << "logtest3\n";

	register_all_packages();

	// create fmodex audio system
	pAudioSystem = new AudioSystem();

	// save visible area info
	originPoint = director->getVisibleOrigin();
	visibleSize = director->getVisibleSize();

	// create UI info
	pUI = new UI();

	// create a scene. it's an autorelease object
	pSceneMainMenu = SceneMainMenu::createScene();

	// run
	director->runWithScene(pSceneMainMenu);

	return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
	Director::getInstance()->stopAnimation();

	// if you use SimpleAudioEngine, it must be pause
	// SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
	Director::getInstance()->startAnimation();

	// if you use SimpleAudioEngine, it must resume here
	// SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
