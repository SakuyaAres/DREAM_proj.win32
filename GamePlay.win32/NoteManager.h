#pragma once
#include <string>
#include <fstream>
#include <vector>

using namespace std;

class NoteManager
{
	public:
	ifstream file;
	string musicFile;
	int previewPoint;
	int gameMode;
	int difficulty;
	int level;
	int trackCount;
	struct MetaData
	{
		string title;
		string titleUTF;
		string artist;
		string artistUTF;
		string noter;
		string version;
		string source;
		string tag;
	};
	MetaData metaData;

	struct Timing
	{
		int startTime;
		bool isHighlight;
		double bpm;
		float speedModer;
	};
	Timing* timingLines;
	int timingLinesSize;
	
	struct Note
	{
		int positionX;
		int startTime;
		enum NoteType
		{
			SINGLE,
			LONG
		}noteType;
		int endTime;
	};
	vector<Note>* trackNotes;

	public:
	NoteManager(string filepath);
	virtual ~NoteManager();

	void loadfile(string filepath);
	void loadOsuFile();
	void loadOsuFileInfo(string& retBuffer);
	void loadOsuFileTiming(string& retBuffer);
	void loadOsuFileNote(string& retBuffer);
};