//
//  HastyChase_ext.cpp
//  RAMDanceToolkit
//
//  Created by YoshitoONISHI on 1/27/16.
//
//

#include "HastyChase_ext.h"

void HastyChase_ext::setupControlPanel()
{
    drawLineToggle = new ofxUIToggle("draw line", &draw_line, 20, 20);
    showBoxToggle = new ofxUIToggle("show box", &show_box, 20, 20);
    fillChaserToggle = new ofxUIToggle("fill chaser", &fill_chaser, 20, 20);
    
    ramGetGUI().addSlider("buffer_time", 1, 10000, &buffer_time);
    ramGetGUI().addSlider("rate", -2, 3, &rate);
    ramGetGUI().addButton("set rate 1");
    
    ramGetGUI().getCurrentUIContext()->addWidgetDown(drawLineToggle);
    ramGetGUI().getCurrentUIContext()->addWidgetDown(showBoxToggle);
    ramGetGUI().getCurrentUIContext()->addWidgetDown(fillChaserToggle);
    ramGetGUI().addColorSelector("chaser color", &joint_color);
    
    mex.setupControlPanel(this);
    
    ofAddListener(ramGetGUI().getCurrentUIContext()->newGUIEvent, this, &HastyChase_ext::onPanelChanged);
}

void HastyChase_ext::onPanelChanged(ofxUIEventArgs& e)
{
    string name = e.widget->getName();
    
    if(name == "set rate 1") {
        rate = 1;
    }
}

void HastyChase_ext::setup()
{
    buffer_time = 3600;
    rate = 1.5;
    show_box = false;
    fill_chaser = false;
    joint_color = ramColor::BLUE_NORMAL;
}

void HastyChase_ext::update()
{
    mex.update();
}

void HastyChase_ext::drawActor(const ramActor& actor)
{
    bool bActEnable = false;
    for (int q = 0;q < mex.getNumPort();q++)
        if (mex.getActorNameAt(q) == actor.getName()) bActEnable = true;
    
    if (!bActEnable) return;
    
    ramTimeShifter &TS = time_shifters[actor.getName()];
    TS.setNumBufferFrame(buffer_time);
    TS.setRate(rate);
    
    ofPushStyle();
    if (fill_chaser)
    {
        ofFill();
    }
    else{
        ofNoFill();
    }
    
    const ramActor &chaser = TS.update(actor);
    
    ofSetColor(joint_color);
    ramDrawBasicActor(chaser);
    
    if (draw_line)
        ramDrawNodeCorresponds(actor, chaser);
    
    if (show_box)
    {
        
        ofSetColor(255, 127);
        ofNoFill();
        
        for (int i = 0; i < chaser.getNumNode(); i++)
        {
            const ramNode &node = chaser.getNode(i);
            ramBox(node, node.getVelocity().length() * 2);
        }
    }
    ofPopStyle();
}

void HastyChase_ext::loadPreset(size_t preset_id)
{
    buffer_time = 3600;
    rate = 1.5;
    draw_line = false;
    show_box = false;
    fill_chaser = false;
    
    if (preset_id == 1)
    {
        draw_line = true;
    }
    
    drawLineToggle->setValue(draw_line);
    drawLineToggle->stateChange();
    showBoxToggle->setValue(show_box);
    showBoxToggle->stateChange();
    fillChaserToggle->setValue(fill_chaser);
    fillChaserToggle->stateChange();
}