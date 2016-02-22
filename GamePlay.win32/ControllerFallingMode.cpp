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

void ControllerFallingMode::setPlayPanel(Node* panelFrame)
{
	pPanelFrame = panelFrame;
	pNoteField = pPanelFrame->getChildByName("panelClip")->getChildByName("panelNoteField");
	pKeyButtons = new Node*[trackCount];
	for (int i = 0; i < trackCount; i++)
	{
		char keyname[5];
		sprintf_s(keyname, 5, "key%d", i);
		pKeyButtons[i] = pPanelFrame->getChildByName(keyname);
		pKeyButtons[i]->runAction(
			RepeatForever::create(
				Sequence::createWithTwoActions(
					FadeTo::create(0.02, 100),
					FadeTo::create(0.02, 185)
					)
				)
			);
	}
	noteFieldHeight = pNoteField->getContentSize().height;
	trackWidth = pNoteField->getContentSize().width / pNoteMgr->trackCount;
	setTimeSegPara();
}

void ControllerFallingMode::initGamePlay()
{
	curTime = 0;
	trackCount = pNoteMgr->trackCount;
	notesInTrack = new vector<NoteObj>[trackCount];
	//indexInTrack = new int[trackCount];
	//inKeys = new vector<EventKeyboard>[trackCount];
	//for (int i = 0; i < trackCount; i++) indexInTrack[i] = 0;
	bgmLength = pAudioSystem->getBgmLength();
	pAudioSystem->loadSoundFromFile("musicdata/198380 sakuzyo - Neurotoxin/" + pNoteMgr->musicFile, AudioSystem::SOUNDTYPE::LOAD_BGM, true);
	pAudioSystem->loadSoundFromFile("musicdata/198380 sakuzyo - Neurotoxin/soft-hitnormal7.wav", AudioSystem::SOUNDTYPE::SFX, false);
	dspClockInit = pAudioSystem->getDSPClockInMS();
}

void ControllerFallingMode::updateNote(float dt)
{
	updateAudio();
	setCurTime(dt);
	moveNotesInField();
	setTopTime();
	drawNewNotes();
	//resizeLongNotes();
	removeOldNotes();
	autoPlay();
}

void ControllerFallingMode::pushKey(EventKeyboard::KeyCode keycode, bool isPressed)
{
	KeyInfo key;
	key.keycode = keycode;
	key.isPressed = isPressed;
	inKeys.push_back(key);
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
		pTimeSegPara[i].startTime = i == 0 ? 0 : pNoteMgr->timingSegs[i].startTime;
		pTimeSegPara[i].noteSpeedPerMs = baseSpeed*speedmod*noteFieldHeight / (mspb * 4);
		pTimeSegPara[i].timeFlowSpeedPerGLP = 1 / pTimeSegPara[i].noteSpeedPerMs;
	}
}

void ControllerFallingMode::updateAudio()
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
	pAudioSystem->update();
}

bool ControllerFallingMode::setCurTime(float dt)
{
	bool waitFlag = false;
	unsigned int bgmTime = pAudioSystem->getBgmPosition();
	if (curTime >= 0 && bgmTime > 0)
	{
		//deltaTime = pAudioSystem->getBgmPosition() - curTime;
		//curTime = pAudioSystem->getBgmPosition();
		deltaTime = Director::getInstance()->getDeltaTime() * 1000;
		float gameTime = curTime + deltaTime;
		if (gameTime < (float)bgmTime - 10)
		{
			//curTime = bgmTime;
			curTime = gameTime + deltaTime*0.15;
		}
		else if (gameTime>(float)bgmTime + 10)
		{
			//waitFlag = true;
			curTime = gameTime - deltaTime*0.15;
		}
		else
		{
			curTime = gameTime;
		}
		if (timeSegIndex < pNoteMgr->timingSegsSize - 1 && curTime >= pNoteMgr->timingSegs[timeSegIndex + 1].startTime)
		{
			timeSegIndex++;
		}
	}
	//else
	//{
	//	curTime += Director::getInstance()->getDeltaTime() * 1000;
	//}
	return waitFlag;
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
				iter->obj->setPositionY(iter->obj->getPositionY() - pTimeSegPara[timeSegIndex].noteSpeedPerMs*deltaTime);
				//iter->obj->setPositionY((iter->info.startTime - curTime)*pTimeSegPara[timeSegIndex].noteSpeedPerMs);
				//iter->obj->setPositionY(getNoteOffsetY(iter->info.startTime));
			}
		}
	}
}

void ControllerFallingMode::setTopTime()
{
	float virtualTopTime;
	virtualTopTime = curTime + pTimeSegPara[timeSegIndex].timeFlowSpeedPerGLP*noteFieldHeight;
	int topTimeSegIndex = getTimeSeg(virtualTopTime);
	if (timeSegIndex == topTimeSegIndex)
	{
		topTime = virtualTopTime;
	}
	else
	{
		//float timeLine = pNoteMgr->timingSegs[topTimeSegIndex].startTime;
		//float timeLinePos = (timeLine - curTime)*pTimeSegPara[topTimeSegIndex-1].noteSpeedPerMs;
		//topTime = timeLine + (noteFieldHeight - timeLinePos)*pTimeSegPara[topTimeSegIndex].timeFlowSpeedPerGLP;
		float offsetTime = curTime;
		float linePos = 0;
		int nearestSegIndex = timeSegIndex;
		for (int i = timeSegIndex; i < topTimeSegIndex; i++)
		{
			int nextLineTime = pTimeSegPara[i + 1].startTime;
			float nextLinePos = (nextLineTime - offsetTime)*pTimeSegPara[i].timeFlowSpeedPerGLP;
			if (nextLinePos + linePos < noteFieldHeight)
			{
				offsetTime = nextLineTime;
				linePos += nextLinePos;
			}
			nearestSegIndex = i;
		}
		offsetTime += (noteFieldHeight - linePos) *pTimeSegPara[nearestSegIndex].timeFlowSpeedPerGLP;
		topTime = offsetTime;
	}

}

