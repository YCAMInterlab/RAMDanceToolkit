//
//  dpHakoVisStageBlob.h
//  RAMDanceToolkit
//
//  Created by kezzardrix2 on 2015/01/15.
//
//

#ifndef RAMDanceToolkit_dpHakoVisstageBlob_h
#define RAMDanceToolkit_dpHakoVisstageBlob_h

class dpHakoVisStageBlob : public ramBaseScene{
public:
    
    class stageBlob{
        
        static const int PTS_MAX = 20;
        
        struct BlobPt{
            ofPoint circlePt;
            KezSlidePoint pt;
        };
        
        vector<BlobPt>mPts;
        
        private:
            KezSlidePoint mPos;
            ofPoint mSize;
            KezSlide mRad;
           // ofPolyline mLine;
        
            int mLife = 0;
            int mLifeLimit = 30;
        
            bool isAlive = true;
        
        KezSlide mAlpha;
        
        public:
        
            stageBlob(){
                for(int i = 0; i < PTS_MAX; i++){
                    mPts.push_back(BlobPt());
                    
                    float rad = ofRandom(20,70);
                    float theta = ofRandom(0,TWO_PI);
                    float x = cos(theta) * rad;//mRad.val * 0.5;
                    float y = sin(theta) * rad;//mRad.val * 0.5;
                    mPts.back().circlePt.set(x,y);
                    mPts.back().pt.speed = ofRandom(0.05,0.333);
                }
            }
    
            void init(ofPoint pt){
                mPos.imSet(pt);
                mLife = 0;
                for(auto &v:mPts){
                    
                    v.pt.imSet(pt + v.circlePt);
                }
                
                mAlpha.imSet(255);
            }
        
            void addVertex(ofPoint pt){
                mPos.set(pt);
                mLife = 0;
                
                for(auto &v:mPts){
                    v.pt.set(pt + v.circlePt);
                }
                
                mAlpha.imSet(255);
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
                    v.pt += ofPoint(ofRandom(-val,val),
                                 ofRandom(-val,val),
                                 ofRandom(-val,val));
                    v.pt.update();
                }
                
                mAlpha.update();
            }
        
            bool getAlive() const {
                return isAlive;
            }
        
        ofPoint getPos(){
            return mPos;
        }
        
            void draw(ofImage &img){
               // if(isAlive){
                
                   // mLine.draw();
                    ofPushStyle();
                    ofSetColor(dpColor::MAIN_COLOR,mAlpha.val);
                    ofSetRectMode(OF_RECTMODE_CENTER);
                    
                    for(auto &v:mPts){
                        img.draw(v.pt.x,v.pt.y,mRad.val,mRad.val);
                    }
                    
                    img.draw(mPos.x,mPos.y,mRad.val,mRad.val);
                    ofSetColor(255,255,255,mAlpha.val);
                    img.draw(mPos.x,mPos.y,30,30);
                
                    ofPopStyle();
               // }
            }
        
        void fadeOut(){
            mAlpha.set(0);
        }
        
        float getAlpha(){
            return mAlpha.val;
        }
    };
    
    string getName() const {return "dpVisStage";};
    void setupControlPanel(){}
    void setup(){
        ramOscManager::instance().addReceiverTag(&mReceiver);
        mReceiver.addAddress("/dp/cameraUnit/Stage/contour/boundingRect");

        mTex.loadImage("images/gradationCricle.png");
    }
    
    void receiveOsc(){
        
        while(mReceiver.hasWaitingMessages()){
            ofxOscMessage m;
            mReceiver.getNextMessage(&m);
            if(m.getAddress() == "/dp/cameraUnit/Stage/contour/boundingRect"){
                
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
                    
                    size *= 1000.0;
                    mCircles[label].setRad(fmaxf(size.x,size.y));
                }
            }
        }
    }
    
    void update(){
        
        receiveOsc();
        
        map<int, stageBlob>::iterator it = mCircles.begin();
        while (it != mCircles.end()) {
            
            it->second.update();
            
            if (it->second.getAlive() == false) {
                it->second.fadeOut();
            }
        
            ++it;
        }
        
        it = mCircles.begin();
        
        while (it != mCircles.end()) {

            if(it->second.getAlpha() < 1.0){
                mCircles.erase(it++);
            }else ++it;
            
        }
        
    }
    void draw(){
        
        ofDisableDepthTest();
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        
        vector<ofPoint>tmp;
        for(auto &v:mCircles){
            v.second.draw(mTex);
            
            if((int)v.second.getAlpha() == 255){
                tmp.push_back(v.second.getPos());
            }
        }
        
        ofSetLineWidth(3);
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
    map<int,stageBlob>mCircles;
    ramOscReceiveTag mReceiver;
    
    ofImage mTex;
};

#endif
