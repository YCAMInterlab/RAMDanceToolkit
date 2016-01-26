//
//  middleScene.cpp
//  example-MiddleScene
//
//  Created by itotaka on 12/31/15.
//
//

#include "middleScene.h"

void MiddleScene::setupControlPanel()
{
    
    ofxUICanvas* panel = ramGetGUI().getCurrentUIContext();

    bDancer1All = panel->addToggle("1", false, 20, 20);
    panel->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    bHalfAll = panel->addToggle("H", false, 20, 20);
    bDancer2All = panel->addToggle("2", false, 20, 20);
    bExtendAll = panel->addToggle("E_ALL", false,20, 20);
    bUnExtendAll = panel->addToggle("E_OFF", false,20, 20);
    panel->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    
    mEuler.x = 0;
    mEuler.y = 0;
//    mEuler.z = 275;
    mEuler.z = 0;
    
    for (int i=0; i<ramActor::NUM_JOINTS; i++)
    {
        switch (i) {
            case ramActor::JOINT_HIPS:
                bHalfBody = panel->addToggle("H", false, 20, 20);
                panel->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
                bExtendBody = panel->addToggle("E_BODY", false,20, 20);
                sliderBody = panel->addSlider("BODY", -1, 1, 0.0f, 150, 8);
                panel->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
                break;
                
            case ramActor::JOINT_LEFT_HIP:
                bHalfLeftLeg = panel->addToggle("H", false, 20, 20);
                panel->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
                bExtendLeftLeg = panel->addToggle("E_L-LEG", false,20, 20);
                sliderLLeg = panel->addSlider("LEFT_LEG", -1, 1, 0.0f, 150, 8);
                panel->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
                break;
                
            case ramActor::JOINT_RIGHT_HIP:
                bHalfRightLeg = panel->addToggle("H", false, 20, 20);
                panel->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
                bExtendRightLeg = panel->addToggle("E_R-LEG", false,20, 20);
                sliderRLeg = panel->addSlider("RIGHT_LEG", -1, 1, 0.0f, 150, 8);
                panel->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
                break;
                
            case ramActor::JOINT_LEFT_COLLAR:
                bHalfLeftArm = panel->addToggle("H", false, 20, 20);
                panel->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
                bExtendLeftArm = panel->addToggle("E_L-ARM", false,20, 20);
                sliderLArm = panel->addSlider("LEFT_ARM", -1, 1, 0.0f, 150, 8);
                panel->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
                break;

            case ramActor::JOINT_RIGHT_COLLAR:
                bHalfRightArm = panel->addToggle("H", false, 20, 20);
                panel->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
                bExtendRightArm = panel->addToggle("E_R-ARM", false,20, 20);
                sliderRArm = panel->addSlider("RIGHT_ARM", -1, 1, 0.0f, 150,8);
                panel->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
                break;
        }
        
        mHalfToggles[i] = panel->addToggle("", false, 20, 20);
        panel->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
        mSliders[i] = panel->addSlider(ramActor::getJointName(i), -1, 1, 0.0f, 150, 8);
        panel->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
        
    }
    
    for (int i = 0; i < 5; i++){
        save[i] = panel->addToggle("S"+ofToString(i), false);
        panel->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    }
    panel->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);

    for (int i = 0; i < 5; i++){
        load[i] = panel->addToggle("L"+ofToString(i), false);
        panel->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    }
    panel->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);

    cameraX = panel->addSlider("cameraX", -100, 100, 0.0f, 150, 8);
    cameraY = panel->addSlider("cameraY", -100, 100, 77.0f, 150, 8);
    cameraZ = panel->addSlider("cameraZ", -600, 600, 300.0f, 150, 8);
    interGap = panel->addSlider("gap", 0, 800, 200.0f, 150, 8);
    
    panel->addSlider("ANGLE X", 0.0f, 360.0f, &mEuler.x, 300.0f, dim);
    panel->addSlider("ANGLE Y", 0.0f, 360.0f, &mEuler.y, 300.0f, dim);
    panel->addSlider("ANGLE Z", 0.0f, 360.0f, &mEuler.z, 300.0f, dim);

    ofAddListener(ramGetGUI().getCurrentUIContext()->newGUIEvent, this, &MiddleScene::onPanelChanged);
    
//    cout << getCameraManager().getActiveCamera().getGlobalPosition() << endl;
//    cout << getCameraManager().getActiveCamera().getLookAtDir() << endl;
    
}

void MiddleScene::setup()
{
    
}

