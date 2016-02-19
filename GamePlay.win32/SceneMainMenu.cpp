#pragma once
#include "AppDelegate.h"
#include "WStringSupport.h"
#include "SceneMainMenu.h"
#include "SceneFallingSolo.h"

#define random_r RandomHelper::random_real

USING_NS_CC;

Scene* SceneMainMenu::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = SceneMainMenu::create();
	// add layer as a child to scene
	layer->setAnchorPoint(Point(0.5, 0.5));
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool SceneMainMenu::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	//pUI->themeColor = Color4B(119, 136, 194, 255);


	/*
	auto closeItem = MenuItemImage::create(
		"./image/CloseNormal.png",
		"./image/CloseSelected.png",
		CC_CALLBACK_1(SceneMainMenu::menuCloseCallback, this));

	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width / 2,
								origin.y + closeItem->getContentSize().height / 2));

	// create menu, it's an autorelease object
	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	/////////////////////////////
	// 3. add your codes below...

	// add a label shows "Hello World"
	// create and initialize a label

	TTFConfig labelStyle;
	labelStyle.fontFilePath = "fonts/FZHTJW.TTF";
	labelStyle.fontSize = 18;
	labelStyle.distanceFieldEnabled = false;
	labelStyle.glyphs = GlyphCollection::DYNAMIC;
	labelStyle.customGlyphs = nullptr;
	char str[512] = "";
	char labelFormat[512] = "";
	strcat(labelFormat, "OpenGL Render Test（vSync Force OFF）\r\n");
	strcat(labelFormat, "Render Blocks = %d\r\n\r\n");
	strcat(labelFormat, "Audio System Test (BGM Playback)\r\n\r\n");
	strcat(labelFormat, "Sence Schedule Update Test\r\n(Dynamic Bezier Line Scale by BGM Wave Raw Data)\r\n\r\n");
	strcat(labelFormat, "UTF-8 TTF to Texture Render Test\r\n(UTF-8 TTF文本纹理生成渲染测试)\0");
	sprintf_s(str, 512, labelFormat, i_max);
	auto label = Label::createWithTTF(labelStyle, A2U(str), TextHAlignment::LEFT);
	label->enableOutline(Color4B(0, 0, 0, 192), 2);


	// position the label on the center of the screen
	label->setAnchorPoint(Point(0, 0));
	label->setPosition(Vec2(0, origin.y + visibleSize.height - label->getContentSize().height));

	// add the label as a child to this layer
	this->addChild(label, 1);

	// add "HelloWorld" splash screen"
	auto sprite = Sprite::create("./image/fgtv1.png");

	// position the sprite on the center of the screen
	sprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

	// add the sprite as a child to this layer
	this->addChild(sprite, 0);
	FiniteTimeAction* titleBlink =
		RepeatForever::create(
			Sequence::createWithTwoActions(
				FadeTo::create(0.25, 0),
				FadeTo::create(0.25, 255)
				)
			);
	sprite->runAction(titleBlink);
	*/

	createBG();
	playBGM();
	createMenu();


	//Create debug class
	debug = new Debug();

	scheduleUpdate();

	return true;
}

