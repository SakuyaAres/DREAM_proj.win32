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
	pNoteManager = new NoteManager("musicdata/Run Through the Sky (Short)/I've - Run Through the Sky (Sakuya Ares) [5K Hard].osu");

	createPlayerPanel(pNoteManager->trackCount);

	//Create debug class
	debug = new Debug();

	createFadeInMask();
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
	layerHudTL->setPosition(Point(0, visibleSize.height*0.9));
	this->addChild(layerHudTL);
}

void SceneFallingSolo::createPlayerPanel(int trackCount)
{
	float trackWidth = visibleSize.width / 16;
	Size panelSize = Size(trackWidth*trackCount, visibleSize.height);
	auto layerPanelFrame = LayerColor::create(pUI->themeColorB(0), panelSize.width, panelSize.height);
	layerPanelFrame->setPosition((visibleSize.width - panelSize.width) / 2, 0);
	layerPanelFrame->setName("panelFrame");
	this->addChild(layerPanelFrame);
	auto frameNode = DrawNode::create();
	layerPanelFrame->addChild(frameNode);
	frameNode->drawLine(Point(0, 0), Point(0, panelSize.height), pUI->themeColorF(255));
	frameNode->drawLine(Point(panelSize.width, 0), Point(panelSize.width, panelSize.height), pUI->themeColorF(255));
	frameNode->drawSolidRect(Point(0, 0), Point(panelSize.width, visibleSize.height / 8), pUI->themeColorF(255));

	Rect clipRect = Rect(1, visibleSize.height / 8, panelSize.width - 2, panelSize.height);
	auto clipNode = ClippingRectangleNode::create(clipRect);
	clipNode->setName("panelClipNode");
	layerPanelFrame->addChild(clipNode);
	auto layerGrad = LayerGradient::create(Color4B(0, 0, 0, 96), Color4B(0, 0, 0, 192));
	clipNode->addChild(layerGrad);
}

void SceneFallingSolo::createFadeInMask()
{
	auto layerFadeInMask = LayerColor::create(Color4B::WHITE);
	layerFadeInMask->setPosition(Point(0, visibleSize.height - layerFadeInMask->getContentSize().height));
	layerFadeInMask->setName("layerFadeInMask");
	this->addChild(layerFadeInMask);
	layerFadeInMask->runAction(
		Sequence::createWithTwoActions(
			FadeTo::create(0.5, 0),
			CallFunc::create([&] { this->removeChildByName("layerFadeInMask"); })
			)
		);
}



void SceneFallingSolo::update(float dt)
{
}

void SceneFallingSolo::debugMsg()
{
}
