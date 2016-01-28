//
//  Donuts_ext.cpp
//  RAMDanceToolkit
//
//  Created by YoshitoONISHI on 1/28/16.
//
//

#include "Donuts_ext.h"

void Donuts_ext::setupControlPanel()
{
    
#ifdef RAM_GUI_SYSTEM_OFXUI
    
    ofxUICanvas* panel = ramGetGUI().getCurrentUIContext();
    
    ramGetGUI().addSection("Original me");
    panel->addSlider("Scale", 0.1, 10.0, &mScale, 200.f, 20.f);
    panel->addSlider("Position: X", -300.0, 300.0, &mTranslate.x, 200.f, 20.f);
    panel->addSlider("Position: Z", -300.0, 300.0, &mTranslate.z, 200.f, 20.f);
    
    ramGetGUI().addSection("Duplicated me");
    panel->addSlider("Duplicate", 1, 200, &mNumDuplicate, 200.f, 20.f);
    panel->addSlider("Radius", 1.0, 1000.0, &mRadius, 200.f, 20.f);
    panel->addSlider("Box Size", 1.0, 1000.0, &mBoxSize, 200.f, 20.f);
    
    ramGetGUI().addSection("Reset, Toggle visibility");
    panel->addButton("Reset all settings", false, 30, 30);
    ramGetGUI().addToggle("Show Actor", &mShowActor);
    ramGetGUI().addToggle("Toggle All", &mToggleAll);
    
    for (int i=0; i<ramActor::NUM_JOINTS; i++)
    {
        mNodeVisibility[i] = (i == ramActor::JOINT_RIGHT_HAND || i == ramActor::JOINT_LEFT_HAND);
        mToggles[i] = panel->addToggle(ramActor::getJointName(i), &mNodeVisibility[i], 8, 8);
    }
    
    mMex.setupControlPanel(this);
    
    clear();
    ofAddListener(panel->newGUIEvent, this, &Donuts_ext::onValueChanged);
    
#endif
}


void Donuts_ext::setup()
{
    
}

void Donuts_ext::update()
{
    mMex.update();
    mNumDuplicate = floor(mNumDuplicate);
    mRadian = 2 * M_PI / mNumDuplicate;
}

void Donuts_ext::drawDonuts(const ramNodeArray &nodeArray)
{
    ofPushStyle();
    ofNoFill();
    
    if (mShowActor)
    {
        for (int n=0; n<mNumDuplicate; n++)
        {
            ofColor c1 = ramColor::RED_DEEP;
            ofColor c2 = ramColor::BLUE_LIGHT;
            c1.g += n*3;
            c2.b += n*3;
            
            float x = cos( mRadian * n ) * mRadius;
            float z = sin( mRadian * n ) * mRadius;
            
            ofPushMatrix();
            ofTranslate(x, 0, z);
            if( n==0 ) ofTranslate(mTranslate.x, mTranslate.y, mTranslate.z);
            
            ofRotateY(360/mNumDuplicate * n);
            if( n==0 ) ofScale(mScale, mScale, mScale);
            
            
            ofPushStyle();
            for(int i=0; i<nodeArray.getNumNode(); i++)
            {
                const ramNode& node = nodeArray.getNode(i);
                
                node.beginTransform();
                ofSetColor(c1);
                ofBox( i==ramActor::JOINT_HEAD ? 8 : 5);
                node.endTransform();
                
                if (node.hasParent())
                {
                    ofSetColor(c2);
                    ofLine(node, *node.getParent());
                }
            }
            ofPopStyle();
            
            ofPopMatrix();
        }
    }
    
    else
    {
        for (int index=0; index<nodeArray.getNumNode(); index++)
        {
            if (mNodeVisibility[index] == false) continue;
            
            for (int n=0; n<mNumDuplicate; n++)
            {
                const ramNode &node = nodeArray.getNode(index);
                
                ofPushMatrix();
                {
                    ofColor c1 = ramColor::RED_DEEP;
                    ofColor c2 = ramColor::BLUE_LIGHT;
                    c1.g += n*3;
                    c2.b += n*3;
                    
                    float x = cos( mRadian * n ) * mRadius;
                    float z = sin( mRadian * n ) * mRadius;
                    
                    ofMatrix4x4 m = node.getOrientationQuat();
                    glMultMatrixf(m.getPtr());
                    
                    ofTranslate(x, 0, z);
                    if( n==0 ) ofTranslate(mTranslate.x, mTranslate.y, mTranslate.z);
                    
                    ofRotateY(360/mNumDuplicate * n);
                    if( n==0 ) ofScale(mScale, mScale, mScale);
                    
                    node.beginTransform();
                    ofSetColor(c2);
                    ofBox(mBoxSize);
                    node.endTransform();
                }
                ofPopMatrix();
            }
        }
    }
    
    
    ofPopStyle();
    
}

void Donuts_ext::draw()
{
    
}

void Donuts_ext::drawActor(const ramActor& actor)
{
    bool bActEnable = false;
    for (int q = 0;q < mMex.getNumPort();q++)
        if (mMex.getActorNameAt(q) == actor.getName()) bActEnable = true;
    
    if (!bActEnable) return;
    
    drawDonuts( actor );
}

void Donuts_ext::drawRigid(const ramRigidBody &rigid)
{
    drawDonuts( rigid );
}

void Donuts_ext::onValueChanged(ofxUIEventArgs& e)
{
    string name = e.widget->getName();
    
    if (name == "Toggle All")
    {
        ofxUIToggle *t = (ofxUIToggle *)e.widget;
        setAllVisiblity(t->getValue());
    }
    
    if (name == "Reset all settings")
    {
        clear();
    }
}

void Donuts_ext::clear()
{
    for (int i=0; i<ramActor::NUM_JOINTS; i++)
    {
        mNodeVisibility[i] = (i == ramActor::JOINT_RIGHT_HAND || i == ramActor::JOINT_LEFT_HAND);
        mToggles[i]->setValue(mNodeVisibility[i]);
    }
    
    mNumDuplicate = 20;
    mRadius = 110;
    mScale = 1.0;
    mBoxSize = 3.0;
    mTranslate = ofVec3f::zero();
    mShowActor = false;
}

string Donuts_ext::getName() const { return "Donuts_ext"; }


void Donuts_ext::setAllVisiblity(bool b)
{
    for (int i=0; i<ramActor::NUM_JOINTS; i++)
        mToggles[i]->setValue(b);
}