void SceneMainMenu::createBG()
{
	auto layerBG = LayerColor::create(Color4B::WHITE);
	layerBG->setName("bgLayer");
	this->addChild(layerBG);
	int i_max = 15;
	for (int i = 1; i <= i_max; i++)
	{
		FLOAT sizex = random(400, 800);
		FLOAT sizey = 10.f;
		FLOAT posy = static_cast<FLOAT>(i)*(visibleSize.height - 200) / i_max - sizey / 2 + 100;

		auto bgRect = DrawNode::create();
		layerBG->addChild(bgRect);
		bgRect->drawSolidRect(
			Point(0, 0),
			Point(sizex, sizey),
			pUI->themeColorF(random(20, 80))
			);
		bgRect->setPosition(Point(visibleSize.width, posy));
		bgRect->runAction(
			RepeatForever::create(
				Sequence::createWithTwoActions(
					EaseQuadraticActionIn::create(
						MoveTo::create(random_r(4.f, 12.f), Point(-sizex, posy))
						),
					Place::create(Point(visibleSize.width, posy))
					)
				)
			);
	}

	int bznum = 3;
	for (int i = 0; i < bznum; i++)
	{
		auto layerBezier = LayerColor::create(Color4B(255, 255, 255, 0));
		char bezierLayerName[32];
		sprintf_s(bezierLayerName, "bezierLayer%d", i);
		layerBezier->setName(bezierLayerName);
		layerBezier->setAnchorPoint(Point(0, 0.5));
		layerBezier->setScaleY(0.5);
		layerBG->addChild(layerBezier);
		float posOffset = (i + 1 - bznum*0.5) * 100;
		for (int j = 0; j < 2; j++)
		{
			auto dBezier = DrawNode::create(5);
			dBezier->create();
			dBezier->drawCubicBezier(
				Point(0, visibleSize.height / 2),
				Point(visibleSize.width*0.5, visibleSize.height*0.77),
				Point(visibleSize.width*0.5, visibleSize.height*0.17),
				Point(visibleSize.width, visibleSize.height / 2),
				static_cast<INT>(visibleSize.width),
				pUI->themeColorF(128)
				);
			char bezierNodeName[32];
			sprintf_s(bezierNodeName, "bezierNode%d", (i * 2 + j));
			dBezier->setName(bezierNodeName);
			layerBezier->addChild(dBezier);
			dBezier->setPosition(Point(j*visibleSize.width, 0));
			dBezier->runAction(
				RepeatForever::create(
					Sequence::createWithTwoActions(
						MoveTo::create(0.85 + (0.17 + 0.17*i)*i*0.5, Point((j - 1)*visibleSize.width, 0)),
						Place::create(Point(j*visibleSize.width, 0))
						)
					)
				);
		}
	}
}

