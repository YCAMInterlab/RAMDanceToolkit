//
//  IceMap.h
//  Ice
//
//  Created by kezzardrix2 on 2014/12/24.
//
//

#ifndef Ice_IceMap_h
#define Ice_IceMap_h

#include "KezSlidePoint.h"

#include "dpIceMapTri.h"

class dpIceMapFace{
public:
    
    void setup(ofPoint origin,ofPoint size){
        mOrigin = origin;
        size *= 0.5;
     
        mTris.clear();
        
        addTri(ofPoint(0,0),ofPoint(-size.x,-size.y),ofPoint(0,-size.y));
        addTri(ofPoint(0,0),ofPoint(-size.x,-size.y), ofPoint(-size.x,0.0));
        
        addTri(ofPoint(0.0,0.0), ofPoint(0.0,-size.y), ofPoint(size.x,-size.y));
        addTri(ofPoint(0.0,0.0), ofPoint(size.x,-size.y), ofPoint(size.x,0.0));
        
        addTri(ofPoint(0.0,0.0), ofPoint(-size.x,0.0), ofPoint(-size.x,size.y));
        addTri(ofPoint(0.0,0.0), ofPoint(0.0,size.y), ofPoint(-size.x,size.y));
        
        addTri(ofPoint(0.0,0.0), ofPoint(size.x,0.0), ofPoint(size.x,size.y));
        addTri(ofPoint(0.0,0.0), ofPoint(0.0,size.y), ofPoint(size.x,size.y));
        
        for(auto &v:mTris){
            v.setAlpha(0);
        }
        
        for(auto &v:mTris){
            const vector<ofVec2f> &tmpV = v.getVerts();
            for(auto &vv:tmpV){
                mVerts.push_back(vv);
            }
            
            const vector<float> &tmpA = v.getAlpha();
            for(auto &vv:tmpA){
                mColors.push_back(ofFloatColor(1,1,1,vv));
            }
        }
        
        mVbo.clear();
        
        mVbo.setVertexData(&mVerts[0], mVerts.size(), GL_DYNAMIC_DRAW);
        mVbo.setColorData(&mColors[0], mColors.size(), GL_DYNAMIC_DRAW);
        
       /* addLine(ofPoint(0,0),ofPoint(-size.x,-size.y));
        addLine(ofPoint(0,0),ofPoint(-size.x,0.0));
        addLine(ofPoint(0,0),ofPoint(-size.x,size.y));
        addLine(ofPoint(0,0),ofPoint(0.0,-size.y));
        addLine(ofPoint(0,0),ofPoint(size.x,-size.y));
        addLine(ofPoint(0,0),ofPoint(size.x,0.0));
        addLine(ofPoint(0,0),ofPoint(size.x,size.y));
        addLine(ofPoint(0,0),ofPoint(0.0,size.y));
        
        addLine(ofPoint(0,-size.y),ofPoint(-size.x,-size.y));
        addLine(ofPoint(0,-size.y),ofPoint(size.x,-size.y));
        addLine(ofPoint(-size.x,0),ofPoint(-size.x,-size.y));
        addLine(ofPoint(-size.x,0),ofPoint(-size.x,size.y));
        addLine(ofPoint(size.x,0.0),ofPoint(size.x,-size.y));
        addLine(ofPoint(size.x,0.0),ofPoint(size.x,size.y));
        addLine(ofPoint(0.0,size.y),ofPoint(size.x,size.y));
        addLine(ofPoint(-size.x,size.y),ofPoint(0.0,size.y));
        
        for(auto &v:mLines){
            v.setAlpha(0);
        }
        */
    }
    
    /*void addLine(ofPoint s1,ofPoint s2){
        mLines.push_back(MapLine(mOrigin + s1,mOrigin + s2));
    }*/
    
    void addTri(ofPoint s1,ofPoint s2,ofPoint s3){
        mTris.push_back(dpIceMapTri(mOrigin + s1,mOrigin + s2,mOrigin + s3));
    }
    
    void extend(){
        hasExtend = true;
  
            for(auto &v:mTris){
                v.imSetTargetMode();
                v.setAlpha(1.0);
            }
        
        /*for(auto &v:mLines){
            v.setTargetMode();
            v.setAlpha(1.0);
        }*/
    }
    
    void extendEase(){
        hasExtend = true;
        
        for(auto &v:mTris){
            v.setTargetMode();
            v.setAlpha(1.0);
        }
        
        /*for(auto &v:mLines){
         v.setTargetMode();
         v.setAlpha(1.0);
         }*/
    }
    
    void randomizeMode(){
        for(auto &v:mTris){
            v.randomizeMode();
        }
    }
    
    void shrink(){
        hasExtend = false;
    }
    
    void update(){

        for(auto &v:mTris){
            
            if(hasExtend == false){
                v.kill();
            }
            
            if(isDrawAll){
                v.setMode(0);
                v.setAlpha(1.0);
            }
            
            v.update();
        }
        
        /*for(auto &v:mLines){
            if(hasExtend == false){
                v.kill();
            }
            if(isDrawAll){
                v.setMode(0);
                v.setAlpha(1.0);
            }
            v.update();
        }*/
        
        int counter = 0;
        
        for(auto &v:mTris){
            const vector<ofVec2f> &tmpV = v.getVerts();
            const vector<float> &tmpA = v.getAlpha();

            for(int i = 0; i < tmpV.size(); i++){
                mVerts[counter].set(tmpV[i]);
                mColors[counter].set(tmpA[i]);
                counter++;
            }
        }
        
        mVbo.updateVertexData(&mVerts[0], mVerts.size());
        mVbo.updateColorData(&mColors[0], mColors.size());
        
    }
    void draw(){
     
       // for(auto &v:mTris){
        //   v.draw();
       // }
        
        /*for(auto &v:mLines){
            v.draw();
        }*/
        
       // ofPushMatrix();
       // ofTranslate(0, 0, mOrigin.z); // vbo uses ofVec2f, so vbo ignore origin.z
        mVbo.draw(GL_LINES, 0, mVerts.size());
       // ofPopMatrix();
        
    }
    
    void togAllDraw(){
        isDrawAll = !isDrawAll;
    }
    
    void setAllDraw(bool flag){
        isDrawAll = flag;
    }
    
    ofPoint getOrigin(){
        return mOrigin;
    }
    
private:

    ofPoint mOrigin;
    
    bool hasExtend = false;
    
    vector<dpIceMapTri>mTris;
    
    ofVbo mVbo;
    vector<ofVec2f>mVerts;
    vector<ofFloatColor>mColors;
    
   // vector<MapLine>mLines;
    
    bool isDrawAll = false;

};

#endif
