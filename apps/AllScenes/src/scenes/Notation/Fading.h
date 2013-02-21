#pragma once

#include "ofMain.h"

class Fading;

class DeadFunctor {
protected:
	float lifespan;
public:
	DeadFunctor(float lifespan) : lifespan(lifespan) {}
	bool operator()(const Fading& fading);
};

class Fading {
protected:
	float birth;
public:
	Fading();
	float getLife(float lifespan) const;
	bool isDead(float lifespan) const;
	
	template <class T>
	static void bury(list<T>& all, float lifespan) {
		DeadFunctor deadFunctor(lifespan);
		all.erase(remove_if(all.begin(), all.end(), deadFunctor), all.end());
	}
};

