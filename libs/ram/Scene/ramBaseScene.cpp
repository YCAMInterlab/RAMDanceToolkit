#include "ramBaseScene.h"



ramBaseScene::ramBaseScene() : bEnabled(false) {}
ramBaseScene::~ramBaseScene() {}


void ramBaseScene::setupControlPanel() {}
void ramBaseScene::setup() {}
void ramBaseScene::update() {}
void ramBaseScene::draw() {}
void ramBaseScene::drawActor(const ramActor &actor) {}
void ramBaseScene::drawRigid(const ramRigidBody &rigid) {}
void ramBaseScene::drawHUD() {}



void ramBaseScene::enable()
{
	bEnabled = true;
}

void ramBaseScene::disable()
{
	bEnabled = false;
}

void ramBaseScene::toggle()
{
	setEnabled(!isEnabled());
}

bool ramBaseScene::isEnabled()
{
	return bEnabled;
}

void ramBaseScene::setEnabled(bool enabled)
{
	if (bEnabled == enabled) return;

	bEnabled = enabled;

	if (enabled)
		onEnabled();
	else
		onDisabled();
}



void ramBaseScene::onEnabled()
{
	cout << "[Scene enabled] " << getName() << endl;
}

void ramBaseScene::onDisabled()
{
	cout << "[Scene disabled] " << getName() << endl;
}



// nodeArray events
void ramBaseScene::onActorSetup(const ramActor &actor) {}
void ramBaseScene::onActorExit(const ramActor &actor) {}
void ramBaseScene::onRigidSetup(const ramRigidBody &rigid) {}
void ramBaseScene::onRigidExit(const ramRigidBody &rigid) {}



ramBaseScene* ramBaseScene::getPtr()
{
	return this;
}


