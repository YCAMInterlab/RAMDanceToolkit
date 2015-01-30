//
//  DanceMap.h
//  RAMDanceToolkit
//
//  Created by kezzardrix2 on 2014/12/23.
//
//

#ifndef RAMDanceToolkit_DanceMap_h
#define RAMDanceToolkit_DanceMap_h

#include "SlidePoint.h"
#include "ofxQuadWarp.h"

class MapLine{
public:
    
    MapLine(ofVec2f v1,ofVec2f v2){
        setup(v1,v2);
    }
    
    void setup(ofVec2f v1,ofVec2f v2){
        mOrigin[0].set(v1);
        mOrigin[1].set(v2);
        
        mVerts[0].imSet(v2);
        mVerts[1].imSet(v2);
        
        mAlpha.imSet(255);
        
        mUnique = ofRandom(0,10000);
        
       // randomizeAlpha();
    }
    
    void setAlpha(float alpha){
        mAlpha.set(alpha);
    }
    
    void setMode(int mode){
        if(mode == 0){
            mVerts[0].set(mOrigin[0]);
            mVerts[1].set(mOrigin[1]);
        }else if(mode == 1){
            mVerts[0].set(mOrigin[0]);
            mVerts[1].set(mOrigin[0]);
        }else if(mode == 2){
            mVerts[0].set(mOrigin[1]);
            mVerts[1].set(mOrigin[1]);
        }
    }
    
    void randomizeMode(){
        setMode((int)ofRandom(0,3));
    }
    
    void randomizeAlpha(){
        if(ofNoise(mVerts[0].x * mVerts[1].y * 0.000001, mVerts[0].y * mVerts[1].x * 0.000001, mUnique + ofGetElapsedTimef() * 0.0001) > 0.5)mAlpha.set(0);
        else mAlpha.set(255);
    }
    
    void update(){
        mAlpha.update();
        for(auto &v:mVerts){
            v.update();
        }
    }
    
    void draw(){
        ofPushStyle();
        ofSetColor(255,255,255,mAlpha.val);
        ofLine(mVerts[0].x,mVerts[0].y,
               mVerts[1].x,mVerts[1].y);
        ofPopStyle();
    }
private:
    Slide mAlpha;
    static const int VERTEX_NUM = 2;
    ofPoint mOrigin[VERTEX_NUM];
    SlidePoint mVerts[VERTEX_NUM];
    
    float mUnique;
};

class MapTri{
public:
    MapTri(ofVec2f v1,ofVec2f v2,ofPoint v3){
        MapLine a(v1,v2);
        MapLine b(v1,v3);
        MapLine c(v2,v3);
        
        mLines.push_back(a);
        mLines.push_back(b);
        mLines.push_back(c);
    }
    
    void setAlpha(float alpha){
        for(auto &v:mLines){
            v.setAlpha(alpha);
        }
    }
    
    void setMode(int mode){
        
        for(auto &v:mLines){
            v.setMode(mode);
        }
 
    }
    
    void randomizeMode(){
        
        for(auto &v:mLines){
            v.randomizeMode();
        }
        
    }
    
    void update(){
        for(auto &v:mLines){
            v.update();
        }
    }
    void draw(){
        for(auto &v:mLines){
            v.draw();
        }
    }
private:
    vector<MapLine>mLines;
};

class DanceMap{
public:
    
