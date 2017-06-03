//
// Laban.cpp - RAMDanceToolkit
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

#include "Laban.h"

static bool LabanMomentIsDead(LabanMoment& moment)
{
    return moment.isDead();
}

void Laban::setupControlPanel()
{
	maxLabanMomentLife = 1.0f;
    threshold = .2;
    lineWidth = 3;
    scale = 20;
    ticks = 3;
    lineLength = 300;
    showLines = true;
    showPlanes = false;
    onlyLimbs = true;
    ofxUICanvas* panel = rdtk::GetGUI().getCurrentUIContext();
    panel->addSlider("Fade out", 0, 2, &maxLabanMomentLife, 300, 20);
    panel->addToggle("Only Limbs", &onlyLimbs, 20, 20);
    panel->addSlider("Threshold", 0, .5, &threshold, 300, 20);
    panel->addToggle("Show lines", &showLines, 20, 20);
    panel->addSlider("Line width", 0, 10, &lineWidth, 300, 20);
    panel->addSlider("Line length", 0, 1000, &lineLength, 300, 20);
    panel->addToggle("Show planes", &showPlanes, 20, 20);
    panel->addSlider("Scale", 0, 1000, &scale, 300, 20);
    panel->addSlider("Ticks", 0, 10, &ticks, 300, 20);
}

void Laban::drawImGui()
{
	ImGui::DragFloat("Fade out", &maxLabanMomentLife, 0.1, 0.0, 2.0);
	ImGui::Checkbox("Only Limbs", &onlyLimbs);
	ImGui::DragFloat("Threshold", &threshold, 0.1, 0.0, 0.5);
	ImGui::Checkbox("Show Lines", &showLines);
	ImGui::DragFloat("Line width", &lineWidth, 0.1, 0, 10);
	ImGui::DragFloat("Line Length", &lineLength, 1, 0, 1000);
	ImGui::Checkbox("Show planes", &showPlanes);
	ImGui::DragFloat("Scale", &scale, 1, 0, 3000);
	ImGui::DragFloat("Ticks", &ticks, 1, 0, 10);
}

void Laban::setup()
{
    for(int x = -1; x <= +1; x++)
    {
        for(int y = -1; y <= +1; y++)
        {
            for(int z = -1; z <= +1; z++)
            {
                if(!(x == 0 && y == 0 && z == 0))
                {
                    ofVec3f cur(x, y, z);
                    cur.normalize();
                    labanDirections.push_back(cur);
                    float red = ofMap(x, -1, 1, 64, 255);
                    float green = ofMap(y, -1, 1, 64, 255);
                    float blue = ofMap(z, -1, 1, 64, 255);
                    labanColors.push_back(ofColor(red, green, blue));
                }
            }
        }
    }
}

void Laban::update()
{
    moments.erase(remove_if(moments.begin(), moments.end(), LabanMomentIsDead), moments.end());
}

void Laban::draw()
{
    rdtk::BeginCamera();
    ofEnableAlphaBlending();
    glDisable(GL_DEPTH_TEST);
    list<LabanMoment>::iterator itr;
    for(itr = moments.begin(); itr != moments.end(); itr++)
    {
        LabanMoment& cur = *itr;
        ofPushStyle();
        float alpha = cur.getLife(maxLabanMomentLife);
        ofSetColor(255, 64 * alpha);
        ofDrawLine(cur.start, cur.start + cur.direction * lineLength);
        ofSetColor(labanColors[cur.choice], 255 * alpha);
        ofDrawLine(cur.start, cur.start + labanDirections[cur.choice] * lineLength);
        ofPopStyle();
    }
    rdtk::EndCamera();
}

void Laban::drawActor(const rdtk::Actor &actor)
{
    for (int i=0; i<actor.getNumNode(); i++)
    {
        if(onlyLimbs)
        {
            if(i != rdtk::Actor::JOINT_LEFT_ANKLE &&
               i != rdtk::Actor::JOINT_RIGHT_ANKLE &&
               i != rdtk::Actor::JOINT_LEFT_WRIST &&
               i != rdtk::Actor::JOINT_RIGHT_WRIST)
            {
                continue;
            }
        }
        const rdtk::Node &node = actor.getNode(i);
        ofSetColor(255);
        ofSetLineWidth(lineWidth);
        if(node.hasParent())
        {
            ofVec3f start = node.getGlobalPosition();
            ofVec3f end = node.getParent()->getGlobalPosition();
            ofVec3f direction = (start - end);
            direction.normalize();
            if(showLines)
            {
                int nearestChoice;
                float nearestDistance;
                for(int i = 0; i < labanDirections.size(); i++)
                {
                    float distance = direction.distance(labanDirections[i]);
                    if(i == 0 || distance < nearestDistance)
                    {
                        nearestChoice = i;
                        nearestDistance = distance;
                    }
                }
                if(nearestDistance < threshold)
                {
                    moments.push_back(LabanMoment(start, direction, nearestChoice));
                }
            }
            if(showPlanes)
            {
                if(abs(direction.x) < threshold)
                {
                    ofPushMatrix();
                    ofTranslate(start);
                    ofDrawGrid(scale, ticks, false, true, false, false);
                    ofPopMatrix();
                }
                if(abs(direction.y) < threshold)
                {
                    ofPushMatrix();
                    ofTranslate(start);
                    ofDrawGrid(scale, ticks, false, false, true, false);
                    ofPopMatrix();
                }
                if(abs(direction.z) < threshold)
                {
                    ofPushMatrix();
                    ofTranslate(start);
                    ofDrawGrid(scale, ticks, false, false, false, true);
                    ofPopMatrix();
                }
            }
        }
    }
}

void Laban::drawRigid(rdtk::RigidBody &rigid)
{
}
