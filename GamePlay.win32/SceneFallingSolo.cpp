#pragma once
#include "SceneFallingSolo.h"
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

	noteManager = new NoteManager("musicdata/Run Through the Sky (Short)/I've - Run Through the Sky (Sakuya Ares) [5K Hard].osu");

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



void SceneFallingSolo::update(float dt)
{
}

void SceneFallingSolo::debugMsg()
{
}
