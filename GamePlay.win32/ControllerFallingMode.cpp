#include "ControllerFallingMode.h"

ControllerFallingMode::ControllerFallingMode(string noteFilePath)
{
	initNoteManager(noteFilePath);

	initGamePlay();
}

void ControllerFallingMode::initNoteManager(string noteFilePath)
{
	pNoteMgr = new NoteManager(noteFilePath);
}

void ControllerFallingMode::setNoteField(Node* noteField)
{
	pNoteField = noteField;
	noteFieldHeight = pNoteField->getContentSize().height;
	trackWidth = pNoteField->getContentSize().width / pNoteMgr->trackCount;
	setTimeSegPara();
}

void ControllerFallingMode::initGamePlay()
{
	curTime = -3;
	trackCount = pNoteMgr->trackCount;
	notesInTrack = new vector<NoteObj>[trackCount];
	indexInTrack = new int[trackCount];
	for (int i = 0; i < trackCount; i++) indexInTrack[i] = 0;
	bgmLength = pAudioSystem->getBgmLength();
	pAudioSystem->loadSoundFromFile("musicdata/Run Through the Sky (Short)/" + pNoteMgr->musicFile, AudioSystem::SOUNDTYPE::LOAD_BGM);
}

void ControllerFallingMode::updateNote(float dt)
{
	setBGMStat();
	setCurTime(dt);
	moveNotesInField();
	setTopTime();
	drawNewNotes();
	removeOldNotes();
}

int ControllerFallingMode::getTrackCount()
{
	return trackCount;
}

void ControllerFallingMode::setTimeSegPara()
{
	pTimeSegPara = new TimeSegPara[pNoteMgr->timingSegsSize];
	for (int i = 0; i < pNoteMgr->timingSegsSize; i++)
	{
		float mspb = pNoteMgr->timingSegs[i].mspb;
		float speedmod = pNoteMgr->timingSegs[i].speedModer;
		pTimeSegPara[i].noteSpeedPerMs = baseSpeed*speedmod*noteFieldHeight / (mspb * 4);
		pTimeSegPara[i].timeFlowSpeedPerGLP = 1 / pTimeSegPara[i].noteSpeedPerMs;
	}
}

void ControllerFallingMode::setBGMStat()
{
	if (curTime >= 0 && !bgmStarted)
	{
		pAudioSystem->playBGM();
		bgmStarted = true;
	}
	if (isPaused != bgmIsPausedPrev)
	{
		if (isPaused) pAudioSystem->pauseBGM();
		else pAudioSystem->resumeBGM();
		bgmIsPausedPrev = isPaused;
	}
}

void ControllerFallingMode::setCurTime(float dt)
{
	if (curTime >= 0)
	{
		deltaTime = pAudioSystem->getBgmPosition() - curTime;
		curTime = pAudioSystem->getBgmPosition();
		if (timeSegIndex < pNoteMgr->timingSegsSize-1)
		{
			if (curTime >= pNoteMgr->timingSegs[timeSegIndex + 1].startTime)
			{
				timeSegIndex++;
			}
		}
	}
	else
	{
		curTime += dt * 1000;
	}
}

void ControllerFallingMode::moveNotesInField()
{
	for (int i = 0; i < pNoteMgr->trackCount; i++)
	{
		int noteCount = notesInTrack[i].size();
		if (noteCount > 0 && deltaTime>0)
		{
			for (Vector<NoteObj>::const_iterator iter = notesInTrack[i].cbegin(); iter != notesInTrack[i].cend(); iter++)
			{
				//iter->obj->setPositionY(iter->obj->getPositionY() - pTimeSegPara[timeSegIndex].noteSpeedPerMs*deltaTime);
				iter->obj->stopAllActions();
				iter->obj->runAction(MoveBy::create(deltaTime / 1000.f, Point(0, -pTimeSegPara[timeSegIndex].noteSpeedPerMs*deltaTime)));
			}
		}
	}
}

void ControllerFallingMode::setTopTime()
{
	int virtualTopTime;
	virtualTopTime = curTime + pTimeSegPara[timeSegIndex].timeFlowSpeedPerGLP*noteFieldHeight;
	
	if (timeSegIndex < pNoteMgr->timingSegsSize - 1)
	{
		int timeline = pNoteMgr->timingSegs[timeSegIndex + 1].startTime;
		if (virtualTopTime <= timeline)
		{
			topTime = virtualTopTime;
		}
		else
		{
			float timeSegLinePos = (timeline - curTime)*pTimeSegPara[timeSegIndex].noteSpeedPerMs;
			topTime = timeline + (noteFieldHeight - timeSegLinePos)*pTimeSegPara[timeSegIndex + 1].timeFlowSpeedPerGLP;
		}
	}
	else topTime = virtualTopTime;
}

void ControllerFallingMode::drawNewNotes()
{
	for (int i = 0; i < trackCount; i++)
	{
		if (pNoteMgr->trackNotes[i].size() == 0) continue;
		vector<NoteManager::Note>::iterator iter = pNoteMgr->trackNotes[i].begin();
		if (iter->startTime < topTime)
		{
			NoteObj note;
			note.obj = LayerColor::create(Color4B::WHITE, trackWidth, 10);
			note.obj->setPosition(Point(trackWidth*i, noteFieldHeight));
			pNoteField->addChild(note.obj);
			note.info = *iter;
			pNoteMgr->trackNotes[i].erase(iter);
			notesInTrack[i].push_back(note);
		}
	}
}

void ControllerFallingMode::removeOldNotes()
{
	for (int i = 0; i < trackCount; i++)
	{
		if (notesInTrack[i].size() == 0) continue;
		vector<NoteObj>::iterator iter = notesInTrack[i].begin();
		if (iter->hitting || curTime - iter->info.startTime>HITRANGE::MISS+200)
		{
			iter->obj->removeFromParentAndCleanup(true);
			notesInTrack[i].erase(iter);
		}
	}
}
