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

using namespace rdtk;

BaseScene::BaseScene() {}
BaseScene::~BaseScene() {}

void BaseScene::setupControlPanel() {}
void BaseScene::setup() {}
void BaseScene::update() {}
void BaseScene::draw() {}
void BaseScene::drawActor(const Actor &actor) {}
void BaseScene::drawRigid(const RigidBody &rigid) {}
void BaseScene::drawHUD() {}
void BaseScene::exit() {}


// nodeArray events
void BaseScene::onActorSetup(const Actor &actor) {}
void BaseScene::onActorExit(const Actor &actor) {}
void BaseScene::onRigidSetup(const RigidBody &rigid) {}
void BaseScene::onRigidExit(const RigidBody &rigid) {}
