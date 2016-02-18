#include "debug.h"
#include "WStringSupport.h"

Debug::Debug() :
	debugMsg(""),
	labelMsg(nullptr),
	deltaTime(0),
	mousePosi(Point(0,0)),
	mouseButton(0)
{
	ttfStyle = new TTFConfig();
	ttfStyle->fontFilePath = "fonts/FZHTJW.TTF";
	ttfStyle->fontSize = 14;
	ttfStyle->distanceFieldEnabled = false;
	ttfStyle->glyphs = GlyphCollection::DYNAMIC;
	ttfStyle->customGlyphs = nullptr;
}

Debug::~Debug()
{
	safeRelease(ttfStyle);
	safeRelease(debugLayer);
}

Label* Debug::debugLabelMsg(const char * msg)
{

	//labelMsg->autorelease();
	labelMsg = Label::createWithTTF(*ttfStyle, A2U(msg), TextHAlignment::LEFT);
	labelMsg->enableOutline(Color4B(0, 0, 0, 192), 1);
	labelMsg->setAnchorPoint(Point(0, 0));
	labelMsg->setPosition(Point(0, 150));
	labelMsg->setGlobalZOrder(99.f);
	return labelMsg;
}

void Debug::safeRelease(void* obj)
{
	free(obj);
	obj = nullptr;
}
