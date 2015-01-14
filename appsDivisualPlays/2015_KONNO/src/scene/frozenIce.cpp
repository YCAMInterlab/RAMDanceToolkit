//
//  frozenIce.cpp
//  example-ramMotionExtractor
//
//  Created by ycam on 2015/01/09.
//
//

#include "frozenIce.h"

frozenIce::frozenIce(){
    
    mDrawLines		= false;
    mDrawTriangle	= false;
    mDrawPreview	= true;
    mDrawDump		= true;
    
    sender.setup("192.168.20.55", 8528);    
}

//==========================================================================
void frozenIce::setup(){
    //OSC送信用の値の初期設定
    hantei  = 0;
    fanVal = 0;
    
    //ファンの初期設定
    fanStart = true;
    
    //ペルチェ素子の初期設定
    frozing = false;
    melting = true;
    
    //Manual Control
    manualControl = false;
    
    //Speed Control
    dancerControl = true;
    fixationTime = 500.0;
    speedThreshold = 2.0;
    
    //Distance Control
    distanceControl = false;
    distanceThreshold = 200.0;
}
//==========================================================================


//プログラム非選択時に、DMXを止める
//！！！うまく出来ていないので、後で要確認！！！！
//==========================================================================
void frozenIce::onDisabled(){
//    
//    cout << "ondisebled " << endl;
//    int countExit = 0;
//    countExit++;
//    
//    if(countExit > 10){
//        fanStart = false;
//        manualControl = true;
//        hantei = 0;
//        fanVal = 0;
//        countExit = 0;
//    }
//    cout << "Count Exit : " << countExit << endl;
}
//==========================================================================

void frozenIce::setupControlPanel(){
    
    ofxUICanvasPlus* gui = ramGetGUI().getCurrentUIContext();
    
    gui->addToggle("Preview"	, &mDrawPreview);
    gui->addToggle("Dump"		, &mDrawDump);
    
    //==========================================================================
    gui->addSpacer();
    gui->addToggle("Fan ON/OFF"		, &fanStart);
    
    //Manual Control
    gui->addSpacer();
    gui->addToggle("Manual Control"		, &manualControl);
    gui->addToggle("Melting"		, &melting);
    gui->addToggle("Frozing"		, &frozing);
    
    //automatic control
    gui->addSpacer();
    gui->addSlider("Fixation Time", 0.0, 500.0, &fixationTime);
    gui->addToggle("MeltingFromDancer"		, &iceMelting);
    gui->addToggle("FrozingFromDancer"		, &iceFrozing);
    
    //Dancer Control
    gui->addSpacer();
    gui->addToggle("Speed Control"		, &dancerControl);
    gui->addSlider("Speed Threshold", 0.0, 5.0, &speedThreshold);
    
    //Distance Control
    gui->addSpacer();
    gui->addToggle("Distance Control"		, &distanceControl);
    gui->addSlider("Distance Threshold", 0.0, 200.0, &distanceThreshold);
    //==========================================================================
    
    gui->addSpacer();
    gui->addToggle("Lines"		, &mDrawLines);
    gui->addToggle("Triangle"	, &mDrawTriangle);
    
    /*=== register ===*/
    motionExtractor.setupControlPanel(this);
    
}

