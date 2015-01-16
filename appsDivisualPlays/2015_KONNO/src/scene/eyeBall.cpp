//
//  eyeBall.cpp
//  example-ramMotionExtractor
//
//  Created by ycam on 2015/01/11.
//
//

#include "eyeBall.h"
eyeBall::eyeBall(){
    
    mDrawLines		= false;
    mDrawTriangle	= false;
    mDrawPreview	= true;
    mDrawDump		= true;
    
    sender.setup("192.168.20.71", 8528);
}

void eyeBall::setup(){
    //-------------
    ofSetFrameRate(60);
    ofBackground(0);
    ofEnableAlphaBlending();
    ofSetVerticalSync(true);
    
    pointBoxSize = 10;
    radius = 0.0;
    servoX_val = 0;
    servoY_val = 0;
    
    //共通
    defaultZ_val = 512;
    
    //eyeCamera
    camera1_x = 0;
    camera1_y = 0;
    camera1_z = 350;
    
    //point
    posX = 0;
    posY = 0;
    posZ = 0;
    //-------------
}

void eyeBall::setupControlPanel(){
    ofxUICanvasPlus* gui = ramGetGUI().getCurrentUIContext();
    
    gui->addToggle("Preview"	, &mDrawPreview);
    gui->addToggle("Dump"		, &mDrawDump);
    
    gui->addSpacer();
    gui->addToggle("Lines"		, &mDrawLines);
    gui->addToggle("Triangle"	, &mDrawTriangle);
    
    //--------------
    //eyeCamera setup
    gui->addSpacer();
    gui->addSlider("eyeCamera1 X", -ofGetWidth()/2, ofGetWidth()/2, &camera1_x);
    gui->addSlider("eyeCamera1 Y", -ofGetHeight()/2, ofGetHeight()/2, &camera1_y);
    gui->addSlider("eyeCamera1 Z", -512, 512, &camera1_z);
    
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
    gui->addToggle("Node Control", &nodeControl);
    
    //send servo
    gui->addSpacer();
    gui->addToggle("Send Servo", &sendServo);
    //--------------
    
    /*=== register ===*/
    motionExtractor.setupControlPanel(this);
    
}

void eyeBall::update(){
    
    /*=== update ===*/
    motionExtractor.update();
    
    //oscを送る数を制限
    if(ofGetFrameNum() % 3 == 0){

    
    //-------------
    //Auto Turn
    if(autoTurn == true){
        phase += speed;
    }
    
    //Reset
    //リセットボタンの処理
    if(reset == true){
        posX = 0;
        posY = 0;
    }
    
    //Node control
    if(nodeControl == true){
        manualControl = false;
        autoTurn == false;
        nodeVal = motionExtractor.getNodeAt(0);
        
        posX = int(nodeVal[0]);
        posY = int(nodeVal[1]);
        posZ = int(nodeVal[2]);
    }
    
    //servo value
    if(sendServo == true){
        servoX_val = int(camera1_AngleX);
        servoY_val = int(camera1_AngleY);
    }
    //-------------
    
    /*=== OSC Send Example ===*/
    ofxOscMessage m;
    m.setAddress("/dp/hakoniwa/eyeBall");
    //m.addFloatArg(motionExtractor.getVelocitySpeedAt(0)); //send node's speed
    m.addIntArg(servoX_val);
    m.addIntArg(servoY_val);
    sender.sendMessage(m);
    
    }
    
}

void eyeBall::draw(){
    
    //----------------
    //塗りつぶし無し
    ofNoFill();
    ramBeginCamera();
    
    //====== eyeBall ===============================
    //eyeCamera Contorol
    //----------------------------------------------
    //カメラの色を指定
    ofSetColor(255, 0, 0);
    
    //eye 横運動のための計算
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
    
    //eye 縦運動のための計算
    float ac2 = camera1_z - posZ;
    float bc2 = camera1_y - posY;
    float triTheta2 = atan(bc2/ac2);
    float angleY = (180 * triTheta2) / PI;
    if(ac2 > 0){
        camera1_AngleY = ofMap(angleY, 90, -90, 0, 180);
    }else{
        camera1_AngleY = ofMap(angleY, 90, -90, 180, 0);
    }
    //カメラを描画
    ofDrawBitmapString("eye1", camera1_x + 15, camera1_y, camera1_z);
    ofRect(camera1_x, camera1_y, camera1_z, pointBoxSize, pointBoxSize);
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
        pos.x = cos(phase) * radius;
        pos.y = sin(phase) * radius;
        
        manualControl = false;
        posX = pos.x;
        posY = pos.y;
        posZ = 0;
    }
    //----------------------------------------------
    
    //test
    //----------------------------------------------
//    cout << "angle      : " << int(camera1_AngleY) << endl;
//    cout << "s_angle    : " << servoY_val << endl;
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

void eyeBall::example_drawLines(){
    
    ofSetColor(255);
    for (int i = 0;i < motionExtractor.getNumPort() - 1;i+=2){
        
        ofVec3f vec_a = motionExtractor.getPositionAt(i);
        ofVec3f vec_b = motionExtractor.getPositionAt(i+1);
        
        ofLine(vec_a, vec_b);
    }
}

void eyeBall::example_drawTriangles(){
    
    ofVec3f vec_a = motionExtractor.getPositionAt(0);
    ofVec3f vec_b = motionExtractor.getPositionAt(1);
    ofVec3f vec_c = motionExtractor.getPositionAt(2);
    
    ofSetLineWidth(3.0);
    ofNoFill();
    ofTriangle(vec_a, vec_b, vec_c);
    ofFill();
    ofSetLineWidth(1.0);
    
}

void eyeBall::example_drawDump(){
    
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