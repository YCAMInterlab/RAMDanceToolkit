#include "ramUtils.h"

bool ramDeadFunctor::operator()(const ramFading& fading) {
	return fading.isDead(lifespan);
}

ramFading::ramFading() {
	birth = ofGetElapsedTimef();
}
float ramFading::getLife(float lifespan) const {
	return 1. - ((ofGetElapsedTimef() - birth) / lifespan);
}
bool ramFading::isDead(float lifespan) const {
	return getLife(lifespan) < 0;
}