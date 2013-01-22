#pragma once

#include "ofMain.h"
#include <tr1/unordered_map>
#include "ofxXmlSettings.h"


#pragma mark - ramCompoundContainer

template <typename T>
class ramCompoundContainer
{
	typedef std::tr1::unordered_map<string, T> MapType;
	typedef vector<T*> ArrayType;

public:

	void add(const string &key, T &o)
	{
		hash[key] = o;
		updateIndexCache();
	}

	void remove(const string &key)
	{
		hash.erase(key);
		updateIndexCache();
	}

	size_t size() { return array.size(); }

	// array

	T& operator[](size_t index)
	{
		return *array[index];
	}

	// map

	const vector<string>& keys() { return hash_keys; }

	T& operator[](const string& key)
	{
		if (hasKey(key))
		{
			return hash[key];
		}
		else
		{
			ofLogWarning("RAMDanceToolkit") << "invalid key";
			static T ret;
			ret = T();
			return ret;
		}
	}

	bool hasKey(const string& key)
	{
		return hash.find(key) != hash.end();
	}

	void updateIndexCache()
	{
		array.clear();
		hash_keys.clear();
		
		typename MapType::iterator it = hash.begin();
		while (it != hash.end())
		{
			hash_keys.push_back(it->first);
			array.push_back(&it->second);
			it++;
		}
	}

private:

	MapType hash;
	ArrayType array;

	vector<string> hash_keys;

};


#pragma mark - ramCameraSettings
class ramFloor
{
	
public:
	
	enum FloorName
	{
		FLOOR_NONE				= 0,
		FLOOR_PLANE				= 1,
		FLOOR_CHECKER_PATTERN	= 2,
		FLOOR_GRID_LINES		= 3,
		NUM_FLOOR_PATTERNS		= 4
	};
	
	static vector<string> getFloorNames()
	{
		string name[NUM_FLOOR_PATTERNS] =
		{
			"NONE",
			"PLANE",
			"CHECKER_PATTERN",
			"GRID_LINES"
		};
		
		vector<string> floorNames;
		floorNames.clear();
		floorNames.resize(NUM_FLOOR_PATTERNS);
		
		for (int i=0; i<floorNames.size(); i++)
			floorNames.at(i) = name[i];
		
		return floorNames;
	}
};


#pragma mark - ramCameraSettings

class ramCameraSettings
{
public:
	ramCameraSettings(ofxXmlSettings& setting)
	{
		pos = ofVec3f(setting.getValue("x", 0),
					  setting.getValue("y", 0),
					  setting.getValue("z", 0));
		name = setting.getValue("name", "unknown");
	}
	
	
	static vector<ramCameraSettings> getSettings(ofxXmlSettings& setting)
	{
		const int numCams = setting.getNumTags("cam");
		
		vector<ramCameraSettings> settings;
		settings.clear();

		for (int i=0; i<numCams; i++)
		{
			setting.pushTag("cam", i);
			{
				settings.push_back( ramCameraSettings(setting) );
			}
			setting.popTag();
		}
		return settings;
	}
	
	
	static vector<string> getCamNames(ofxXmlSettings& setting)
	{
		const int numCams = setting.getNumTags("cam");
		
		vector<string> names;
		names.clear();
		names.resize(numCams);
		
		for (int i=0; i<numCams; i++)
		{
			setting.pushTag("cam", i);
			{
				names.at(i) = setting.getValue("name", "");
			}
			setting.popTag();
		}
		return names;
	}
	
	static vector<ofVec3f> getCamPositions(ofxXmlSettings& setting)
	{
		const int numCams = setting.getNumTags("cam");
		
		vector<ofVec3f> positions;
		positions.clear();
		positions.resize(numCams);
		
		for (int i=0; i<numCams; i++)
		{
			setting.pushTag("cam", i);
			{
				positions.at(i) = ofVec3f(setting.getValue("x", 0),
										  setting.getValue("y", 0),
										  setting.getValue("z", 0));
			}
			setting.popTag();
		}
		
		if (numCams==0) positions.at(0) = ofVec3f(0,0,0);
		return positions;
	}
	
	ofVec3f pos;
	string name;
};



#pragma mark - ramColor
class ramColor
{
	
public:
	
	enum ColorId
	{
		RED_NORMAL		= 0,
		RED_DEEP		= 1,
		RED_LIGHT		= 2,
		
		GREEN_NORMAL	= 3,
		GREEN_DEEP		= 4,
		GREEN_LIGHT		= 5,
		
		BLUE_NORMAL		= 6,
		BLUE_DEEP		= 7,
		BLUE_LIGHT		= 8,
		
		YELLOW_NORMAL	= 9,
		YELLOW_DEEP		= 10,
		YELLOW_LIGHT	= 11,
		
		BLACK			= 12,
		GRAY			= 13,
		WHITE			= 14,
		
		NUM_COLORS		= 15
	};
};
static ofColor getRamColor(const int colorId)
{
	ofColor color;
	switch (colorId)
	{
		case ramColor::RED_NORMAL:	 color.setHex(0xff6666); break;
		case ramColor::RED_DEEP:	 color.setHex(0x993333); break;
		case ramColor::RED_LIGHT:	 color.setHex(0xff9898); break;
			
		case ramColor::GREEN_NORMAL: color.setHex(0x66cc33); break;
		case ramColor::GREEN_DEEP:	 color.setHex(0x339900); break;
		case ramColor::GREEN_LIGHT:  color.setHex(0x99cc99); break;
			
		case ramColor::BLUE_NORMAL:  color.setHex(0x0099cc); break;
		case ramColor::BLUE_DEEP : 	 color.setHex(0x003366); break;
		case ramColor::BLUE_LIGHT: 	 color.setHex(0x99cccc); break;
			
		case ramColor::YELLOW_NORMAL:color.setHex(0xffcc00); break;
		case ramColor::YELLOW_DEEP:  color.setHex(0xcc9900); break;
		case ramColor::YELLOW_LIGHT: color.setHex(0xffff00); break;
			
		case ramColor::BLACK:		 color.setHex(0x000000); break;
		case ramColor::GRAY:		 color.setHex(0x666666); break;
		case ramColor::WHITE:		 color.setHex(0xffffff); break;
			
		default: color.set(0xff0000); break;
	}
	return color;
}