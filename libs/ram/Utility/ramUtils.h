// 
// ramUtils.h - RAMDanceToolkit
// 
// Copyright 2012-2013 YCAM InterLab, Yoshito Onishi, Satoru Higa, Motoi Shimizu, and Kyle McDonald
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//    http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include "ofMain.h"

#ifndef TARGET_WIN32
#include <tr1/unordered_map>
#endif

#include "ofxXmlSettings.h"

#pragma mark - ramFading

class ramFading;

class ramDeadFunctor {
protected:
	float lifespan;
public:
	ramDeadFunctor(float lifespan) : lifespan(lifespan) {}
	bool operator()(const ramFading& fading);
};

class ramFading {
protected:
	float birth;
public:
	ramFading();
	float getLife(float lifespan) const;
	bool isDead(float lifespan) const;
	
	template <class T>
	static void bury(list<T>& all, float lifespan) {
		ramDeadFunctor deadFunctor(lifespan);
		all.erase(remove_if(all.begin(), all.end(), deadFunctor), all.end());
	}
};


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