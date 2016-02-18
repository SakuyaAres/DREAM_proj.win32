#include "NoteManager.h"
#include "WStringSupport.h"
#include "StringTools.h"


using namespace std;

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
	getline(file,buffer);
	if (strcmp(buffer.substr(0, 3).c_str(), "osu") == 0)
		loadOsuFile();
}

void NoteManager::loadOsuFile()
{
	string buffer;
	while (!file.eof())
	{
		getline(file, buffer);
		if (buffer[0] == '[')
		{
			string option;
			sscanf(buffer.c_str(), "[%s]", option);
			if (strcmp(option.c_str(), "General") == 0)
			{
				do
				{
					getline(file, buffer);
					buffer.find_first_not_of(' ');
					loadOsuFileGenData();
				} while (buffer[0] != '[');
			}
		}
	}
}

void NoteManager::loadOsuFileGenData()
{
	string buffer;
	getline(file, buffer);
	const char* delim = ":";
	char* line;
}