void SceneMainMenu::createMenu()
{
	auto layerMenu = LayerColor::create(pUI->themeColorB(255), visibleSize.width *0.25, visibleSize.height);
	layerMenu->setPositionX(100.f);
	this->addChild(layerMenu);

	auto menuWidth = layerMenu->getContentSize().width;
	auto menuHeight = layerMenu->getContentSize().height;
	float menuItemHeight = 100.f;
	int menuItemCount = 3;
	auto posy = (menuHeight - 200) / menuItemCount;
	TTFConfig labelStyleCN;
	labelStyleCN.fontFilePath = "fonts/FZHTJW.TTF";
	labelStyleCN.fontSize = 32;
	labelStyleCN.distanceFieldEnabled = false;
	labelStyleCN.glyphs = GlyphCollection::DYNAMIC;
	labelStyleCN.customGlyphs = nullptr;
	labelStyleCN.outlineSize = 0;
	TTFConfig labelStyleEN = labelStyleCN;
	labelStyleEN.fontSize = 18;
	for (int i = 1; i <= menuItemCount; i++)
	{
		auto layerMenuItemClip = ClippingRectangleNode::create(Rect(0, 0, menuWidth, menuItemHeight));
		layerMenu->addChild(layerMenuItemClip);
		layerMenuItemClip->setPosition(Point(0, 0));
		layerMenuItemClip->ignoreAnchorPointForPosition(false);
		layerMenuItemClip->setAnchorPoint(Point(0, 0.5));
		layerMenuItemClip->setPositionY(50 + posy*(menuItemCount - i + 1));

		auto layerMenuItem = LayerColor::create(Color4B::WHITE, menuWidth, menuItemHeight);
		layerMenuItem->setName("item");
		layerMenuItemClip->addChild(layerMenuItem);

		auto layerMenuAnime = LayerColor::create(Color4B::WHITE, menuWidth, menuItemHeight);
		int animeArrowCount = 20;
		for (int j = 0; j < animeArrowCount; j++)
		{
			auto animeArrow = DrawNode::create();
			layerMenuAnime->addChild(animeArrow);
			Point arrowVec[6];
			arrowVec[0] = Point(20, 0);
			arrowVec[1] = Point(40, 0);
			arrowVec[2] = Point(20, menuItemHeight);
			arrowVec[3] = Point(0, menuItemHeight);
			animeArrow->drawPolygon(arrowVec,
									4,
									i == menuItemCount ?
									Color4F(Color4B(255, 133, 107, 255)) :
									pUI->themeColorF(255),
									0.f,
									pUI->themeColorF(0)
									);
			animeArrow->setPosition(Point(j * menuWidth * 2 / animeArrowCount, 0));
		}
		layerMenuItem->addChild(layerMenuAnime);
		layerMenuAnime->runAction(
			RepeatForever::create(
				Sequence::createWithTwoActions(
					Place::create(Point(0, 0)),
					MoveTo::create(3.5, Point(-menuWidth, 0))
					)
				)
			);

		auto maskRect = DrawNode::create(3);
		//maskRect->drawSolidRect(Point(0, 0), Point(menuWidth, menuItemHeight), pUI->themeColorF(255));
		maskRect->drawLine(Point(30, 0), Point(menuWidth, 0), Color4F::WHITE);
		auto layerMask = LayerColor::create(pUI->themeColorB(235), menuWidth, menuItemHeight);
		layerMask->setName("mask");
		layerMask->setTag(0);
		layerMask->addChild(maskRect);
		layerMenuItem->addChild(layerMask);

		char* strCN = new char[16];
		char* strEN = new char[64];
		layerMenuItem->setTag(i);
		switch (i)
		{
			case 1:
				strCN = "单人游戏";
				strEN = "Solo game";
				break;
			case 2:
				strCN = "设置";
				strEN = "Open config panel";
				break;
			case 3:
				strCN = "退出";
				strEN = "Quit and do something else";
				break;
			default:
				strCN = "";
				strEN = "";
		}
		auto labelCN = Label::createWithTTF(labelStyleCN, A2U(strCN));
		labelCN->setAnchorPoint(Point(0, 0));
		labelCN->setPosition(30, layerMenuItem->getContentSize().height*0.35);
		layerMenuItem->addChild(labelCN);
		auto labelEN = Label::createWithTTF(labelStyleEN, A2U(strEN));
		labelEN->setAnchorPoint(Point(0, 1));
		labelEN->setPosition(30, layerMenuItem->getContentSize().height*0.35);
		layerMenuItem->addChild(labelEN);

		auto mouseListener = EventListenerMouse::create();
		mouseListener->onMouseDown = CC_CALLBACK_1(SceneMainMenu::menuCloseCallback, this);
		mouseListener->onMouseMove = CC_CALLBACK_1(SceneMainMenu::menuMouseMoveCallback, this);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, layerMenuItem);
	}
}

void SceneMainMenu::playBGM()
{
	pAudioSystem->loadSoundFromFile("./audio/title.mp3", AudioSystem::SOUNDTYPE::BGM);
	pAudioSystem->playBGM();
	waveDataPrev = pAudioSystem->getWaveData();
}


void SceneMainMenu::menuCloseCallback(Event* event)
{
	EventMouse* e = (EventMouse*)event;
	auto target = e->getCurrentTarget();
	Point mouseLocOri = e->getLocation();
	mouseLocOri.y = visibleSize.height - mouseLocOri.y;
	Point mouseLoc = target->convertToNodeSpace(mouseLocOri);
	Point layerPos = target->getPosition();
	Size size = target->getContentSize();
	Rect rect = Rect(target->getPosition(), size);

	auto cond = rect.containsPoint(mouseLoc);
	auto mouseButton = e->getMouseButton();
	debug->mouseButton = mouseButton;
	auto menutag = target->getTag();
	if (cond && mouseButton == 0)
	{
		switch (menutag)
		{
			case 1:
				gotoSoloScene();
				break;
			case 3:
				Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
				exit(0);
#endif
				break;
			default:
				return;
		}
	}
}

