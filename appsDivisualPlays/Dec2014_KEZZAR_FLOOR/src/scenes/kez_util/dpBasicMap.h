//
//  MapGrid.h
//  Ice
//
//  Created by kezzardrix2 on 2014/12/24.
//
//

#ifndef Ice_MapGrid_h
#define Ice_MapGrid_h

class dpBasicMapGrid{
public:
    
    class SimpleTri{
    public:
        SimpleTri(ofPoint v1,ofPoint v2,ofPoint v3){
            mV[0] = v1;
            mV[1] = v2;
            mV[2] = v3;
        }
        
        void draw(){
            ofSetColor(255,255,255);
            
            ofNoFill();
            ofPushMatrix();
            ofTriangle(mV[0],mV[1],mV[2]);
            ofPopMatrix();
        }
        
    private:
        ofPoint mV[3];
    };
   
    void setup(ofPoint origin,ofPoint size){
        mOrigin = origin;
        mTargetSize = size;

        mTris.clear();
        
        addTri(ofPoint(-size.x,-size.y),ofPoint(0,-size.y));
        addTri(ofPoint(-size.x,-size.y), ofPoint(-size.x,0.0));
        
        addTri(ofPoint(0.0,-size.y), ofPoint(size.x,-size.y));
        addTri(ofPoint(size.x,-size.y), ofPoint(size.x,0.0));
        
        addTri(ofPoint(-size.x,0.0), ofPoint(-size.x,size.y));
        addTri(ofPoint(0.0,size.y), ofPoint(-size.x,size.y));
        
        addTri(ofPoint(size.x,0.0), ofPoint(size.x,size.y));
        addTri(ofPoint(0.0,size.y), ofPoint(size.x,size.y));
        
        
    }
    
    void addTri(ofPoint s1,ofPoint s2){
        mTris.push_back(SimpleTri(mOrigin + s1,mOrigin + s2,mOrigin));
    }
    
    void draw(){
 
        for(auto &v:mTris){
           v.draw();
        }
   
    }
    
    void drawLine(ofPoint origin,ofPoint size){
        ofSetColor(255,255,255);
        ofLine(origin,origin + size);
    }
    
private:
    static const int NUM = 8;
    

    ofPoint mOrigin;
    ofPoint mSize;
    ofPoint mTargetSize;
    
    bool hasExtend = false;
    
    bool isDiagonal = false;
    
    vector<SimpleTri>mTris;
    
    bool isDrawAll = true;
};

class dpBasicMap{
private:
    int mDiv = 4;
    vector<dpBasicMapGrid>mLines;
    
    ofFbo mFbo;
    ofShader mMult;
    
    void setupLines(){
        mLines.clear();
        mLines.assign(mDiv * mDiv, dpBasicMapGrid());
        
        int size = SINGLE_SCREEN_WIDTH / mDiv;
        
        for(int i = 0; i < mDiv; i++){
            for(int j = 0; j < mDiv; j++){
                
                mLines[j * mDiv + i].setup(ofPoint(i * size + size * 0.5,j * size + size * 0.5), ofPoint(size * 0.5,size * 0.5));
                
            }
        }
        
        drawToFbo();
    }
    
    void drawToFbo(){
        mFbo.begin();
        ofBackground(0,0,0,0);
        for(auto &v:mLines){
            v.draw();
        }
        mFbo.end();
    }
    
public:
    
    void setup(){
        setupLines();
        mFbo.allocate(SINGLE_SCREEN_WIDTH,SINGLE_SCREEN_HEIGHT,GL_RGB);
        mMult.load("","shaders/mult.frag");
    }
    
    void subdivision(){
        mDiv++;
        setupLines();
    }
    
    void reduction(){
        mDiv--;
        setupLines();
    }
    
    void draw(ofTexture &tex){
        drawToFbo();
        mMult.begin();
        mMult.setUniformTexture("mask", tex, 1);
        mFbo.draw(0,0);
        mMult.end();
        
        
    }
};

#endif
