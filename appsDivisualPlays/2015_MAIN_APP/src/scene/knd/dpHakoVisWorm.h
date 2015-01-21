//
//  dpHakoVisWave.h
//  RAMDanceToolkit
//
//  Created by kezzardrix2 on 2015/01/11.
//
//

#ifndef RAMDanceToolkit_dpHakoVisWave_h
#define RAMDanceToolkit_dpHakoVisWave_h

#define STRINGIFY(A) #A

class dpWaveLine{
private:
    
    float mVectorMax = 10.0;
    float mRadScale = 20.0;
    
    bool enableMove = true;
    
public:
    KezSlidePoint mPos;
    ofPoint mOrigin;
    ofFloatColor mColor;
  
    float mToMainColor = 0.0;
    float mToMainColorInc = 0.02;
    
    float mLength = 0.0;
    
    void setup(ofPoint pos,ofFloatColor color,bool _enableMove){
        mPos.imSet(pos);
        mColor = color;
        mOrigin = mPos;
        mPos.speed = 0.01;
        enableMove = _enableMove;
        
        mToMainColor = 1.0;
        
    }
    
    void setRad(float rad){
        mRadScale = rad;
    }
    
    void force(ofPoint vec){
        
        if(enableMove){
            mPos.imSet(vec.x + mPos.x,
                       vec.y + mPos.y,
                       mPos.z + vec.length());
            
            mVectorMax = (mPos - mOrigin).length();
            if(mVectorMax <= 0)mVectorMax = 1.0;
            
            mPos.set(mOrigin);
            mColor.g = 0.0;
            mColor.b = 0.0;
        
        }
        
        mToMainColor = 0.0;
    }
    
    void update(){
        mPos.update();
        mLength = ofMap((mPos - mOrigin).length(),0.0,mVectorMax,0,1.0,true);
      
        mToMainColor += mToMainColorInc;
        
        mColor.r = ofLerp(dpFloatColor::MAIN_COLOR.r, 1.0, mToMainColor);
        mColor.g = ofLerp(dpFloatColor::MAIN_COLOR.g, 1.0, mToMainColor);
        mColor.b = ofLerp(dpFloatColor::MAIN_COLOR.b, 1.0, mToMainColor);
     
    }
    
    ofPoint getPos(){
        return mPos;
    }
    
    ofFloatColor getColor(){
        return mColor;
    }
    
    float getPointSize(){
        return mLength * mRadScale;
    }
};

class dpVisWorm : public ramBaseScene{
public:
    
    string getName() const{return "dpVisWorm";};
    
    void setupControlPanel(){
        
        ramGetGUI().addSlider("vecScale", 1.0, 1000.0, &mVecScale);
        ramGetGUI().addSlider("rad", 1.0, 100.0, &mRad);
        ramGetGUI().addIntSlider("lineWidth", 1, 10, &mLineWidth);
        ramGetGUI().addIntSlider("divX",1,150,&mDivX);
        ramGetGUI().addIntSlider("divY",1,150,&mDivY);
        ramGetGUI().addToggle("isDrawLine", &isDrawLine);
    
        mMotionExtractor.setupControlPanel(this);
        ofAddListener(ramGetGUI().getCurrentUIContext()->newGUIEvent, this, &dpVisWorm::onPanelChanged);
    }
    
    void setup(){
        
        setupShader();
        setupVbo();
        
        ramOscManager::instance().addReceiverTag(&mReceiver);
        mReceiver.addAddress("/dp/cameraUnit/Worm/features");
    }
    
    void setupVbo(){
        mLines.clear();
        mVerts.clear();
        mColors.clear();
        mPointSizes.clear();
        
        float offsetX = 100.0;
        float offsetY = 100.0;
        
        float mGridX = (SINGLE_SCREEN_WIDTH - offsetX) / (mDivX - 1);
        float mGridY = (SINGLE_SCREEN_HEIGHT - offsetY) / (mDivY - 1);
        
        
        for(int i = 0; i < mDivX; i++){
            
            for(int j = 0; j < mDivY; j++){
                
                mLines.push_back(dpWaveLine());
                ofPoint tmp(mGridX * i + offsetX * 0.5,mGridY * j + offsetY * 0.5,0.0);
                ofFloatColor color(1,1,1,1);
                
                bool enableMove = true;
                
                if(j == mDivY - 1 || j == 0){
                    color.set(0,0,0,0);
                    enableMove = false;
                }
                
                mLines.back().setup(tmp,color,enableMove);
                
            }
        }
        
        for(int i = 0; i < mLines.size(); i++){
            mVerts.push_back(ofPoint(0,0,0));
            mColors.push_back(ofFloatColor(1,1,1,1));
            mPointSizes.push_back(1.0);
        }
        
        mVbo.setVertexData(&mVerts[0],mVerts.size(),GL_DYNAMIC_DRAW);
        mVbo.setColorData(&mColors[0],mColors.size(),GL_DYNAMIC_DRAW);
        
        mPointSprite.begin();
        mPointSizeAttLoc = mPointSprite.getAttributeLocation("pointSize");
        mVbo.setAttributeData(mPointSizeAttLoc, &mPointSizes[0], 1, mLines.size(), GL_DYNAMIC_DRAW);
        mPointSprite.end();
    }
    
