#include "NoteManager.h"
#include "WStringSupport.h"
#include "StringTools.h"


using namespace std;
using namespace strtool;

NoteManager::NoteManager(string filepath)
{
	loadfile(filepath);
}

NoteManager::~NoteManager()
{
}

void NoteManager::loadfile(string filepath)
{
	//ifstream file;
	file.open(filepath);
	if (file.eof()) return;

	string buffer;
	getline(file, buffer);
	if (strcmp(buffer.substr(0, 3).c_str(), "osu") == 0)
		loadOsuFile();
}

void NoteManager::loadOsuFile()
{
	string buffer = "";
	//char option[16];
	while (!file.eof())
	{
		if (buffer[0] != '[')
		{
			getline(file, buffer);
			continue;
		}
		if (
			buffer.compare("[General]") == 0 ||
			buffer.compare("[Metadata]") == 0 ||
			buffer.compare("[Difficulty]") == 0
			)
		{
			loadOsuFileInfo(buffer);
		}
		else if (buffer.compare("[TimingPoints]") == 0) loadOsuFileTiming(buffer);
		else if (buffer.compare("[HitObjects]") == 0) loadOsuFileNote(buffer);
		else
		{
			getline(file, buffer); continue;
		}
	}
}

void NoteManager::loadOsuFileInfo(string& retBuffer)
{
	string linedata;
	string name;
	string value;
	while (!file.eof())
	{
		getline(file, linedata);
		if (linedata[0] == '[')
		{
			retBuffer = linedata;
			break;
		}
		if (linedata.empty()) continue;
		vector<string> data;
		split(trim(linedata), data, ":");
		if (data.empty()) continue;
		else name = data[0];
		if (data.size() > 1) value = trim(data[1]);
		else value = "";
		if (name.compare("AudioFilename") == 0) { musicFile = value; continue; }
		if (name.compare("PreviewTime") == 0) { previewPoint = atoi(value.c_str()); continue; }
		if (name.compare("Mode") == 0) { gameMode = atoi(value.c_str()); continue; }
		if (name.compare("Title") == 0) { metaData.title = value; continue; }
		if (name.compare("TitleUnicode") == 0) { metaData.titleUTF = A2U(value); continue; }
		if (name.compare("Artist") == 0) { metaData.artist = value; continue; }
		if (name.compare("TitleUnicode") == 0) { metaData.artistUTF = A2U(value); continue; }
		if (name.compare("Creator") == 0) { metaData.noter = value; continue; }
		if (name.compare("Version") == 0) { metaData.version = value; continue; }
		if (name.compare("Source") == 0) { metaData.source = value; continue; }
		if (name.compare("Tags") == 0) { metaData.tag = value; continue; }
		if (name.compare("CircleSize") == 0) { trackCount = atoi(value.c_str()); continue; }
	}
}

void NoteManager::loadOsuFileTiming(string & retBuffer)
{
	string linedata;
	vector<Timing> vTimings;
	Timing timing;
	while (!file.eof())
	{
		getline(file, linedata);
		if (linedata[0] == '[')
		{
			retBuffer = linedata;
			break;
		}
		if (linedata.empty()) continue;
		vector<string> data;
		split(trim(linedata), data, ",");
		if (data.empty() || data.size() < 8) continue;
		timing.startTime = atof(data[0].c_str());
		timing.mspb = atof(data[1].c_str());
		if (timing.mspb > 0)
		{
			timing.bpm = 60000 / timing.mspb;
			timing.speedModer = 1.0;
		}
		else timing.speedModer = 100 / timing.mspb * -1;
		timing.isHighlight = atoi(data[7].c_str()) == 1 ? true : false;
		vTimings.push_back(timing);
	}
	timingSegsSize = vTimings.size();
	timingSegs = new Timing[timingSegsSize];
	for (int i = 0; i < timingSegsSize; i++)
	{
		if (i > 0 && vTimings[i].mspb < 0) vTimings[i].mspb = vTimings[i - 1].mspb;
		timingSegs[i] = vTimings[i];
	}
}

void NoteManager::loadOsuFileNote(string & retBuffer)
{
	string linedata;
	vector<Note> vNotes;
	Note note;
	int maxPosX = 0;
	while (!file.eof())
	{
		getline(file, linedata);
		if (linedata.empty()) continue;
		vector<string> data;
		split(trim(linedata), data, ",");
		note.positionX = atoi(data[0].c_str());
		if (note.positionX > maxPosX) maxPosX = note.positionX;
		note.startTime = atoi(data[2].c_str());
		char endtime[8];
		sscanf(data[5].c_str(), "%[^:]", endtime);
		note.endTime = atoi(endtime);
		note.noteType = note.endTime ? note.LONG : note.SINGLE;
		vNotes.push_back(note);
	}
	int divisor = maxPosX / trackCount;
	trackNotes = new vector<Note>[trackCount];
	for (int i = 0; i < vNotes.size(); i++)
	{
		int trackNo = vNotes[i].positionX / divisor;
		trackNotes[trackNo < trackCount ? trackNo : trackCount - 1].push_back(vNotes[i]);
	}
}