void frozenIce::update(){
    
    /*=== update ===*/
    motionExtractor.update();
    
    //oscを送る数を制限
    if(ofGetFrameNum() % 10 == 0){
    
    //Manual Control
    //==========================================================================
    //操作するのはMeltingのトグルのみ
    if(manualControl == true){
        iceFrozing = false;
        iceMelting = false;
    
        if(melting == true){
            frozing = false;
            hantei = 0;
        }else{
            frozing = true;
            hantei = 1;
        }
        
        //他のトグルをOFFにする
        dancerControl = false;
        distanceControl = false;
    }
    //==========================================================================
        
    //Dancer Control
    //==========================================================================
    if(dancerControl == true){
        
        //他のトグルをOFFにする
        manualControl = false;
        distanceControl = false;
        
        //Dancer Control [Frozing]
        //500フレーム以降は、5フレームに1回溶ける（0を送る）
        if(iceFrozing == true){
            frozingCount++;
            if(frozingCount > fixationTime){
                if(frozingCount%5 == 0){
                    hantei = 0;
                }else{
                    hantei = 1;
                }
            }else{
                hantei = 1;
            }
        }else{
            frozingCount = 0;
        }
    
    
        //Dancer Control [Melting]
        //500フレーム以降は、5フレームに1回凍る（1を送る）
        if(iceMelting == true){
            meltingCount++;
            if(meltingCount > fixationTime){
                if(meltingCount%5 == 0){
                    hantei = 1;
                }else{
                    hantei = 0;
                }
            }else{
                hantei = 0;
            }
        }else{
            meltingCount = 0;
        }
    
        //ダンサー(0)が動くと溶ける
        if(motionExtractor.getVelocitySpeedAt(0) > speedThreshold){
            if(iceFrozing == true){
                iceFrozing = false;
            }
            if(iceMelting == false){
                iceMelting = true;
            }
        
        }else{
            if(iceFrozing == false){
                iceFrozing = true;
            }
            if(iceMelting == true){
                iceMelting = false;
            }
        }
    }
    //==========================================================================

    //Distance Control
    //==========================================================================
    if(distanceControl == true){
        
        //他のトグルをOFFにする
        dancerControl = false;
        manualControl = false;
        
        //iceFrozingとiceMeltingの条件分け
        //Distance Control [Frozing]
        //500フレーム以降は、5フレームに1回溶ける（0を送る）
        if(iceFrozing == true){
            frozingCount++;
                if(frozingCount > fixationTime){
                    if(frozingCount%5 == 0){
                        hantei = 0;
                    }else{
                        hantei = 1;
                    }
                }else{
                    hantei = 1;
                }
            }else{
                frozingCount = 0;
            }
        
        //Distance Control [Melting]
        //500フレーム以降は、5フレームに1回凍る（1を送る）
        if(iceMelting == true){
            meltingCount++;
            if(meltingCount > fixationTime){
                if(meltingCount%5 == 0){
                    hantei = 1;
                }else{
                    hantei = 0;
                }
            }else{
                hantei = 0;
            }
        }else{
            meltingCount = 0;
        }
        
        //指定したノード間の距離が[distanceThreshold]より大きいと溶け、小さいと凍る
        if(motionExtractor.getDistanceAt(0, 1) < distanceThreshold){
            if(iceFrozing == true){
                iceFrozing = false;
            }
            if(iceMelting == false){
                iceMelting = true;
            }
        }else{
            if(iceFrozing == false){
                iceFrozing = true;
            }
            if(iceMelting == true){
                iceMelting = false;
            }
        }
    }
    //==========================================================================
        
    //Fan ON/OFF
    //==========================================================================
    if(fanStart == true){
        fanVal = 1;
    }else{
        fanVal = 0;
    }
    //==========================================================================

    
    /*=== OSC Send Example ===*/
    ofxOscMessage m;
    m.setAddress("/dp/hakoniwa/frozenIce");
    //m.addFloatArg(hantei);
    m.addIntArg(fanVal);
    m.addIntArg(hantei);
    sender.sendMessage(m);
    }
}

void frozenIce::draw(){
    
    ramBeginCamera();
    
    /*=== Preview selected nodes ===*/
    
    if (mDrawPreview)	motionExtractor.draw();
    
    
    /*=== Example drawing with motionExtractor ===*/
    
    if (mDrawLines)		example_drawLines();
    if (mDrawTriangle)	example_drawTriangles();
    
    
    ramEndCamera();
    
    if (mDrawDump)		example_drawDump();
    
    //test
    //==========================================================================
    cout << "frozing    : " << frozingCount << ":" << hantei << endl;
    cout << "melting    : " << meltingCount << ":" << hantei << endl;
    cout << "distance    : " << motionExtractor.getDistanceAt(0, 1) << endl;
    //==========================================================================
    
}

void frozenIce::example_drawLines(){
    
    ofSetColor(255);
    for (int i = 0;i < motionExtractor.getNumPort() - 1;i+=2){
        
        ofVec3f vec_a = motionExtractor.getPositionAt(i);
        ofVec3f vec_b = motionExtractor.getPositionAt(i+1);
        
        ofLine(vec_a, vec_b);
    }
    
}

void frozenIce::example_drawTriangles(){
    
    ofVec3f vec_a = motionExtractor.getPositionAt(0);
    ofVec3f vec_b = motionExtractor.getPositionAt(1);
    ofVec3f vec_c = motionExtractor.getPositionAt(2);
    
    ofSetLineWidth(3.0);
    ofNoFill();
    ofTriangle(vec_a, vec_b, vec_c);
    ofFill();
    ofSetLineWidth(1.0);
    
}

void frozenIce::example_drawDump(){
    
    ofPushMatrix();
    ofTranslate(700, 10);
    
    for (int i = 0;i < motionExtractor.getNumPort();i++){
        ofPushMatrix();
        ofTranslate(0, i*90);
        
        ofVec3f vec = motionExtractor.getVelocityAt(i);
        float speed = motionExtractor.getVelocitySpeedAt(i);
        
        ofNoFill();
        ofRect(0, 0, 200, 80);
        ofFill();
        
        string info = "";
        info += "Port  :" + ofToString(i) + "\n";
        info += "Actor :" + motionExtractor.getActorNameAt(i) + "\n";
        info += "Joint :" + motionExtractor.getJointNameAt(i) + "\n";
        info += "Speed :" + ofToString(motionExtractor.getVelocitySpeedAt(i)) + "\n";
        
        ofSetColor(100);
        ofRect(10, 45, motionExtractor.getVelocitySpeedAt(i)*10.0, 15);
        
        ofSetColor(255);
        ofDrawBitmapString(info, 10, 15);
        
        
        ofPopMatrix();
    }
    
    ofPopMatrix();
    
}

void frozenIce::exit(){
}