    void setup(ofVec2f origin,ofVec2f size){
        
      //  mLines.clear();
        mTris.clear();
        
        mOrigin = origin;
        mSize = size;
        
        for(int i = 0; i < ROW_NUM; i++){
            for(int j = 0; j < COL_NUM; j++){
                 mVerts[j * ROW_NUM + i].set(mOrigin.x + mSize.x * 0.5 * i,
                                             mOrigin.y + mSize.y * 0.5 * j);
            }
        }
        
        /*mLines.push_back(MapLine(mVerts[1 * ROW_NUM + 1],mVerts[0 * ROW_NUM + 0]));
        mLines.push_back(MapLine(mVerts[1 * ROW_NUM + 1],mVerts[1 * ROW_NUM + 0]));
        mLines.push_back(MapLine(mVerts[1 * ROW_NUM + 1],mVerts[2 * ROW_NUM + 0]));
        mLines.push_back(MapLine(mVerts[1 * ROW_NUM + 1],mVerts[0 * ROW_NUM + 1]));
        mLines.push_back(MapLine(mVerts[1 * ROW_NUM + 1],mVerts[0 * ROW_NUM + 2]));
        mLines.push_back(MapLine(mVerts[1 * ROW_NUM + 1],mVerts[1 * ROW_NUM + 2]));
        mLines.push_back(MapLine(mVerts[1 * ROW_NUM + 1],mVerts[2 * ROW_NUM + 2]));
        mLines.push_back(MapLine(mVerts[1 * ROW_NUM + 1],mVerts[2 * ROW_NUM + 1]));
        
        mLines.push_back(MapLine(mVerts[0 * ROW_NUM + 0],mVerts[0 * ROW_NUM + 1]));
        mLines.push_back(MapLine(mVerts[0 * ROW_NUM + 0],mVerts[1 * ROW_NUM + 0]));
        
        mLines.push_back(MapLine(mVerts[0 * ROW_NUM + 2],mVerts[0 * ROW_NUM + 1]));
        mLines.push_back(MapLine(mVerts[0 * ROW_NUM + 2],mVerts[1 * ROW_NUM + 2]));
        
        mLines.push_back(MapLine(mVerts[2 * ROW_NUM + 0],mVerts[1 * ROW_NUM + 0]));
        mLines.push_back(MapLine(mVerts[2 * ROW_NUM + 0],mVerts[2 * ROW_NUM + 1]));
        
        mLines.push_back(MapLine(mVerts[2 * ROW_NUM + 2],mVerts[1 * ROW_NUM + 2]));
        mLines.push_back(MapLine(mVerts[2 * ROW_NUM + 2],mVerts[2 * ROW_NUM + 1]));*/
        
        mTris.push_back(MapTri(mVerts[0 * ROW_NUM + 0],mVerts[0 * ROW_NUM + 1],mVerts[1 * ROW_NUM + 1]));
        mTris.push_back(MapTri(mVerts[0 * ROW_NUM + 0],mVerts[1 * ROW_NUM + 1],mVerts[1 * ROW_NUM + 0]));
      
        mTris.push_back(MapTri(mVerts[0 * ROW_NUM + 1],mVerts[0 * ROW_NUM + 2],mVerts[1 * ROW_NUM + 1]));
        mTris.push_back(MapTri(mVerts[1 * ROW_NUM + 1],mVerts[0 * ROW_NUM + 2],mVerts[1 * ROW_NUM + 2]));
        
        mTris.push_back(MapTri(mVerts[1 * ROW_NUM + 0],mVerts[1 * ROW_NUM + 1],mVerts[2 * ROW_NUM + 0]));
        mTris.push_back(MapTri(mVerts[1 * ROW_NUM + 1],mVerts[2 * ROW_NUM + 0],mVerts[2 * ROW_NUM + 1]));
        
        mTris.push_back(MapTri(mVerts[1 * ROW_NUM + 1],mVerts[1 * ROW_NUM + 2],mVerts[2 * ROW_NUM + 2]));
        mTris.push_back(MapTri(mVerts[1 * ROW_NUM + 1],mVerts[2 * ROW_NUM + 1],mVerts[2 * ROW_NUM + 2]));
        
    }
    void update(){
       
        
        for(auto &v:mTris){
            v.update();
        }
    }
    
    /*void setVerts(int mode,float ratio){
        if(mode == 0){
            for(auto &v:mLines){
                v.setMode(0);
            }
        }else if(mode == 1){
            for(auto &v:mLines){
                v.setMode(1);
            }
            
            int num = mLines.size() * ratio;
            
            for(int i = 0; i < num; i++){
                mLines[(int)ofRandom(0,mLines.size())].setMode(0);
            }
        }
    }*/
    
    void setTriVerts(int mode,float ratio){
        if(mode == 0){
            for(auto &v:mTris){
                v.setMode(0);
            }
        }else if(mode == 1 || mode == 2){
            for(auto &v:mTris){
                v.setMode(1);
            }
            
            int num = mTris.size() * ratio;
         
            for(int i = 0; i < num; i++){
                
                int idx = (int)ofRandom(0,mTris.size());
        
               if(mode == 1) mTris[idx].setMode(0);
               else if(mode == 2)mTris[idx].randomizeMode();
            }
        }
    }
    
    
    /*void setAlpha(int mode,int num){
        if(mode == 0){
            for(auto &v:mLines){
                v.setAlpha(255);
            }
        }else if(mode == 1){
            for(auto &v:mLines){
                v.setAlpha(0);
            }
        }else if(mode == 2){
            for(auto &v:mLines){
                v.setAlpha(0);
            }
            
            if(num > mLines.size())num = mLines.size();
            
            for(int i = 0; i < num; i++){
                mLines[(int)ofRandom(0,num)].setAlpha(255);
            }
        }
    }*/
    
