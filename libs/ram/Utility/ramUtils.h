#pragma once

#include "ofMain.h"

#ifndef TARGET_WIN32
#include <tr1/unordered_map>
#endif

#include "ofxXmlSettings.h"

#pragma mark - ramCompoundContainer

template <typename T>
class ramCompoundContainer
{
#ifndef TARGET_WIN32
	typedef std::tr1::unordered_map<string, T> MapType;
#else
	typedef std::map<string, T> MapType;
#endif
	typedef vector<T*> ArrayType;

public:

	void set(const string &key, const T &o)
	{
		hash[key] = o;
		updateIndexCache();
	}

	void erase(const string &key)
	{
		hash.erase(key);
		updateIndexCache();
	}

	size_t size() { return array.size(); }

	void clear()
	{
		array.clear();
		hash.clear();
		hash_keys.clear();
	}

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
			ofLogError("RAMDanceToolkit::ramCompoundContainer") << "invalid key: " << key;
			assert(false);
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