void MiddleScene::update()
{
    
    ofMatrix4x4 mat;
    mat.rotate(mEuler.z, 0.0f, 0.0f, 1.0f);
    mat.rotate(mEuler.y, 0.0f, 1.0f, 0.0f);
    mat.rotate(mEuler.x, 1.0f, 0.0f, 0.0f);
    
    mRotation = mat.getRotate();

    setSlidersValAll(bDancer1All, -1);
    setSlidersValAll(bDancer2All, 1);
    setSlidersValAll(bHalfAll, 0);
    
    setSlidersVal(bHalfBody, sliderBody, bExtendBody, 0, 5, 0);
    setSlidersVal(bHalfLeftLeg, sliderLLeg, bExtendLeftLeg, 5, 9, 0);
    setSlidersVal(bHalfRightLeg, sliderRLeg, bExtendRightLeg, 9, 13, 0);
    setSlidersVal(bHalfLeftArm, sliderLArm, bExtendLeftArm, 13, 18, 0);
    setSlidersVal(bHalfRightArm, sliderRArm, bExtendRightArm, 18, 23, 0);
    
    getCameraManager().getActiveCamera().setGlobalPosition(cameraX->getValue(), cameraY->getValue(), cameraZ->getValue());
    
}

void MiddleScene::setSlidersVal(ofxUIToggle* toggle, ofxUISlider* groupSlider, ofxUIToggle* extend, int from, int to, float val){

    if (toggle->getValue()) {
        for (int i = from; i < to; i++){
            mSliders[i]->setValue(val * (bExtended[i] ? EXTEND_VAL : 1));
        }
        groupSlider->setValue(val * (extend->getValue() ? EXTEND_VAL : 1));
        toggle->setValue(false);
    }
}

void MiddleScene::setSlidersVal(ofxUISlider* slider, int from, int to){

    for (int i = from; i < to; i++) {
        mSliders[i]->setValue(slider->getValue());
    }
}

void MiddleScene::setSlidersValAll(ofxUIToggle* toggle, float val){
    
    if (toggle->getValue()) {
        for (int i = 0; i < ramActor::NUM_JOINTS; i++){
            mSliders[i]->setValue(val * (bExtended[i] ? EXTEND_VAL : 1));
        }
        
        sliderBody->setValue(val * (bExtendBody ? EXTEND_VAL : 1));
        sliderLLeg->setValue(val * (bExtendLeftLeg ? EXTEND_VAL : 1));
        sliderRLeg->setValue(val * (bExtendRightLeg ? EXTEND_VAL : 1));
        sliderLArm->setValue(val * (bExtendLeftLeg ? EXTEND_VAL : 1));
        sliderRArm->setValue(val * (bExtendRightLeg ? EXTEND_VAL : 1));
        
        toggle->setValue(false);
    }
}

void MiddleScene::draw()
{
    ramBeginCamera();
    ofSetLineWidth(2);
    
    ramActorManager & AM = getActorManager();
    ramNode rn[3][30];
    
    
    for (int i = 0; i < AM.getNumNodeArray(); i++) {
        ramNodeArray & na = AM.getNodeArray(i);

//        ofPushMatrix();
//        ofQuaternion base = na.getNode(ramActor::JOINT_HIPS).getOrientationQuat();
//        ofQuaternion rotated = base * mRotation;
        ofQuaternion rotated = mRotation;
        na.getNode(ramActor::JOINT_HIPS).setOrientation(rotated);
//        ofPopMatrix();

        for (int k = 0; k < na.getNumNode(); k++){
            rn[i][k] = na.getNode(k);
            
            if (rn[i][k].hasParent()){
                ramNode *p = rn[i][k].getParent();
                
                ofLine(p->getGlobalPosition()-na.getNode(0)+ofPoint((interGap->getValue()*2)*i-interGap->getValue(),0,0),
                       (rn[i][k].getGlobalPosition()-na.getNode(0)+ofPoint((interGap->getValue()*2)*i-interGap->getValue(),0,0)));
            }
        }
    }
    
    if (AM.getNumNodeArray() > 1) {
        
        ramNodeArray & na0 = AM.getNodeArray(0);
        ramNodeArray & na1 = AM.getNodeArray(1);

//        ofPushMatrix();
//        ofQuaternion base0 = na0.getNode(ramActor::JOINT_HIPS).getOrientationQuat();
        ofQuaternion rotated0 = mRotation;
        na0.getNode(ramActor::JOINT_HIPS).setOrientation(rotated0);
//        ofPopMatrix();
        
//        ofPushMatrix();
//        ofQuaternion base1 = na1.getNode(ramActor::JOINT_HIPS).getOrientationQuat();
        ofQuaternion rotated1 = mRotation;
        na1.getNode(ramActor::JOINT_HIPS).setOrientation(rotated1);
//        ofPopMatrix();
        
        for (int k = 0; k < na0.getNumNode(); k++) {
            
            ramNode nk0 = na0.getNode(k);
            ramNode nk1 = na1.getNode(k);
            
            if (nk0.hasParent()){
                
                //Parental Dancer1
                ofPoint p00 = nk0.getParent()->getGlobalPosition() - na0.getNode(0);
                int nID00 = nk0.getParent()->getID();
                
                //Kid Dancer1
                ofPoint p01 = nk0.getGlobalPosition() - na0.getNode(0);
                
                //Parental Dancer2
                ofPoint p10 = nk1.getParent()->getGlobalPosition() - na1.getNode(0);
                
                //Kid Dancer2
                ofPoint p11 = nk1.getGlobalPosition() - na1.getNode(0);
                
                // line parental -> kids
                ofLine((p00+((p10-p00)*(0.5+mSliders[nID00]->getValue()*0.5))),(p01+((p11-p01)*(0.5+mSliders[k]->getValue()*0.5))));
//                ofLine((p00+((p10-p00)/2.0f)),(p01+((p11-p01)/2.0f)));
            }
        }
    }
    ramEndCamera();
}