    void setTriAlpha(int mode,float ratio){
        if(mode == 0){
            for(auto &v:mTris){
                v.setAlpha(255);
            }
        }else if(mode == 1){
            for(auto &v:mTris){
                v.setAlpha(0);
            }
        }else{
            for(auto &v:mTris){
                v.setAlpha(0);
            }
            
            int num = mTris.size() * ratio;
            
            for(int i = 0; i < num; i++){
                mTris[(int)ofRandom(0,num)].setAlpha(255);
            }
        }
    }
    
    void draw(){
       
        for(auto &v:mTris){
            v.draw();
        }
    }
private:
    
    static const int ROW_NUM = 3;
    static const int COL_NUM = 3;
    
    static const int VERTEX_NUM = ROW_NUM * COL_NUM;
    ofVec2f mVerts[VERTEX_NUM];
    ofPoint mOrigin;
    ofPoint mSize;
    
    vector<MapTri>mTris;
 
};

class DanceMapScene : public ramBaseScene{
public:
    string getName() const{return "DanceMapScene";};
    void setup(){
        
        setupDanceMap();
        mWarper.load("xmls/warperSettings.xml");
        mWarper.setup();
      //  resetWarper();
    }
    
    void resetWarper(){
        mWarper.setSourceRect(ofRectangle(mOffset.x,mOffset.y,FLOOR_WIDTH,FLOOR_HEIGHT));
        mWarper.setTopLeftCornerPosition(mOffset);
        mWarper.setTopRightCornerPosition(ofPoint(FLOOR_WIDTH - mOffset.x,mOffset.y));
        mWarper.setBottomLeftCornerPosition(ofPoint(mOffset.x,FLOOR_HEIGHT - mOffset.y));
        mWarper.setBottomRightCornerPosition(ofPoint(FLOOR_WIDTH - mOffset.x,FLOOR_HEIGHT - mOffset.y));
        mWarper.setup();
    }
    
    void setupDanceMap(){
        mMaps.clear();
        
        int size = FLOOR_WIDTH / (int)mDim;
        
        mOffset = ofPoint((SINGLE_SCREEN_WIDTH - FLOOR_WIDTH) * 0.5,(SINGLE_SCREEN_HEIGHT - FLOOR_HEIGHT) * 0.5);
        
        for(int i = 0; i < (int)mDim; i++){
            for(int j = 0; j < (int)mDim; j++){
                DanceMap tmp;
                mMaps.push_back(tmp);
                mMaps.back().setup(ofPoint(i * size,j * size) + mOffset, ofPoint(size,size));
            }
        }
    }
    
    void setupControlPanel(){
        
        ramGetGUI().addSlider("dim", 1, 20, &mDim);
        ramGetGUI().addSlider("ratio", 0.1, 1.0, &mAppearTriRatio);
        
        
        mAppearTriModeName.push_back("All");
        mAppearTriModeName.push_back("Tri");
        mAppearTriModeName.push_back("WithLine");
        
        ramGetGUI().addRadioGroup("appearMode", mAppearTriModeName, &mAppearTriMode);
        
        mMapModeName.push_back("VERTICAL_CENTER");
        mMapModeName.push_back("HORIZONTAL_CENTER");
        mMapModeName.push_back("VERTICAL");
        mMapModeName.push_back("HORIZONTAL");
        mMapModeName.push_back("DIAGONAL_RIGHT");
        mMapModeName.push_back("DIAGONAL_LEFT");
        mMapModeName.push_back("COMPLEX");
        
        ramGetGUI().addRadioGroup("mapMode", mMapModeName, &mMapMode);
        ramGetGUI().addToggle("isDrawWarper", &isDrawWarper);
        ramGetGUI().addButton("saveWarper");
        
        ofAddListener(ramGetGUI().getCurrentUIContext()->newGUIEvent, this, &DanceMapScene::onPanelChanged);
    }
    
