//
//  eyeBallDancer.cpp
//  example-ramMotionExtractor
//
//  Created by ycam on 2015/01/24.
//
//

#include "eyeBallDancer.h"
eyeBallDancer::eyeBallDancer(){
    
    mDrawLines		= false;
    mDrawTriangle	= false;
    mDrawPreview	= true;
    mDrawDump		= true;
    
    sender.setup("192.168.20.73", 8528);
    sender2.setup("192.168.20.74", 8528);
    
    //MagPendulum receive
    receiver.addAddress("/dp/cameraUnit");
    ramOscManager::instance().addReceiverTag(&receiver);
}

void eyeBallDancer::setup(){
    //-------------
    //    ofSetFrameRate(60);
    //    ofBackground(0);
    //    ofEnableAlphaBlending();
    //    ofSetVerticalSync(true);
    //
    //    pointBoxSize = 10;
    //    radius = 0.0;
    //    servoX_val = 0;
    //    servoY_val = 0;
    //
    //    //共通
    //    defaultZ_val = 512;
    //
    //    //Eye
    //    //-------------------
    //    //Eye1
    //    camera1_x = -20;
    //    camera1_y = 0;
    //    camera1_z = 350;
    //
    //    //Eye2
    //    camera2_x = 20;
    //    camera2_y = 0;
    //    camera2_z = 350;
    //
    //    //Eye3
    //    camera3_x = 20;
    //    camera3_y = 0;
    //    camera3_z = 350;
    //
    //    //Eye4
    //    camera4_x = 20;
    //    camera4_y = 0;
    //    camera4_z = 350;
    //    //-------------------
    //
    //    //point
    //    posX = 0;
    //    posY = 0;
    //    posZ = 0;
    //
    //    //初期状態
    //    manualControl = true;
    //    reset = true;
    //    sendServo = true;
    //    //-------------
}


void eyeBallDancer::setupControlPanel(){
    ofxUICanvasPlus* gui = ramGetGUI().getCurrentUIContext();
    
    gui->addToggle("Preview"	, &mDrawPreview);
    gui->addToggle("Dump"		, &mDrawDump);
    
    gui->addSpacer();
    gui->addToggle("Lines"		, &mDrawLines);
    gui->addToggle("Triangle"	, &mDrawTriangle);
    
    //--------------
    //Eye1 setup
    gui->addSpacer();
    gui->addSlider("Eye1 X", -ofGetWidth()/2, ofGetWidth()/2, &camera1_x);
    gui->addSlider("Eye1 Y", -ofGetHeight()/2, ofGetHeight()/2, &camera1_y);
    gui->addSlider("Eye1 Z", -512, 512, &camera1_z);
    
    //Eye2 setup
    gui->addSpacer();
    gui->addSlider("Eye2 X", -ofGetWidth()/2, ofGetWidth()/2, &camera2_x);
    gui->addSlider("Eye2 Y", -ofGetHeight()/2, ofGetHeight()/2, &camera2_y);
    gui->addSlider("Eye2 Z", -512, 512, &camera2_z);
    
    //Eye3 setup
    gui->addSpacer();
    gui->addSlider("Eye3 X", -ofGetWidth()/2, ofGetWidth()/2, &camera3_x);
    gui->addSlider("Eye3 Y", -ofGetHeight()/2, ofGetHeight()/2, &camera3_y);
    gui->addSlider("Eye3 Z", -512, 512, &camera3_z);
    
    //Eye4 setup
    gui->addSpacer();
    gui->addSlider("Eye4 X", -ofGetWidth()/2, ofGetWidth()/2, &camera4_x);
    gui->addSlider("Eye4 Y", -ofGetHeight()/2, ofGetHeight()/2, &camera4_y);
    gui->addSlider("Eye4 Z", -512, 512, &camera4_z);
    //--------------
    
    //Manual Control
    gui->addSpacer();
    gui->addToggle("Manual Control", &manualControl);
    gui->addToggle("Reset", &reset);
    gui->addSlider("Position X", -ofGetWidth()/2, ofGetWidth()/2, &posX);
    gui->addSlider("Position Y", -ofGetHeight()/2, ofGetHeight()/2, &posY);
    gui->addSlider("Position Z", -512, 512, &posZ);
    
    //Auto Turn
    gui->addSpacer();
    gui->addToggle("Auto Turn"		, &autoTurn);
    //スピードの設定
    gui->addSlider("Speed", 0.0, 0.1, &speed);
    //半径の設定
    gui->addSlider("Radius", 0.0, ofGetWidth()/2, &radius);
    
    //Node Control
    gui->addSpacer();
    gui->addToggle("pendulum Contorl", &pendulumContorl);
    
    //Node Control
    gui->addSpacer();
    gui->addToggle("Node Control", &nodeControl);
    
    //send servo
    gui->addSpacer();
    gui->addToggle("Send Servo", &sendServo);
    //--------------
    
    /*=== register ===*/
    motionExtractor.setupControlPanel(this);
    
}