void MiddleScene::onPanelChanged(ofxUIEventArgs &e)
{
    const string name = e.widget->getName();
    
    if (name == "E_ALL") {
        bExtendBody->setValue(true);
        bExtendLeftLeg->setValue(true);
        bExtendRightLeg->setValue(true);
        bExtendLeftArm->setValue(true);
        bExtendRightArm->setValue(true);
        bExtendAll->setValue(false);
        
        changeExtended(bExtendBody, sliderBody, 0, 5);
        changeExtended(bExtendLeftLeg, sliderLLeg, 5, 9);
        changeExtended(bExtendRightLeg, sliderRLeg, 9, 13);
        changeExtended(bExtendLeftArm, sliderLArm, 13, 18);
        changeExtended(bExtendRightArm, sliderRArm, 18, 23);
        
    } else if (name == "E_OFF"){
        bExtendBody->setValue(false);
        bExtendLeftLeg->setValue(false);
        bExtendRightLeg->setValue(false);
        bExtendLeftArm->setValue(false);
        bExtendRightArm->setValue(false);
        bUnExtendAll->setValue(false);
        
        changeExtended(bExtendBody, sliderBody, 0, 5);
        changeExtended(bExtendLeftLeg, sliderLLeg, 5, 9);
        changeExtended(bExtendRightLeg, sliderRLeg, 9, 13);
        changeExtended(bExtendLeftArm, sliderLArm, 13, 18);
        changeExtended(bExtendRightArm, sliderRArm, 18, 23);
    
    } else if (name == "E_BODY") changeExtended(bExtendBody, sliderBody, 0, 5);
    else if (name == "E_L-LEG") changeExtended(bExtendLeftLeg, sliderLLeg, 5, 9);
    else if (name == "E_R-LEG") changeExtended(bExtendRightLeg, sliderRLeg, 9, 13);
    else if (name == "E_L-ARM") changeExtended(bExtendLeftArm, sliderLArm, 13, 18);
    else if (name == "E_R-ARM") changeExtended(bExtendRightArm, sliderRArm, 18, 23);
    else if (name == "BODY") setSlidersVal(sliderBody, 0, 5);
    else if (name == "LEFT_LEG") setSlidersVal(sliderLLeg, 5, 9);
    else if (name == "RIGHT_LEG") setSlidersVal(sliderRLeg, 9, 13);
    else if (name == "LEFT_ARM") setSlidersVal(sliderLArm, 13, 18);
    else if (name == "RIGHT_ARM") setSlidersVal(sliderRArm, 18, 23);
    else if (name == "S0") saveData("middleSceneData0", save[0]);
    else if (name == "S1") saveData("middleSceneData1", save[1]);
    else if (name == "S2") saveData("middleSceneData2", save[2]);
    else if (name == "S3") saveData("middleSceneData3", save[3]);
    else if (name == "S4") saveData("middleSceneData4", save[4]);
    else if (name == "L0") loadData("middleSceneData0", load[0]);
    else if (name == "L1") loadData("middleSceneData1", load[1]);
    else if (name == "L2") loadData("middleSceneData2", load[2]);
    else if (name == "L3") loadData("middleSceneData3", load[3]);
    else if (name == "L4") loadData("middleSceneData4", load[4]);
    
}

