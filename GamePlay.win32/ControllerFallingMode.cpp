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
	pAudioSystem->loadSoundFromFile("musicdata/296478 FLOOR LEGENDS -KAC 2012- - KAC 2012 ULTIMATE MEDLEY -HISTORIA SOUND VOLTEX-/" + pNoteMgr->musicFile, AudioSystem::SOUNDTYPE::LOAD_BGM, true);
	dspClockInit = pAudioSystem->getDSPClockInMS();
}

void ControllerFallingMode::updateNote(float dt)
{
	setBGMStat();
	setCurTime(dt);
	moveNotesInField();
	setTopTime();
	drawNewNotes();
	resizeLongNotes();
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
	//int dspClock = pAudioSystem->getDSPClockInMS();
}

void ControllerFallingMode::setCurTime(float dt)
{
	if (curTime >= 0)
	{
		deltaTime = pAudioSystem->getBgmPosition() - curTime;
		curTime = pAudioSystem->getBgmPosition();
		//deltaTime = pAudioSystem->getDSPClockInMS() - dspClockInit - curTime;
		//curTime = pAudioSystem->getDSPClockInMS() - dspClockInit;
		if (timeSegIndex < pNoteMgr->timingSegsSize - 1)
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
			note.obj = createNote(iter, i);
			note.obj->setPosition(Point(trackWidth*i, noteFieldHeight - (topTime - iter->startTime)*pTimeSegPara[findTimeSeg(topTime)].noteSpeedPerMs));
			pNoteField->addChild(note.obj);
			note.info = *iter;
			pNoteMgr->trackNotes[i].erase(iter);
			notesInTrack[i].push_back(note);
		}
	}
}

void ControllerFallingMode::resizeLongNotes()
{
	for (int i = 0; i < trackCount; i++)
	{
		for (vector<NoteObj>::iterator iter = notesInTrack[i].begin(); iter != notesInTrack[i].end(); iter++)
		{
			if (iter->info.noteType == NoteManager::Note::NoteType::LONG && !iter->resized)
			{
				float posy = iter->obj->getPositionY();
				float noteHeight = noteFieldHeight - posy + singleNoteHeight;
				if (iter->info.endTime > topTime)
				{
					iter->obj->setContentSize(Size(trackWidth, noteHeight));
				}
				else
				{
					float endToTopDist = (topTime - iter->info.endTime)*pTimeSegPara[findTimeSeg(topTime)].noteSpeedPerMs;
					iter->obj->setContentSize(Size(trackWidth, noteHeight - endToTopDist));
					iter->resized = true;
				}
			}
		}
	}
}

Node * ControllerFallingMode::createNote(vector<NoteManager::Note>::iterator noteIter, int trackIndex)
{
	Node* noteobj = LayerColor::create(
		Color4B(225 + 30 * (trackIndex % 2), 225, 225 + 30 * (1 - trackIndex % 2), 255),
		trackWidth,
		singleNoteHeight);
	//if (noteIter->noteType == NoteManager::Note::NoteType::LONG)
	//{
	//	float height = 0;
	//	int startTime = noteIter->startTime;
	//	for (int i = timeSegIndex; i < pNoteMgr->timingSegsSize; i++)
	//	{
	//		int segTime = pNoteMgr->timingSegs[i].startTime;
	//		if (segTime<startTime) continue;
	//		if (noteIter->endTime > segTime)
	//		{
	//			height += (segTime - startTime)*pTimeSegPara[i].noteSpeedPerMs;
	//			startTime = segTime;
	//		}
	//		else
	//		{
	//			height += (noteIter->endTime - startTime)*pTimeSegPara[i].noteSpeedPerMs;
	//			break;
	//		}
	//	}
	//	noteobj->setContentSize(Size(trackWidth, height));
	//}
	return noteobj;
}

void ControllerFallingMode::removeOldNotes()
{
	for (int i = 0; i < trackCount; i++)
	{
		if (notesInTrack[i].size() == 0) continue;
		vector<NoteObj>::iterator iter = notesInTrack[i].begin();
		if (iter->info.noteType == NoteManager::Note::NoteType::SINGLE && (iter->hitting || curTime - iter->info.startTime>HITRANGE::MISS + 200))
		{
			deleteNote(i, iter);
		}
		else if (iter->info.noteType == NoteManager::Note::NoteType::LONG && (iter->hitting || curTime - iter->info.endTime > HITRANGE::MISS + 200))
		{
			deleteNote(i, iter);
		}


	}
}

void ControllerFallingMode::deleteNote(int trackIndex, vector<NoteObj>::iterator noteIter)
{
	noteIter->obj->removeFromParentAndCleanup(true);
	notesInTrack[trackIndex].erase(noteIter);
}

int ControllerFallingMode::findTimeSeg(int timePoint)
{
	int index = -2;
	for (int i = 0; i < pNoteMgr->timingSegsSize; i++)
	{
		if (timePoint < pNoteMgr->timingSegs[i].startTime) index = i - 1;
	}
	if (index >= 0) return index;
	if (index < -1) return pNoteMgr->timingSegsSize - 1;
	else return 0;
}
