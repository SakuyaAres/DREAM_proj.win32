#pragma once
#include <string>
#include <fstream>
#include <vector>

using namespace std;

enum NoteType
{
	FallingSingle,
	FallingLong
};

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
	};
	Timing* timingLines;
	
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

	struct FallingTrack
	{
		int trackIndex;
		Note* notes;
	};

	public:
	NoteManager(string filepath);
	virtual ~NoteManager();

	void loadfile(string filepath);
	void loadOsuFile();
	void loadOsuFileInfo(string& retBuffer);
};