void ControllerFallingMode::drawNewNotes()
{
	for (int i = 0; i < trackCount; i++)
	{
		if (pNoteMgr->trackNotes[i].size() == 0) continue;
		//vector<NoteManager::Note>* trackNotes = &pNoteMgr->trackNotes[i];
		for (vector<NoteManager::Note>::iterator iter = pNoteMgr->trackNotes[i].begin(); iter != pNoteMgr->trackNotes[i].end();)
		{
			if (iter->startTime < topTime + 2000)
			{
				NoteObj note;
				float notePosY = getNoteOffsetY(iter->startTime);
				note.obj = LayerColor::create(
					Color4B(225 + 30 * (i % 2), 225, 225 + 30 * (1 - i % 2), 255),
					trackWidth,
					singleNoteHeight);
				note.info = *iter;
				note.obj->setPosition(trackWidth*i, getNoteOffsetY(iter->startTime));
				if (note.info.noteType == NoteManager::Note::NoteType::LONG) note.obj->setContentSize(Size(trackWidth, getNoteOffsetY(iter->endTime) - notePosY + singleNoteHeight));
				pNoteField->addChild(note.obj);
				iter = pNoteMgr->trackNotes[i].erase(iter);
				notesInTrack[i].push_back(note);
			}
			else break;
		}
	}
}

float ControllerFallingMode::getNoteOffsetY(int timePoint)
{
	int baseTime = curTime;
	int targetSegIndex = getTimeSeg(timePoint);
	float posy = 0;
	for (int i = timeSegIndex; i < targetSegIndex; i++)
	{
		int nextLineTime = pTimeSegPara[i + 1].startTime;
		posy += (nextLineTime - baseTime)*pTimeSegPara[i].noteSpeedPerMs;
		baseTime = nextLineTime;
	}
	posy += (timePoint - baseTime)*pTimeSegPara[targetSegIndex].noteSpeedPerMs;

	return posy;
}

Node * ControllerFallingMode::createNote(vector<NoteManager::Note>::iterator noteIter, int trackIndex)
{
	Node* noteobj = LayerColor::create(
		Color4B(225 + 30 * (trackIndex % 2), 225, 225 + 30 * (1 - trackIndex % 2), 255),
		trackWidth,
		singleNoteHeight);
	return noteobj;
}

void ControllerFallingMode::setNewNotePos(NoteObj* note, int trackIndex)
{
	int baseTime = curTime;
	int startTime = note->info.startTime;
	int noteTimeSegIndex = getTimeSeg(startTime);
	float posy = 0;
	for (int i = timeSegIndex; i < noteTimeSegIndex; i++)
	{
		int nextLineTime = pTimeSegPara[i + 1].startTime;
		posy += (nextLineTime - baseTime)*pTimeSegPara[i].noteSpeedPerMs;
		baseTime = nextLineTime;
	}
	posy += (startTime - baseTime)*pTimeSegPara[noteTimeSegIndex].noteSpeedPerMs;
	note->obj->setPosition(trackWidth*trackIndex, posy);
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
					float endToTopDist = (topTime - iter->info.endTime)*pTimeSegPara[getTimeSeg(topTime)].noteSpeedPerMs;
					iter->obj->setContentSize(Size(trackWidth, noteHeight - endToTopDist));
					iter->resized = true;
				}
			}
		}
	}
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
		else if (iter->info.noteType == NoteManager::Note::NoteType::LONG && curTime - iter->info.endTime > HITRANGE::MISS + 200)
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

void ControllerFallingMode::autoPlay()
{
	for (int i = 0; i < trackCount; i++)
	{
		for (vector<NoteObj>::iterator iter = notesInTrack[i].begin(); iter != notesInTrack[i].end(); iter++)
		{
			if (iter->info.startTime <= curTime && !iter->hitting)
			{
				iter->hitting = true;
				pAudioSystem->playSFX(0);
				auto hitEffect = Sprite::create("image/FallingHitLight.png");
				hitEffect->ignoreAnchorPointForPosition(false);
				hitEffect->setAnchorPoint(Point(0.5, 0.25));
				hitEffect->setOpacity(255);
				hitEffect->setPosition(Point(trackWidth*(i + 0.5), visibleSize.height / 8));
				hitEffect->setScale(2, 0);
				//sprintf_s(name, "hit%d", i);
				pPanelFrame->addChild(hitEffect);
				hitEffect->runAction(
					Sequence::createWithTwoActions(
						ScaleTo::create(0.15, 0, 1),
						RemoveSelf::create()
						)
					);
			}
		}
	}
}

float ControllerFallingMode::getCurTime()
{
	return curTime;
}

float ControllerFallingMode::getTopTime()
{
	return topTime;
}

int ControllerFallingMode::getTimeSeg(int timePoint)
{
	int index = -1;
	for (int i = 0; i < pNoteMgr->timingSegsSize; i++)
	{
		int linePoint = 0;
		if (i > 0)linePoint = pNoteMgr->timingSegs[i].startTime;
		if (timePoint < linePoint)
		{
			index = i - 1;
			break;
		}
	}
	if (index >= 0) return index;
	else return pNoteMgr->timingSegsSize - 1;
}
