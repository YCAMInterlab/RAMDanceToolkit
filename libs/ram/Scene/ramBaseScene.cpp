// 
// ramBaseScene.cpp - RAMDanceToolkit
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


