#pragma once
#include "SceneFallingSolo.h"
#include "AppDelegate.h"


#define random_r RandomHelper::random_real

USING_NS_CC;

Scene* SceneFallingSolo::createScene()
{
	auto scene = Scene::create();
	auto layer = SceneFallingSolo::create();
	layer->setAnchorPoint(Point(0.5, 0.5));
	scene->addChild(layer);
	return scene;
}

bool SceneFallingSolo::init()
{

	if (!Layer::init())
	{
		return false;
	}

	createBG();
	createHud();
	pGameController = new ControllerFallingMode("musicdata/198380 sakuzyo - Neurotoxin/sakuzyo - Neurotoxin (Rumia-) [Black Another].osu");
	
	createPlayerPanel(pGameController->getTrackCount());
	pGameController->setNoteField(this->getChildByName("panelFrame")->getChildByName("panelClip")->getChildByName("panelNoteField"));

	//pNoteManager = new NoteManager("musicdata/Run Through the Sky (Short)/I've - Run Through the Sky (Sakuya Ares) [5K Hard].osu");
	//auto scene = Director::getInstance()->getRunningScene();

	//Create debug class
	debug = new Debug();

	createFadeInMask();

	//Audio System operation
	pAudioSystem->stopBGM();

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
	Size panelSize = Size(visibleSize.width / 5 + 20 * trackCount, visibleSize.height);
	float trackWidth = panelSize.width / trackCount;
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
	clipNode->setName("panelClip");
	layerPanelFrame->addChild(clipNode);
	auto layerGrad = LayerGradient::create(Color4B(0, 0, 0, 96), Color4B(0, 0, 0, 192));
	clipNode->addChild(layerGrad);
	auto layerNoteField = LayerColor::create(Color4B(0, 0, 0, 0), panelSize.width, panelSize.height * 7 / 8);
	layerNoteField->setPosition(Point(1, visibleSize.height / 8));
	layerNoteField->setName("panelNoteField");
	clipNode->addChild(layerNoteField);
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
	pGameController->updateNote(dt);
	debugMsg();
}

void SceneFallingSolo::debugMsg()
{
	debug->deltaTime += Director::getInstance()->getDeltaTime();
	if (debug->deltaTime > 0)
	{
		debug->deltaTime = 0;
		auto bgl = this->getChildByName("bgLayer");
		bgl->removeChildByName("debugLabel");
		char* msg = new char[256];
		sprintf(msg,
				"Top time:%d\r\nCur time:%d",
				pGameController->getTopTime(),
				pGameController->getCurTime()
				);
		auto debuglb = debug->debugLabelMsg(msg);
		debuglb->setName("debugLabel");
		bgl->addChild(debuglb);
	}

}
