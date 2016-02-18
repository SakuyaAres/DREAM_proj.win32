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
		float mpb = atof(data[1].c_str());
		if (mpb > 0)
		{
			timing.bpm = 60000 / mpb;
			timing.speedModer = 1.0;
		}
		else timing.speedModer = 100 / mpb * -1;
		timing.isHighlight = atoi(data[7].c_str()) == 1 ? true : false;
		vTimings.push_back(timing);
	}
	timingLinesSize = vTimings.size();
	timingLines = new Timing[timingLinesSize];
	for (int i = 0; i < timingLinesSize; i++)
	{
		timingLines[i] = vTimings[i];
	}
}
