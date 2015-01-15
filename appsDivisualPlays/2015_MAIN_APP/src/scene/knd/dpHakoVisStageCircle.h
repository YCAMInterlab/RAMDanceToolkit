//
//  dpHakoVisStageCircle.h
//  RAMDanceToolkit
//
//  Created by kezzardrix2 on 2015/01/15.
//
//

#ifndef RAMDanceToolkit_dpHakoVisStageCircle_h
#define RAMDanceToolkit_dpHakoVisStageCircle_h

class dpHakoVisStageCircle : public ramBaseScene{
public:
    
    class StageCircle{
        
        static const int PTS_MAX = 20;
        vector<KezSlidePoint>mPts;
        
        private:
            KezSlidePoint mPos;
            ofPoint mSize;
            KezSlide mRad = 60;
           // ofPolyline mLine;
        
            int mLife = 0;
            int mLifeLimit = 30;
        
            bool isAlive = true;
        
        public:
        
            StageCircle(){
                for(int i = 0; i < PTS_MAX; i++){
                    mPts.push_back(KezSlidePoint());
                    mPts.back().speed = ofRandom(0.05,0.333);
                }
            }
    
            void init(ofPoint pt){
                mPos.imSet(pt);
                mLife = 0;
                for(auto &v:mPts){
                    v.imSet(pt);
                }
            }
        
            void addVertex(ofPoint pt){
                mPos.set(pt);
                mLife = 0;
                
                for(auto &v:mPts){
                    v.set(pt);
                }
            }
        
        void setRad(float rad){
            mRad.set(rad);
        }
        
            void update(){
                mLife++;
                if(mLife > mLifeLimit)isAlive = false;
                
                mPos.update();
              //  if(isAlive)mLine.addVertex(mPos);
                
                mRad.update();
                
                for(auto &v:mPts){
                    float val = 6.0;
                    v += ofPoint(ofRandom(-val,val),
                                 ofRandom(-val,val),
                                 ofRandom(-val,val));
                    v.update();
                }
            }
        
            bool getAlive() const {
                return isAlive;
            }
        
        ofPoint getPos(){
            return mPos;
        }
        
            void draw(ofImage &img){
                if(isAlive){
                    ofSetColor(255,255,255);
                   // mLine.draw();
                    ofPushStyle();
                    ofSetColor(dpColor::MAIN_COLOR);
                    ofSetRectMode(OF_RECTMODE_CENTER);
                    
                    for(auto &v:mPts){
                        img.draw(v.x,v.y,mRad.val,mRad.val);
                    }
                    
                    img.draw(mPos.x,mPos.y,mRad.val,mRad.val);
                    ofSetColor(255,255,255);
                    img.draw(mPos.x,mPos.y,10,10);
                
                    ofPopStyle();
                }
            }
    };
    
    string getName() const {return "dpVisStage";};
    void setupControlPanel(){}
    void setup(){
        ramOscManager::instance().addReceiverTag(&mReceiver);
        mReceiver.addAddress("/dp/cameraUnit/stage/contour/boundingRect");

        mTex.loadImage("images/gradationCricle.png");
    }
    
    void receiveOsc(){
        
        while(mReceiver.hasWaitingMessages()){
            ofxOscMessage m;
            mReceiver.getNextMessage(&m);
            if(m.getAddress() == "/dp/cameraUnit/stage/contour/boundingRect"){
                
                int num = m.getArgAsInt32(0);
                
                for(int i = 1; i < num * 5 + 1; i+=5){
            
                    int label = m.getArgAsInt32(i);
                    
                    ofPoint size(m.getArgAsFloat(i+3),m.getArgAsFloat(i+4));
                    
                    ofPoint pos((m.getArgAsFloat(i+1) + size.x * 0.5) * SINGLE_SCREEN_HEIGHT + (SINGLE_SCREEN_WIDTH - SINGLE_SCREEN_HEIGHT) * 0.5,
                                (m.getArgAsFloat(i+2) + size.y * 0.5) * SINGLE_SCREEN_HEIGHT);
                    
                    
                    
                    if(mCircles.find(label) == mCircles.end()){
                        
                        mCircles[label].init(pos);
                        
                    }else{
                    
                        mCircles[label].addVertex(pos);
                        
                    }
                    
                    size *= 300.0;
                    mCircles[label].setRad(fmaxf(size.x,size.y));
                }
            }
        }
    }
    
    void update(){
        
        receiveOsc();
        
        map<int, StageCircle>::iterator it = mCircles.begin();
        while (it != mCircles.end()) {
            
            it->second.update();
            
            if (it->second.getAlive() == false) {
                mCircles.erase(it++);
            } else ++it;
        }

        
    }
    void draw(){
        
        ofDisableDepthTest();
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        
        vector<ofPoint>tmp;
        for(auto &v:mCircles){
            v.second.draw(mTex);
            
            if(v.second.getAlive()){
                tmp.push_back(v.second.getPos());
            }
        }
        
        if(tmp.empty() == false){
            ofSetColor(255,255,255);
            glBegin(GL_LINE_LOOP);
            for(int i = 0; i < tmp.size(); i++){
                glVertex2d(tmp[i].x,tmp[i].y);
            }
            glEnd();
        }
        
        
        
    }
private:
    map<int,StageCircle>mCircles;
    ramOscReceiveTag mReceiver;
    
    ofImage mTex;
};

#endif
