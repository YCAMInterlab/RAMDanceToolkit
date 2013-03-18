#include "ramBaseScene.h"



ramBaseScene::ramBaseScene() {}
ramBaseScene::~ramBaseScene() {}


void ramBaseScene::setupControlPanel() {}
void ramBaseScene::setup() {}
void ramBaseScene::update() {}
void ramBaseScene::draw() {}
void ramBaseScene::drawActor(const ramActor &actor) {}
void ramBaseScene::drawRigid(const ramRigidBody &rigid) {}
void ramBaseScene::drawHUD() {}
void ramBaseScene::exit() {}


// nodeArray events
void ramBaseScene::onActorSetup(const ramActor &actor) {}
void ramBaseScene::onActorExit(const ramActor &actor) {}
void ramBaseScene::onRigidSetup(const ramRigidBody &rigid) {}
void ramBaseScene::onRigidExit(const ramRigidBody &rigid) {}



ramBaseScene* ramBaseScene::getPtr()
{
	return this;
}


