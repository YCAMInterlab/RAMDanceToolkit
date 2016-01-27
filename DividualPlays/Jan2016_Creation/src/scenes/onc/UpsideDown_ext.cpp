//
//  UpsideDown_ext.cpp
//  RAMDanceToolkit
//
//  Created by YoshitoONISHI on 1/27/16.
//
//

#include "UpsideDown_ext.h"

void UpsideDown_ext::setupControlPanel()
{
#ifdef RAM_GUI_SYSTEM_OFXUI
    
    
    ofxUICanvas* panel = ramGetGUI().getCurrentUIContext();
    
    ofAddListener(panel->newGUIEvent, this, &UpsideDown_ext::onValueChanged);
    
    const float dim = 16.0f;
    const float width = 200.f;
    
    panel->addSlider("ANGLE X", 0.0f, 360.0f, &mEuler.x, width, dim);
    panel->addSlider("ANGLE Y", 0.0f, 360.0f, &mEuler.y, width, dim);
    panel->addSlider("ANGLE Z", 0.0f, 360.0f, &mEuler.z, width, dim);
    
    panel->addSpacer(width, 1.0f);
    
    panel->addToggle("AUTO ROTATE X", &mAutoRotate.x, dim, dim);
    panel->addToggle("AUTO ROTATE Y", &mAutoRotate.y, dim, dim);
    panel->addToggle("AUTO ROTATE Z", &mAutoRotate.z, dim, dim);
    
    panel->addSlider("AUTO ROTATE SPEED X", -5.0f, 5.0f, &mAutoRotateSpeed.x, width, dim);
    panel->addSlider("AUTO ROTATE SPEED Y", -5.0f, 5.0f, &mAutoRotateSpeed.y, width, dim);
    panel->addSlider("AUTO ROTATE SPEED Z", -5.0f, 5.0f, &mAutoRotateSpeed.z, width, dim);
    
    panel->addSpacer(width, 1.0f);
    
    panel->addSlider("OFFSET", -300.0f, 300.0f, &mOffset, width, dim);
    
    panel->addSpacer(width, 1.0f);
    
    panel->addButton("RESET", false, dim, dim);
    
    mMex.setupControlPanel(this);
    
#endif
}

void UpsideDown_ext::setup()
{
    mAutoRotateSpeed.set(1.0f);
    mOffset = -3.0f;
}

void UpsideDown_ext::loopAngle(float &a)
{
    if (a>=360.0f) a=0.0f;
    else if (a<0.0f) a=360.0f;
}


void UpsideDown_ext::update()
{
    mMex.update();
    
    if (mAutoRotate.x) {
        mEuler.x += mAutoRotateSpeed.x;
        loopAngle(mEuler.x);
    }
    if (mAutoRotate.y) {
        mEuler.y += mAutoRotateSpeed.y;
        loopAngle(mEuler.y);
    }
    if (mAutoRotate.z) {
        mEuler.z += mAutoRotateSpeed.z;
        loopAngle(mEuler.z);
    }
    
    ofMatrix4x4 mat;
    mat.rotate(mEuler.z, 0.0f, 0.0f, 1.0f);
    mat.rotate(mEuler.y, 0.0f, 1.0f, 0.0f);
    mat.rotate(mEuler.x, 1.0f, 0.0f, 0.0f);
    
    mRotation = mat.getRotate();
    for (int i=0; i<mUpsideDown.getNumFilters(); i++)
    {
        mUpsideDown.getFilter(i).setOffset(mOffset);
    }
}

void UpsideDown_ext::draw()
{
    vector<ramNodeArray> NAs;
    for (int i=0; i<getNumNodeArray(); i++)
    {
        ramNodeArray tmpActor = getNodeArray(i);
        
        bool bActEnable = false;
        for (int q = 0;q < mMex.getNumPort();q++)
            if (mMex.getActorNameAt(q) == tmpActor.getName()) bActEnable = true;
        
        if (!bActEnable) continue;
        
        ofQuaternion base = tmpActor.getNode(ramActor::JOINT_HIPS).getOrientationQuat();
        ofQuaternion rotated = base * mRotation;
        tmpActor.getNode(ramActor::JOINT_HIPS).setOrientation(rotated);
        
        NAs.push_back(tmpActor);
    }
    
    vector<ramNodeArray> filterdNAs = mUpsideDown.update(NAs);
    
    ramBeginCamera();
    
    ofPushStyle();
    for (int i=0; i<NAs.size(); i++)
    {
        ramNodeArray &NA = filterdNAs[i];
        ofSetColor(ramColor::RED_DEEP);
        ramDrawBasicActor(NA);
    }
    ofPopStyle();
    
    ramEndCamera();
}


void UpsideDown_ext::onValueChanged(ofxUIEventArgs& e)
{
    if (e.widget->getName() == "RESET") {
        mAutoRotate.x = mAutoRotate.y = mAutoRotate.z = 0.0f;
        mEuler.set(0.0f);
        mAutoRotateSpeed.set(1.0f);
        mOffset = -3.0f;
    }
}