    void setupShader(){
        mVert = STRINGIFY(attribute float pointSize;
                        
                          void main(){
                              gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
                              gl_PointSize = pointSize;
                              gl_FrontColor = gl_Color;
                          }
                          
                          );
        
        mFrag = "#version 120\n";
        mFrag += STRINGIFY(
                          void main(){
                              vec3 n;
                              n.xy = gl_PointCoord * 2.0 - 1.0;
                              n.z = 1.0 - dot(n.xy, n.xy);
                              if (n.z < 0.0) discard;
                              
                              gl_FragColor = gl_Color;
                          }
                          );
        
        mPointSprite.setupShaderFromSource(GL_VERTEX_SHADER, mVert);
        mPointSprite.setupShaderFromSource(GL_FRAGMENT_SHADER, mFrag);
        mPointSprite.linkProgram();
        
        
    }
    
    void receiveOsc(){
        
        while(mReceiver.hasWaitingMessages()){
            
            ofxOscMessage m;
            mReceiver.getNextMessage(&m);
            
            if(m.getAddress() == "/dp/cameraUnit/Worm/features"){
                
                int num = m.getArgAsInt32(0);
                
                for(int i = 1; i < num * 4 + 1; i+=4){
                    
                    int idxX = ofMap(m.getArgAsFloat(i),0.0,1.0,0,mDivX,true);
                    int idxY = ofMap(m.getArgAsFloat(i+1),0.0,1.0,0,mDivY,true);
                    
                    if(idxX > 0 && idxY > 0){
                        int idx = idxX * mDivY + idxY;
                        mLines[idx].force(ofPoint(m.getArgAsFloat(i+2),
                                                  m.getArgAsFloat(i+3)) * mVecScale);
                        //cout << m.getArgAsFloat(i+2) << endl;
                    }
            
                }
            }
        }
    }
    
    void update(){
        /*mMotionExtractor.update();
        receiveOsc();
        
        for(int i = 0; i < mLines.size(); i++){
            mLines[i].update();
            mVerts[i] = mLines[i].getPos();
            mColors[i] = mLines[i].getColor();
            mPointSizes[i] = mLines[i].getPointSize();
        }
        
        mVbo.updateVertexData(&mVerts[0],mVerts.size());
        mVbo.updateColorData(&mColors[0],mColors.size());
        mVbo.updateAttributeData(mPointSizeAttLoc, &mPointSizes[0], mPointSizes.size());
         */
    }
    
    void draw(){
       /* ofDisableDepthTest();
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        ramSetViewPort(dpGetFirstScreenViewPort());
        ramBeginCamera();
        ofPushMatrix();
        
        ofTranslate(-SINGLE_SCREEN_WIDTH * 0.5, -SINGLE_SCREEN_HEIGHT * 0.5);

        ofSetLineWidth(mLineWidth);
        if(isDrawLine)mVbo.draw(GL_LINE_STRIP,0,mLines.size());
        
        ofEnablePointSprites();
        mPointSprite.begin();
        mVbo.draw(GL_POINTS,0,mLines.size());
        mPointSprite.end();
        
        ofPopMatrix();
        ofDisablePointSprites();
       
 
        ramEndCamera();*/
    }
    
    void onPanelChanged(ofxUIEventArgs& e){
        string name = e.widget->getName();
        
        if(name == "rad"){
            for(auto &v:mLines){
                v.setRad(mRad);
            }
        }
        
        if(name == "divX" || name == "divY"){
            
            if(mDivX % 2 != 0)mDivX++;
            
            if(mDivY % 2 != 0)mDivY++;
            
            setupVbo();
        }
    }
    
private:
    ramOscReceiveTag mReceiver;
    
    int mDivX = 50;
    int mDivY = 50;
    int mCurrentParticle = 0;
    int mPointSizeAttLoc = 0;
    int mLineWidth = 3;
    
    vector<dpWaveLine>mLines;
    vector<ofPoint>mVerts;
    vector<ofFloatColor>mColors;
    vector<float>mPointSizes;
   
    ofVbo mVbo;
    
    ramMotionExtractor mMotionExtractor;
    
    float mNodePosScale = 4.0;
    float mVecScale = 200.0;
    float mRad = 20.0;

    ofShader mPointSprite;
    string mVert;
    string mFrag;
    
    bool isDrawLine = true;
    
};


#endif
