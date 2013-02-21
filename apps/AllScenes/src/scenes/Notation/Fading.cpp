#include "Fading.h"

bool DeadFunctor::operator()(const Fading& fading) {
	return fading.isDead(lifespan);
}

Fading::Fading() {
	birth = ofGetElapsedTimef();
}
float Fading::getLife(float lifespan) const {
	return 1. - ((ofGetElapsedTimef() - birth) / lifespan);
}
bool Fading::isDead(float lifespan) const {
	return getLife(lifespan) < 0;
}