void SceneMainMenu::menuMouseMoveCallback(Event * event)
{
	EventMouse* e = (EventMouse*)event;
	auto target = e->getCurrentTarget();
	Point mouseLocOri = e->getLocation();
	debug->mousePosi = mouseLocOri;
	mouseLocOri.y = visibleSize.height - mouseLocOri.y;
	Point mouseLoc = target->convertToNodeSpace(mouseLocOri);
	Point layerPos = target->getPosition();
	Size size = target->getContentSize();
	Rect rect = Rect(target->getPosition(), size);
	auto maskLayer = target->getChildByName("mask");
	auto tag = maskLayer->getTag();

	auto cond = rect.containsPoint(mouseLoc);
	if (cond && !tag)
	{
		maskLayer->runAction(FadeTo::create(0.33, 150));
		maskLayer->setTag(1);
	}
	else if (!cond && tag)
	{
		maskLayer->runAction(FadeTo::create(0.33, 235));
		maskLayer->setTag(0);
	}
}

void SceneMainMenu::update(float dt)
{
	waveEffect();
	debugMsg();
}

void SceneMainMenu::gotoSoloScene()
{
	auto layerSceneSwitchEffect = LayerColor::create(Color4B::WHITE);
	layerSceneSwitchEffect->setOpacity(0);
	this->addChild(layerSceneSwitchEffect);
	layerSceneSwitchEffect->runAction(
		Sequence::createWithTwoActions(
			FadeTo::create(0.5, 255),
			CallFunc::create([&] {
		Director* director = Director::getInstance();
		director->pushScene(pSceneMainMenu);
		if (!pSceneFallingSolo) pSceneFallingSolo = SceneFallingSolo::createScene();
		director->replaceScene(pSceneFallingSolo);
	})
			)
		);
}

void SceneMainMenu::waveEffect()
{
	AudioSystem::WaveData sWaveData = pAudioSystem->getWaveData();
	float waveData[3] = { sWaveData.L,sWaveData.R,sWaveData.MIX };
	float waveDataPrevArr[3] = { waveDataPrev.L,waveDataPrev.R,waveDataPrev.MIX };
	float scaleTime = 0.2;
	float scaleRecoverTime = 0.35;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			if (waveData[i] > waveDataPrevArr[i])
			{
				char name[32];
				sprintf_s(name, "bezierLayer%d", i);
				auto bezierLayer = this->getChildByName("bgLayer")->getChildByName(name);
				float scaleY = 0.5 + abs(waveData[i]) * 3;
				//float scaleY = 0.5 + logf(abs(waveData[i]) + 1.5);
				if (scaleY > bezierLayer->getScaleY() && scaleY > 1)
				{
					bezierLayer->stopActionByTag(WAVE_EFFECT_TAG);
					FiniteTimeAction* waveEffect =
						Sequence::createWithTwoActions(
							ScaleTo::create(scaleTime, 1, scaleY),
							ScaleTo::create(scaleRecoverTime, 1, 0.5)
							);
					waveEffect->setTag(WAVE_EFFECT_TAG);
					bezierLayer->runAction(waveEffect);
				}
			}
			else continue;
		}
	}
	waveDataPrev.L = sWaveData.L;
	waveDataPrev.R = sWaveData.R;
	waveDataPrev.MIX = sWaveData.MIX;

}


void SceneMainMenu::debugMsg()
{
	debug->deltaTime += Director::getInstance()->getDeltaTime();
	if (debug->deltaTime > 0.032)
	{
		debug->deltaTime = 0;
		auto bgl = this->getChildByName("bgLayer");
		bgl->removeChildByName("debugLabel");
		char* msg = new char[256];
		sprintf(msg,
				"Audio Peak:\r\nLchAbs= %+2.5e\r\nRchAbs= %+2.5e\r\nMixAvg= %+2.5e\r\n\r\nMouse Position=(%.2f,%.2f)\r\nMouse Button=%d",
				abs(waveDataPrev.L),
				abs(waveDataPrev.R),
				waveDataPrev.MIX,
				debug->mousePosi.x,
				debug->mousePosi.y,
				debug->mouseButton);
		auto debuglb = debug->debugLabelMsg(msg);
		debuglb->setName("debugLabel");
		bgl->addChild(debuglb);
	}
}
