//
//  BurstBody_ext.h
//  RAMDanceToolkit
//
//  Created by kezzardrix2 on 2016/01/28.
//
//

#ifndef RAMDanceToolkit_BurstBody_ext_h
#define RAMDanceToolkit_BurstBody_ext_h

#include "ramMotionExtractor.h"
#include "KezSlidePoint.h"
#include "BurstBody.h"

#include "ramCenteredActor.h"

class BurstBody_ext : public ramBaseScene
{
    
public:
    
    string getName() const { return "BurstBody_ext"; }
    
    void setupControlPanel()
    {
        
        ramGetGUI().addSlider("thresh", 0.1, 30.0, &mThresh);
        ramGetGUI().addToggle("threshDir",&mThreshDir);
        ramGetGUI().addToggle("velMode",&mVelMode);
   
        ramGetGUI().addToggle("Centered", &mIsCentered);
        
        ramGetGUI().addSlider("trans:x", -300, 300, &mTrans.x);
        ramGetGUI().addSlider("trans:y", -300, 300, &mTrans.y);
        ramGetGUI().addSlider("trans:z", 0, 2000, &mTrans.z);
        
        ofxUICanvasPlus* gui = ramGetGUI().getCurrentUIContext();
        gui->addWidgetDown(new ofxUILabel("BodyColor", OFX_UI_FONT_MEDIUM));

        
        gui->addSlider("R", 0, 1, &bodyColor.r, 70, 16);
        gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
        gui->addSlider("G", 0, 1, &bodyColor.g, 70, 16);
        gui->addSlider("B", 0, 1, &bodyColor.b, 70, 16);
        
        gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
        gui->autoSizeToFitWidgets();
		
        mex.setupControlPanel(this);
        
        bodyColor.set(dpDancerFloatColor::SHIMAJI_COLOR.r,
                      dpDancerFloatColor::SHIMAJI_COLOR.g,
                      dpDancerFloatColor::SHIMAJI_COLOR.b);

    }
    
    void setup()
    {
        
#define _S(src) #src
        
        const char *vs = _S(
                            
                            varying vec3 normal;
                            varying vec4 worldPos;
                            
                            void main(){
                                gl_Position = ftransform();
                                worldPos = gl_ModelViewMatrix * gl_Vertex;
                                
                                normal = gl_NormalMatrix * gl_Normal;
                                gl_FrontColor = gl_Color;
                            }
                            );
        
        const char *fs = _S(
                            
                            varying vec3 normal;
                            varying vec4 worldPos;
                            uniform vec3 light;
                            
                            vec3 ads(){
                                vec3 n = normalize(normal.xyz);
                                vec3 s = normalize(light);//-v_Vertex.xyz);
                                vec3 v = normalize(vec3(-worldPos));
                                vec3 r = reflect(-s,-n);
                                
                                if(dot(n,s) > 0.0)return vec3(max(dot(s,n), 0.0) + vec3(pow(max(dot(r,v),0.0),10.0)) * 1.0) * 1.0;
                                else return vec3(0,0,0);
                            }
                            
                            void main(){
                                gl_FragColor = vec4(gl_Color.rgb * ads(),1.0);
                            }
        );
        
#undef _S
        
        mShader.setupShaderFromSource(GL_VERTEX_SHADER, vs);
        mShader.setupShaderFromSource(GL_FRAGMENT_SHADER, fs);
        mShader.linkProgram();
        
        bodyColor.set(dpDancerFloatColor::SHIMAJI_COLOR.r,
                      dpDancerFloatColor::SHIMAJI_COLOR.g,
                      dpDancerFloatColor::SHIMAJI_COLOR.b);
        
    }
    
    void updatWithOscMessage(ofxOscMessage &m){
        
    }
    
    void update()
    {
        ramActorManager &am = ramGlobalShortcut::getActorManager();
        
        int numNodes = 0;
        for (int i = 0;i < am.getNumNodeArray();i++)
            numNodes += am.getNodeArray(i).getNumNode();
        
        if (mBoxes.size() != numNodes)
        {
            mBoxes.clear();
            for (int i = 0;i < numNodes;i++)
                mBoxes.push_back(BurstBox());
        }
        
        mex.update();
        
        for(auto &v:mBoxes){
            v.setVelMode(mVelMode);
        }
    }
    
    void draw()
    {
        
        ramBeginCamera();
        
        ramActorManager &am = ramGlobalShortcut::getActorManager();
        int cnt = 0;
        for (int i = 0;i < am.getNumNodeArray();i++)
        {

            ramActor act = am.getNodeArray(i);
            bool bActEnable = false;
            for (int q = 0;q < mex.getNumPort();q++)
                if (mex.getActorNameAt(q) == act.getName()) bActEnable = true;
            
            if (!bActEnable) continue;
            
            const ofPoint center = act.getNode(ramActor::JOINT_ABDOMEN).getGlobalPosition();
            
            ofPushMatrix();
            if(mIsCentered)ofTranslate(-center.x,0.0,-center.z);
            ofTranslate(mTrans);
            
            for (int j = 0;j < act.getNumNode();j++)
            {
                ramNode & tmpNode = act.getNode(j);
                
                float jointSize = (j == ramActor::JOINT_HEAD) ? 8.0 : 5.0;
                float length = tmpNode.getVelocity().length();
                int idx = cnt;
                
                if(mThreshDir)mBoxes[idx].bFixed = (length > mThresh);
                else mBoxes[idx].bFixed = (length < mThresh);
                
                mBoxes[idx].update(tmpNode.getGlobalPosition(),act.getNode(i).getVelocity());
                
                tmpNode.setGlobalPosition(mBoxes[idx].mPos);
                
                mShader.begin();
                mShader.setUniform3f("light", 0, 0, 100);
                ofSetColor(bodyColor);
                ramBox(tmpNode, jointSize);
                ramLine(tmpNode);
                mShader.end();
                
                cnt++;
            }
            
            ofPopMatrix();
        }
        
        mex.draw();
        ramEndCamera();
    }
    
    void drawActor(const ramActor& actor)
    {
        
    }
    
    
    
    void drawRigid(const ramRigidBody &rigid)
    {
        
    }
    
    void onActorSetup(const ramActor &actor)
    {
        
    }
    
    void onActorExit(const ramActor &actor)
    {
        
    }
    
    void onRigidSetup(const ramRigidBody &rigid)
    {
        
    }
    
    void onRigidExit(const ramRigidBody &rigid)
    {
        
    }
    
    
    void onPanelChanged(ofxUIEventArgs& e)
    {
       
    }
    
private:

    float mThresh = 2.0;
    
    bool mThreshDir = true;
    bool mVelMode = false;
    
    ofFloatColor bodyColor;
    vector<BurstBox> mBoxes;
    ramMotionExtractor mex;
    
    ofShader mShader;
    
    ofPoint mTrans;
    
    bool mIsCentered = true;
    
};


#endif
