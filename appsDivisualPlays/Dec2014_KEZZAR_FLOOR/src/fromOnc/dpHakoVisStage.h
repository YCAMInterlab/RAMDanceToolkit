//
//  dpHakoVisStage.h
//  RAMDanceToolkit
//
//  Created by kezzardrix2 on 2015/01/14.
//
//

#ifndef RAMDanceToolkit_dpHakoVisStage_h
#define RAMDanceToolkit_dpHakoVisStage_h

class dpStageBlob{
public:
    void add(ofPoint pt){
        mPts.push_back(pt);
    }
    
    void setCentroid(ofPoint centroid){
        mCentroid = centroid;
    }
    
    void update(){
        for(auto &v:mPts){
            mCentroid += v;
        }
        
        mCentroid /= mPts.size();
    }
    
    void draw(){
        
        ofPolyline line;
        
        for(auto &v:mPts){
            line.addVertex(v);
        }
        
        line.close();
        
        line.draw();
        
       // ofCircle(mCentroid,mRad);
    }
    
    ofPoint getCentroid(){
        return mCentroid;
    }
    
private:
    vector<ofPoint>mPts;
    ofPoint mCentroid;
    float mRad = 2.0;
};

class dpHakoVisStage : public ramBaseScene{
public:
    
    string getName() const {return "dpVisStage";};
    
    void setupControlPanel(){
        ramGetGUI().addSlider("blobScale",1.0,21000.0,&mBlobScale);
    }
    
    void setup(){
        ramOscManager::instance().addReceiverTag(&mReceiver);
        mReceiver.addAddress("/dp/cameraUnit/stage/contour/blob");
    }
    
    void receiveOsc(){
        
       // mBlobs.clear();
        
        while (mReceiver.hasWaitingMessages()) {
            ofxOscMessage m;
            mReceiver.getNextMessage(&m);
            
            if (m.getAddress() == "/dp/cameraUnit/stage/contour/blob") {
                
                int numBlobs = m.getArgAsInt32(0);
                
                int sumArg= 1;
                
                int blobIdx = 0;
                
                for(int i = 0; i < numBlobs; i++){
                    
                    int nVerts = m.getArgAsInt32(sumArg);
                    
                    dpStageBlob blob;
    
                    for(int j = sumArg + 1; j < sumArg + nVerts; j+=2){
                        float x = m.getArgAsFloat(j);
                        float y = m.getArgAsFloat(j + 1);
                        
                        ofPoint v;
                        v.x = x - 0.5;
                        v.y = y - 0.5;
                        v *= mBlobScale;
                        
                        v.x += SINGLE_SCREEN_WIDTH * 0.5;
                        v.y += SINGLE_SCREEN_HEIGHT * 0.5;
                        
                        blob.add(v);
                        
                       
                    }
                    
                    if(nVerts != 0)mBlobs.push_back(blob);
                    
                    sumArg += nVerts * 2 + 1;
                }
            }
        }
        
    }
    
    void update(){
        
        receiveOsc();
        
        while (mBlobs.size() > BLOB_MAX) {
            mBlobs.pop_front();
        }
        
        for(auto &v:mBlobs){
            v.update();
        }
        
    
    }
    void draw(){
        
        for(auto &v:mBlobs){
            v.draw();
        }
    
        glBegin(GL_LINE_LOOP);
        for(auto &v:mBlobs){
        //    glVertex2f(v.getCentroid().x,v.getCentroid().y);
        }
        glEnd();
        
    }
    
private:
    static const int BLOB_MAX = 300;
    ramOscReceiveTag mReceiver;
    deque<dpStageBlob>mBlobs;
    float mBlobScale = 100.0;
    
};

#endif