    void update(){
        
        ramSetViewPort(getFirstViewPort());
        
        for(auto &v:mMaps){
            v.update();
        }
        
        mLineAlpha.update();
        
    }
    void draw(){
        
        ofPushMatrix();
        ofMatrix4x4 mat = mWarper.getMatrix();
        
        ofMultMatrix(mat);
      //  ofTranslate(SINGLE_SCREEN_WIDTH, 0);
        
        ofSetColor(255,255,255,mLineAlpha.val);
        
        int size = FLOOR_WIDTH / (int)mDim;
        
        int mode = mMapMode;
        
        if(mode == COMPLEX){
        
            for(auto &v:mMaps){
                v.draw();
            }
        
        }else if(mode == VERTICAL_CENTER){
            
            ofLine(SINGLE_SCREEN_WIDTH * 0.5,mOffset.y,SINGLE_SCREEN_WIDTH * 0.5,SINGLE_SCREEN_HEIGHT - mOffset.y);
            
        }else if(mode == HORIZONTAL_CENTER){
            
            ofLine(mOffset.x,SINGLE_SCREEN_HEIGHT * 0.5,SINGLE_SCREEN_WIDTH - mOffset.x,SINGLE_SCREEN_HEIGHT * 0.5);
            
        }else if(mode == VERTICAL){
            
            for(int i = 0; i < (int)mDim + 1; i++){
                 ofLine(mOffset.x + i * size,mOffset.y,mOffset.x + i * size,SINGLE_SCREEN_HEIGHT - mOffset.y);
            }
            
        }else if(mode == HORIZONTAL){
            
            for(int i = 0; i < (int)mDim + 1; i++){
                ofLine(mOffset.x,mOffset.y + i * size, SINGLE_SCREEN_WIDTH - mOffset.x,mOffset.y + i * size);
            }
            
        }else if(mode == DIAGONAL_LEFT){
            
            ofLine(mOffset.x,mOffset.y,SINGLE_SCREEN_WIDTH - mOffset.x,SINGLE_SCREEN_HEIGHT - mOffset.y);
            
        }else if(mode == DIAGONAL_RIGHT){
            
            ofLine(mOffset.x,SINGLE_SCREEN_HEIGHT - mOffset.y,SINGLE_SCREEN_WIDTH - mOffset.x,mOffset.y);
            
        }
        
        ofPopMatrix();
        
        if(isDrawWarper){
            ofSetColor(ofColor::magenta);
            ofSetLineWidth(3);
            mWarper.drawQuadOutline();
            mWarper.drawCorners();
            mWarper.drawHighlightedCorner();
            mWarper.drawSelectedCorner();
        }
        
    }
    void drawActor(const ramActor& actor){}
    
    void onPanelChanged(ofxUIEventArgs& e)
	{
		string name = e.widget->getName();
  
        if(name == "dim"){
            
            setupDanceMap();
            randomizeTris();
        
        }
        
        if(name == "saveWarper"){
            mWarper.save("xmls/warperSettings.xml");
        }
        
        if(name == "ratio"){
            randomizeTris();
        }
        
        name = e.widget->getParent()->getName();
        
        if(name == "appearMode"){
            randomizeTris();
        }
        
        if(name == "mapMode"){
            
            if((int)mMapMode != COMPLEX){
                
                for(auto &v:mMaps){
                    v.setTriAlpha(1,0);
                }
                
                mLineAlpha.imSet(0);
                mLineAlpha.set(255);
                
            }else{
                for(auto &v:mMaps){
                    v.setTriAlpha(0,1.0);
                }
                randomizeTris();
            }
            
        }
        
        
	}
    
    void randomizeTris(){
     
        for(auto &v:mMaps){
            v.setTriVerts(mAppearTriMode,mAppearTriRatio);
        }
        
    }
    
    ~DanceMapScene(){
  //      mWarper.save("xmls/warperSettings.xml");
    }
    
private:
    static const int FLOOR_WIDTH = 1000;
    static const int FLOOR_HEIGHT = 1000;
    
    float mDim = 6;
    
    vector<DanceMap>mMaps;
    
    enum MAP_MODE{
        VERTICAL_CENTER,
        HORIZONTAL_CENTER,
        VERTICAL,
        HORIZONTAL,
        DIAGONAL_RIGHT,
        DIAGONAL_LEFT,
        COMPLEX
    };
    

    int mMapMode = COMPLEX;
    
    ofPoint mOffset;
    Slide mLineAlpha;
    
    float mAppearTriRatio = 1.0;
    
    int mAppearTriMode = 0;
    
    vector<string>mAppearTriModeName;
    vector<string>mMapModeName;
    
    ofxQuadWarp mWarper;
    
    bool isDrawWarper = false;
};

#endif
