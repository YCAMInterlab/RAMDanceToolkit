#pragma once

#include "ofMain.h"

#include <tr1/unordered_map>

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




#pragma mark - ramColor

/* Red */
const ofColor ramColorRedNormal = ofColor::fromHex(0xff6666);
const ofColor ramColorRedDeep = ofColor::fromHex(0x993333);
const ofColor ramColorRedLight = ofColor::fromHex(0xff9898);

/* Blue */
const ofColor ramColorBlueNormal = ofColor::fromHex(0x66cc33);
const ofColor ramColorBlueDeep = ofColor::fromHex(0x339900);
const ofColor ramColorBlueLight = ofColor::fromHex(0x99cc99);

/* Green */
const ofColor ramColorGreenNormal = ofColor::fromHex(0x0099cc);
const ofColor ramColorGreenDeep = ofColor::fromHex(0x003366);
const ofColor ramColorGreenLight = ofColor::fromHex(0x99cccc);

/* Yellow */
const ofColor ramColorYellowNormal = ofColor::fromHex(0xffcc00);
const ofColor ramColorYellowDeep = ofColor::fromHex(0xcc9900);
const ofColor ramColorYellowLight = ofColor::fromHex(0xffff00);

/* Black, Gray, White */
const ofColor ramColorBlack = ofColor::fromHex(0x000000);
const ofColor ramColorGray = ofColor::fromHex(0x666666);
const ofColor ramColorWhite = ofColor::fromHex(0xffffff);

const ofColor ramColors[] =
{
	ramColorRedNormal,
	ramColorRedDeep,
	ramColorRedLight,
	ramColorBlueNormal,
	ramColorBlueDeep,
	ramColorBlueLight,
	ramColorGreenNormal,
	ramColorGreenDeep,
	ramColorGreenLight,
	ramColorYellowNormal,
	ramColorYellowDeep,
	ramColorYellowLight,
	ramColorBlack,
	ramColorGray,
	ramColorWhite
};