void eyeBallDancer::onEnabled(){
    manualControl = false;
    reset = false;
    nodeControl = false;
    ledVal = 1;
    pointBoxSize = 10;
    
    //sendServo = true;
    //test用
    sendServo = true;
    
    //デフォルトはnode control
    nodeControl = true;
    manualControl = false;
    autoTurn = false;
    speed = 0.03;
    radius = 80.0;
    
    
    //デフォルトのEyeポジション（仮設定）
    //-----------------------
    //eye1
    camera1_x = -150.0;
    camera1_y = 25.0;
    camera1_z = 240.0;
    
    //eye2
    camera2_x = -60.0;
    camera2_y = 25.0;
    camera2_z = 240.0;
    
    //eye3
    camera3_x = 50.0;
    camera3_y = 25.0;
    camera3_z = 240.0;
    
    //eye4
    camera4_x = 140.0;
    camera4_y = 25.0;
    camera4_z = 240.0;
    //-----------------------
    
    
    
    refleshState();
}

void eyeBallDancer::onDisabled(){
    manualControl = true;
    reset = true;
    sendServo = true;
    ledVal = 0;
    
    refleshState();
}

void eyeBallDancer::update(){
    
    /*=== update ===*/
    motionExtractor.update();
    
    //oscを送る数を制限
    if(ofGetFrameNum() % 3 == 0){
        refleshState();
    }
    
}

void eyeBallDancer::refleshState(){
    //Auto Turn
    if(autoTurn == true){
        phase += speed;
        //phase -= speed;
        
    }
    
    //Reset
    //リセットボタンの処理
    if(reset == true){
        posX = 0;
        posY = 25;
        posZ = -240;
    }
    
    //Node control
    if(nodeControl == true){
        manualControl = false;
        autoTurn = false;
        nodeVal = motionExtractor.getNodeAt(0);
        ramNode nodeHipVal = motionExtractor.getNodeAt(1);
        
        //        posX = 3 * int(nodeVal.getGlobalPosition().x - nodeHipVal.getGlobalPosition().x);
        //        posY = 3 * int(nodeVal.getGlobalPosition().y - nodeHipVal.getGlobalPosition().y);
        //        posZ = 3 * int(nodeVal.getGlobalPosition().z - nodeHipVal.getGlobalPosition().z);
        
        posX = 2 * int(nodeVal.getGlobalPosition().x - nodeHipVal.getGlobalPosition().x);
        posY = 2 * int(nodeVal.getGlobalPosition().y - nodeHipVal.getGlobalPosition().y);
        posZ = 2 * int(nodeVal.getGlobalPosition().z - nodeHipVal.getGlobalPosition().z);
    }
    
    //
    if(pendulumContorl == true){
        manualControl = false;
        autoTurn = false;
        nodeControl = false;
        
        posX = ofMap(int(pendulumPosX *200),0,500,140,-512);
        posY = 100;
        posZ = ofMap(int(pendulumPosZ *200),0,500,140,-512);
    }
    
    //servo value
    if(sendServo == true){
        //Eye1 servo
        servoX_val = int(camera1_AngleX);
        servoY_val = int(camera1_AngleY);
        
        //Eye2 servo
        servoX2_val = int(camera2_AngleX);
        servoY2_val = int(camera2_AngleY);
        
        //Eye3 servo
        servoX3_val = int(camera3_AngleX);
        servoY3_val = int(camera3_AngleY);
        
        //Eye4 servo
        servoX4_val = int(camera4_AngleX);
        servoY4_val = int(camera4_AngleY);
    }
    //-------------
    
    //=== OSC Read =======
    // check for waiting messages
    while(receiver.hasWaitingMessages()){
        // get the next message
        ofxOscMessage r;
        receiver.getNextMessage(&r);
        
        // check for mouse moved message
        if(r.getAddress() == "/dp/cameraUnit/MagPendulum/features"){
            pendulumPosX = r.getArgAsFloat(1);
            pendulumPosZ = r.getArgAsFloat(2);
            //cout << "receive!!!" << endl;
            cout << "x  :" << int(pendulumPosX *200) << endl;
            //cout << "x  :" << ofMap(int(pendulumPosX *200),0,500,140,-512) << endl;
            //cout << "z  :" << ofMap(int(pendulumPosZ *200),0,500,140,-512) << endl;
        }
    }
    //=== OSC Read =======
    
    /*=== OSC Send Example ===*/
    
    //1台目のArduino
    ofxOscMessage m;
    m.setAddress("/dp/hakoniwa/eyeBall");
    //m.addFloatArg(motionExtractor.getVelocitySpeedAt(0)); //send node's speed
    m.addIntArg(servoX_val);
    m.addIntArg(servoY_val);
    m.addIntArg(servoY2_val);
    m.addIntArg(servoX2_val);
    m.addIntArg(ledVal);
    sender.sendMessage(m);
    
    //２台目のArduino
    ofxOscMessage n;
    n.setAddress("/dp/hakoniwa/eyeBall2");
    n.addIntArg(servoX3_val);
    n.addIntArg(servoY3_val);
    n.addIntArg(servoX4_val);
    n.addIntArg(servoY4_val);
    n.addIntArg(ledVal);
    sender2.sendMessage(n);
}


