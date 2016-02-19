#pragma once
#include "AppDelegate.h"
#include "cocos2d.h"
#include "debug.h"
#include "NoteManager.h"

USING_NS_CC;

class ControllerFallingMode
{
	public:
	ControllerFallingMode(string noteFilePath);
	~ControllerFallingMode() {};


	public:
	NoteManager* pNoteMgr;
	Node* pNoteField;
	float baseSpeed = 2;
	bool isPaused = false;

	enum HITRANGE
	{
		GREAT = 30,
		GOOD = 60,
		POOR = 90,
		MISS = 120
	};

	private:
	float noteFieldHeight;
	float trackWidth;
	int trackCount;
	unsigned int bgmLength;
	bool bgmStarted = false;
	bool bgmIsPausedPrev = false;
	int curTime;
	int topTime;
	int deltaTime;
	struct TimeSegPara
	{
		float noteSpeedPerMs; // GLPoint per ms
		float timeFlowSpeedPerGLP; // ms per GLPoint
	};
	TimeSegPara* pTimeSegPara;
	int timeSegIndex = 0;
	struct NoteObj
	{
		Node* obj;
		NoteManager::Note info;
		bool hitting = false;
	};
	vector<NoteObj>* notesInTrack;
	int* indexInTrack;

	public:
	void initNoteManager(string noteFilePath);
	void setNoteField(Node* noteField);
	void initGamePlay();
	void setTimeSegPara();
	void updateNote(float dt);

	public:
	int getTrackCount();

	private:
	void setBGMStat();
	void setCurTime(float dt);
	void moveNotesInField();
	void setTopTime();
	void drawNewNotes();
	void removeOldNotes();
};