void MiddleScene::changeExtended(ofxUIToggle* toggle, ofxUISlider* groupSlider, int from, int to){
    
    for (int i = from; i < to; i++){
        bExtended[i] = toggle->getValue();
        float sliderVal = mSliders[i]->getValue();
        mSliders[i]->setMin(-1 * (bExtended[i] ? EXTEND_VAL : 1));
        mSliders[i]->setMax(1 * (bExtended[i] ? EXTEND_VAL : 1));
        mSliders[i]->setValue(sliderVal * (bExtended[i] ? EXTEND_VAL : 1.0f/EXTEND_VAL));
    }

    groupSlider->setMin(-1 * (toggle->getValue() ? EXTEND_VAL : 1));
    groupSlider->setMax(1 * (toggle->getValue() ? EXTEND_VAL : 1));
    
}

void MiddleScene::saveData(string filename, ofxUIToggle* toggle){
    
    ofxXmlSettings xml;
    
    xml.setValue("EXTEND_BODY", (int)bExtendBody->getValue());
    xml.setValue("EXTEND_LEFT_LEG", (int)bExtendLeftLeg->getValue());
    xml.setValue("EXTEND_RIGHT_LEG", (int)bExtendRightLeg->getValue());
    xml.setValue("EXTEND_LEFT_ARM", (int)bExtendLeftArm->getValue());
    xml.setValue("EXTEND_RIGHT_ARM", (int)bExtendRightArm->getValue());
    
    xml.setValue("SLIDER_BODY", sliderBody->getValue());
    xml.setValue("SLIDER_LEFT_LEG", sliderLLeg->getValue());
    xml.setValue("SLIDER_RIGHT_LEG", sliderRLeg->getValue());
    xml.setValue("SLIDER_LEFT_ARM", sliderLArm->getValue());
    xml.setValue("SLIDER_RIGHT_ARM", sliderRArm->getValue());
    
    for (int i = 0; i < ramActor::NUM_JOINTS; i++) {
        xml.setValue("SLIDER"+ofToString(i), mSliders[i]->getValue());
    }
    
    xml.setValue("cameraX", cameraX->getValue());
    xml.setValue("cameraY", cameraY->getValue());
    xml.setValue("cameraZ", cameraZ->getValue());
    xml.setValue("gap", interGap->getValue());

    xml.save(filename);
    toggle->setValue(false);
    
}

void MiddleScene::loadData(string filename, ofxUIToggle* toggle){

    ofxXmlSettings xml;
    xml.load(filename);
    
    bExtendBody->setValue(xml.getValue("EXTEND_BODY", false) == 1);
    bExtendLeftLeg->setValue(xml.getValue("EXTEND_LEFT_LEG", false) == 1);
    bExtendRightLeg->setValue(xml.getValue("EXTEND_RIGHT_LEG", false) == 1);
    bExtendLeftArm->setValue(xml.getValue("EXTEND_LEFT_ARM", false) == 1);
    bExtendRightArm->setValue(xml.getValue("EXTEND_RIGHT_ARM", false) == 1);
    
    changeExtended(bExtendBody, sliderBody, 0, 5);
    changeExtended(bExtendLeftLeg, sliderLLeg, 5, 9);
    changeExtended(bExtendRightLeg, sliderRLeg, 9, 13);
    changeExtended(bExtendLeftArm, sliderLArm, 13, 18);
    changeExtended(bExtendRightArm, sliderRArm, 18, 23);
    
    sliderBody->setValue(xml.getValue("SLIDER_BODY", 0));
    sliderLLeg->setValue(xml.getValue("SLIDER_LEFT_LEG", 0));
    sliderRLeg->setValue(xml.getValue("SLIDER_RIGHT_LEG", 0));
    sliderLArm->setValue(xml.getValue("SLIDER_LEFT_ARM", 0));
    sliderRArm->setValue(xml.getValue("SLIDER_RIGHT_ARM", 0));

    for (int i = 0; i < ramActor::NUM_JOINTS; i++) {
        mSliders[i]->setValue(xml.getValue("SLIDER"+ofToString(i), 0));
    }

    cameraX->setValue(xml.getValue("cameraX", 0));
    cameraY->setValue(xml.getValue("cameraY", 77));
    cameraZ->setValue(xml.getValue("cameraZ", 300));
    interGap->setValue(xml.getValue("gap", 150));
    
    toggle->setValue(false);
    
}

void MiddleScene::drawActor(const ramActor& actor)
{
    
}

void MiddleScene::drawRigid(const ramRigidBody &rigid)
{
    
}

void MiddleScene::onActorSetup(const ramActor &actor)
{
    
}

void MiddleScene::onActorExit(const ramActor &actor)
{
    
}

void MiddleScene::onRigidSetup(const ramRigidBody &rigid)
{
    
}

void MiddleScene::onRigidExit(const ramRigidBody &rigid)
{
    
}
