#pragma once
#include "objects.h"
struct Level {
	float ver = -1;
	std::vector<Object*> objects;
};

// pretty much a class of stolen code!
class LevelHelpers {
public:
	// from https://github.com/Load-Ma/cppXor/blob/master/cppXor/cppXor.cpp
	static std::string xorstr(const char* msg, const char* key) {
		std::string result;
		for (int i = 0; i < strlen(msg); i++)
			result.push_back(msg[i] ^ key[i % sizeof(key)]);
		return result;
	}
	// https://stackoverflow.com/a/46931770
	static std::vector<std::string> split(std::string s, std::string delimiter) {
		size_t pos_start = 0, pos_end, delim_len = delimiter.length();
		std::string token;
		std::vector<std::string> res;

		while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
			token = s.substr(pos_start, pos_end - pos_start);
			pos_start = pos_end + delim_len;
			res.push_back(token);
		}

		res.push_back(s.substr(pos_start));
		return res;
	}
};

// change this if you want, but it's visible in strings anyways so it legit doesn't matter.
// this is just for funnyness points
const char* hilarious = "420funnynumberhaha";

class LevelParser {
public:

	static Level parseLevel(std::string path)
	{
		std::ifstream stream;
		stream.open(path);
		if (!stream.good())
			return { -999, {}};
		std::string l;
		int ind = 0;
		Level output;
		bool readingObjects = false;
		Sprite* spr = nullptr;
		Text* text = nullptr;

		int specialTag = 0;
		int x = 0;
		int y = 0;
		std::string sp1 = "";
		std::string sp2 = "";
		std::string sp3 = "";

		while (std::getline(stream, l))
		{
			if (l.size() < 1)
				continue;
			if (l[0] == '/' && l[1] == '/') // comemnt check
				continue;
			if (!readingObjects)
			{
				switch (ind)
				{
				case 0:
					output.ver = std::stof(l);
					break;
				default:
					readingObjects = true;
					break;
				}
				ind++;
				continue;
			}
			std::string obj = l;
			std::vector<std::string> s = LevelHelpers::split(obj, ":");
			printf("\noutput: %s", obj.c_str());
			for (int i = 0; i < s.size(); i++)
			{
				switch (i)
				{
				case 0: // object type
					specialTag = std::stoi(s[i]);
					break;
				case 1: // x
					x = std::stof(s[i]);
					break;
				case 2: // y
					y = std::stof(s[i]);
					break;
				case 3: // special property #1
					switch (specialTag)
					{
					case 1:
						// text
						sp1 = s[i];
						break;
					case 0:
						// sprite texture
						sp1 = s[i];
						break;
					}
					break;
				case 4: // special property #2
					switch (specialTag)
					{
					case 1:
						// text font
						sp2 = s[i];
						break;
					case 0:
						// sprite atlas
						sp1 = s[i];
						break;
					}
					break;
				case 5: // special property #3
					switch (specialTag)
					{
					case 1:
						// text size
						sp3 = s[i];
						break;
					}
					break;
				}
			}


			switch (specialTag)
			{
			case 0:
				spr = new Sprite(x, y, sp1.size() == 0 ? "" : sp1);
				if (sp1.size() == 0)
				{
					spr->spriteIndex = std::stoi(sp2);
					spr->spriteTexture = nullptr;
				}
				output.objects.push_back(spr);
				break;
			case 1:
				text = new Text(x, y, sp2, std::stoi(sp3), sp1);
				output.objects.push_back(text);
				break;
			}
		}
		stream.close();
		return output;
	}

	static void saveLevel(Level l, std::string savepath)
	{
		std::string levelString = "// generated via level edtior\n" + std::to_string(l.ver);
		for (Object* obj : l.objects)
		{
			std::string line = "";
			line += "\n";
			line += std::to_string(obj->specialTag);
			line += ":";
			line += std::to_string(obj->x);
			line += ":";
			line += std::to_string(obj->y);

			switch (obj->specialTag)
			{
			case 0: // spr
				line += ":";
				if (!((Sprite*)obj)->spriteTexture)
					line += "null:";
				line +=
					((Sprite*)obj)->spritePath.size() == 0
					? std::to_string(((Sprite*)obj)->spriteIndex)
					: ((Sprite*)obj)->spritePath;
				break;
			case 1: // text
				line += ":" + ((Text*)obj)->text 
					+ ":" + ((Text*)obj)->_fontPath 
					+ ":" + std::to_string(((Text*)obj)->_size);
				break;
			}

			levelString += line;
		}

		std::ofstream stream;
		stream.open(savepath);
		stream << levelString;
		stream.close();
	}
};