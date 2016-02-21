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
	float singleNoteHeight = 10;
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
	float curTime;
	float topTime;
	int dspClockInit;
	float deltaTime;
	struct TimeSegPara
	{
		int startTime;
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
		bool resized = false;
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
	int getTimeSeg(int timePoint);

	private:
	void setBGMStat();
	bool setCurTime(float dt);
	void moveNotesInField();
	void setTopTime();
	void drawNewNotes();
	float getNoteOffsetY(int timePoint);
	Node* createNote(vector<NoteManager::Note>::iterator noteIter, int trackIndex);
	void setNewNotePos(NoteObj* note, int trackIndex);
	void resizeLongNotes();
	void removeOldNotes();
	void deleteNote(int trackIndex, vector<NoteObj>::iterator noteIter);

	void autoPlay();

	public:
	int getCurTime();
	int getTopTime();
};