void eyeBallDancer::draw(){
    
    //----------------
    //塗りつぶし無し
    ofNoFill();
    ramBeginCamera();
    
    //====== eyeBall ===============================
    //eyeCamera Contorol
    //Eye1
    //----------------------------------------------
    //Eye1の色を指定
    ofSetColor(255, 0, 0);
    //Eye1 横運動のための計算
    //eye1のZ座標がポイントのZ座標よりも大きい時
    //eye1のX座標がポイントのX座標よりも小さい時
    //eye1とポイントの底辺の長さ（AC）
    float ac1 = camera1_z - posZ;
    //eye1とポイントの高さ（BC）
    float bc1 = camera1_x - posX;
    //アークタンジェント
    float triTheta1 = atan(bc1/ac1);
    //ラジアンを角度に変換
    float angleX = (180 * triTheta1) / PI;
    //サーボモーターに角度を送る
    camera1_AngleX = ofMap(angleX, 90, -90, 180, 0);
    
    //Eye1 縦運動のための計算
    float ac2 = camera1_z - posZ;
    float bc2 = camera1_y - posY;
    float triTheta2 = atan(bc2/ac2);
    float angleY = (180 * triTheta2) / PI;
    //if(ac2 > 0){
    camera1_AngleY = ofMap(angleY, 90, -90, 0, 180);
    //}else{
    //camera1_AngleY = ofMap(angleY, 90, -90, 180, 0);
    //}
    //Eye1を描画
    ofDrawBitmapString("eye1", camera1_x + 15, camera1_y, camera1_z);
    ofRect(camera1_x, camera1_y, camera1_z, pointBoxSize, pointBoxSize);
    //----------------------------------------------
    
    //Eye2
    //----------------------------------------------
    //Eye2の色を指定
    ofSetColor(0, 255, 0);
    
    //Eye2 横運動のための計算
    float ac3 = camera2_z - posZ;
    float bc3 = camera2_x - posX;
    float triTheta3 = atan(bc3/ac3);
    float angleX2 = (180 * triTheta3) / PI;
    camera2_AngleX = ofMap(angleX2, 90, -90, 180, 0);
    
    //Eye2 縦運動のための計算
    float ac4 = camera2_z - posZ;
    float bc4 = camera2_y - posY;
    float triTheta4 = atan(bc4/ac4);
    float angleY2 = (180 * triTheta4) / PI;
    //if(ac4 > 0){
    camera2_AngleY = ofMap(angleY2, 90, -90, 0, 180);
    //}else{
    //camera2_AngleY = ofMap(angleY2, 90, -90, 180, 0);
    //}
    
    //Eye2を描画
    ofDrawBitmapString("eye2", camera2_x + 15, camera2_y, camera2_z);
    ofRect(camera2_x, camera2_y, camera2_z, pointBoxSize, pointBoxSize);
    //----------------------------------------------
    
    
    //Eye3
    //----------------------------------------------
    //Eye3の色を指定
    ofSetColor(0, 255, 255);
    
    //Eye3 横運動のための計算
    float ac5 = camera3_z - posZ;
    float bc5 = camera3_x - posX;
    float triTheta5 = atan(bc5/ac5);
    float angleX3 = (180 * triTheta5) / PI;
    camera3_AngleX = ofMap(angleX3, 90, -90, 180, 0);
    
    //Eye3 縦運動のための計算
    float ac6 = camera3_z - posZ;
    float bc6 = camera3_y - posY;
    float triTheta6 = atan(bc6/ac6);
    float angleY3 = (180 * triTheta6) / PI;
    //if(ac6 > 0){
    camera3_AngleY = ofMap(angleY3, 90, -90, 0, 180);
    //}else{
    //camera3_AngleY = ofMap(angleY3, 90, -90, 180, 0);
    //}
    
    //Eye3を描画
    ofDrawBitmapString("eye3", camera3_x + 15, camera3_y, camera3_z);
    ofRect(camera3_x, camera3_y, camera3_z, pointBoxSize, pointBoxSize);
    //----------------------------------------------
    
    //Eye4
    //----------------------------------------------
    //Eye4の色を指定
    ofSetColor(255, 255, 255);
    
    //Eye4 横運動のための計算
    float ac7 = camera4_z - posZ;
    float bc7 = camera4_x - posX;
    float triTheta7 = atan(bc7/ac7);
    float angleX4 = (180 * triTheta7) / PI;
    camera4_AngleX = ofMap(angleX4, 90, -90, 180, 0);
    
    //Eye4 縦運動のための計算
    float ac8 = camera4_z - posZ;
    float bc8 = camera4_y - posY;
    float triTheta8 = atan(bc8/ac8);
    float angleY4 = (180 * triTheta8) / PI;
    //if(ac8 > 0){
    camera4_AngleY = ofMap(angleY4, 90, -90, 0, 180);
    //}else{
    //camera4_AngleY = ofMap(angleY4, 90, -90, 180, 0);
    //}
    
    //Eye4を描画
    ofDrawBitmapString("eye4", camera4_x + 15, camera4_y, camera4_z);
    ofRect(camera4_x, camera4_y, camera4_z, pointBoxSize, pointBoxSize);
    //----------------------------------------------
    
    //==============================================
    
    
    
    //----------------------------------------------
    
    //Line
    //----------------------------------------------
    //    //ラインの色を指定
    //    ofSetColor(255, 255, 255);
    //    ofLine(camera1_x, camera1_y, camera1_z, posX, posY, posZ);
    //----------------------------------------------
    
    //Point
    //----------------------------------------------
    //視点の色を指定
    ofSetColor(0, 255, 255);
    //pointの描画
    ofDrawBitmapString("point", posX + 15, posY, posZ);
    ofRect(posX, posY, posZ, pointBoxSize, pointBoxSize);
    
    //Manual Control
    if(manualControl == true){
        autoTurn = false;
    }
    
    //Auto Turn
    if(autoTurn == true){
        ofNoFill();
        //ofPoint pos;
        
        //円の座標を三角関数を利用して計算
        //pos.x = cos(phase) * radius;
        //pos.y = sin(phase) * radius;
        
        //test
        pos.x = cos(phase) * radius;
        pos.y = sin(phase) * radius;
        pos.z = cos(phase) * radius;
        
        manualControl = false;
        posX = pos.x;
        posY = pos.y;
        //posZ = 0;
        posZ = pos.z;
    }
    //----------------------------------------------
    
    //test
    //----------------------------------------------
    //    cout << "angle 1     : " << servoX_val << endl;
    //    cout << "angle 1     : " << servoY_val << endl;
    //    cout << "angle 3     : " << servoX3_val << endl;
    //    cout << "angle 3     : " << servoY3_val << endl;
    //    cout << "node       : " << motionExtractor.getNodeAt(0) << endl;
    //    cout << "ac1    : " << int(ac1) << endl;
    //    cout << "bc1    : " << int(bc1) << endl;
    //    cout << "pointX : " << int(pos.x) << endl;
    //    cout << "pointY : " << int(pos.y) << endl;
    //    cout << "pointZ : " << int(pos.z) << endl;
    //----------------------------------------------
    
    //====== eyeBall ===============================
    
    /*=== Preview selected nodes ===*/
    
    if (mDrawPreview)	motionExtractor.draw();
    
    
    /*=== Example drawing with motionExtractor ===*/
    
    if (mDrawLines)		example_drawLines();
    if (mDrawTriangle)	example_drawTriangles();
    
    ramEndCamera();
    
    if (mDrawDump)		example_drawDump();
}

void eyeBallDancer::example_drawLines(){
    
    ofSetColor(255);
    for (int i = 0;i < motionExtractor.getNumPort() - 1;i+=2){
        
        ofVec3f vec_a = motionExtractor.getPositionAt(i);
        ofVec3f vec_b = motionExtractor.getPositionAt(i+1);
        
        ofLine(vec_a, vec_b);
    }
}

void eyeBallDancer::example_drawTriangles(){
    
    ofVec3f vec_a = motionExtractor.getPositionAt(0);
    ofVec3f vec_b = motionExtractor.getPositionAt(1);
    ofVec3f vec_c = motionExtractor.getPositionAt(2);
    
    ofSetLineWidth(3.0);
    ofNoFill();
    ofTriangle(vec_a, vec_b, vec_c);
    ofFill();
    ofSetLineWidth(1.0);
    
}

void eyeBallDancer::example_drawDump(){
    
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