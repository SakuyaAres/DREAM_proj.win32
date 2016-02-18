#pragma once
#include "SceneFallingSolo.h"
#include "WStringSupport.h"
#include "AppDelegate.h"

#define random_r RandomHelper::random_real

USING_NS_CC;

Scene* SceneFallingSolo::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = SceneFallingSolo::create();
	// add layer as a child to scene
	layer->setAnchorPoint(Point(0.5, 0.5));
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool SceneFallingSolo::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	createBG();
	createHud();


	//Create debug class
	debug = new Debug();

	scheduleUpdate();

	return true;
}

void SceneFallingSolo::createBG()
{
	auto layerBG = LayerColor::create(Color4B::WHITE);
	layerBG->setName("bgLayer");
	this->addChild(layerBG);
}

void SceneFallingSolo::createHud()
{
	auto layerHudTL = LayerColor::create(pUI->themeColorB(255), visibleSize.width*0.25, visibleSize.height*0.1);
	layerHudTL->setPosition(Point(0, visibleSize.height - layerHudTL->getContentSize().height));
	layerHudTL->setOpacity(0);
	this->addChild(layerHudTL);
	layerHudTL->runAction(
		FadeTo::create(0.5, 255)
		);
}

void SceneFallingSolo::playBGM()
{
	pAudioSystem->loadSoundFromFile("./audio/title.mp3", AudioSystem::SOUNDTYPE::BGM);
	pAudioSystem->playBGM(0);
	waveDataPrev = pAudioSystem->getWaveData();
}


void SceneFallingSolo::update(float dt)
{
}

void SceneFallingSolo::debugMsg()
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
