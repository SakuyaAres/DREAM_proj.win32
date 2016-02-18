#pragma once
#include "cocos2d.h"

USING_NS_CC;

class Debug
{
	public:
	char* debugMsg = new char[512];
	TTFConfig* ttfStyle;
	Layer* debugLayer;
	Label* labelMsg;
	float deltaTime;
	Point mousePosi;
	int mouseButton;

	public:
	Debug();
	~Debug();

	public:
	Label* debugLabelMsg(const char* msg);

	private:
	void safeRelease(void* obj);
};