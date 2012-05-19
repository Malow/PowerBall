#ifndef GAMEOPTIONS
#define GAMEOPTIONS

#include "SoundEngine\SoundEngine.h"
#include "stdafx.h"
#include <fstream>

class GameOptions
{
public:
	static float masterVolume;
	static float songVolume;
	static float effectVolume;
	static SoundSong* songPlaying;
	static SoundEffect* mouseClick;
	static bool isPlaying;

	GameOptions()
	{

	}

	void LoadFromeFile(string file)
	{
		ifstream in;
		in.open(file.c_str());
		if(!in)
		{
			MaloW::Debug("Failed to load GameOptinsParams from " + file + ". Might be because it didn't excist, creating it now.");
			this->SaveToFile(file);
			return;
		}

		string line = "";

		getline(in, line);
		this->masterVolume = atof(line.c_str());
		getline(in, line);
		this->songVolume = atof(line.c_str());
		getline(in, line);
		this->effectVolume = atof(line.c_str());


		in.close();
	}

	void SaveToFile(string file)
	{
		ofstream out;
		out.open(file);
		if(!out)
		{
			MaloW::Debug("Failed to save GameOptinsParams from " + file);
			return;
		}

		out << this->masterVolume << endl;
		out << this->songVolume << endl;
		out << this->effectVolume << endl;

		out.close();
	}


	void SaveToFile(string file, int masterVolume, int songVolume, int effectVolume)
	{
		ofstream out;
		out.open(file);
		if(!out)
		{
			MaloW::Debug("Failed to save GameOptinsParams from " + file);
			return;
		}

		out << masterVolume << endl;
		out << songVolume << endl;
		out << effectVolume << endl;

		out.close();
	}



	virtual ~GameOptions() { }
};

#endif