//
//  dpScoreSceneDataDisplacement.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 1/15/15.
//
//

#include "dpScoreSceneDataDisplacement.h"

DP_SCORE_NAMESPACE_BEGIN

static const GLchar* kVertSrc = R"END(
#version 120

uniform float timeX = 1.0;
uniform float timeY = 1.0;
uniform float scaleX = 1.0;
uniform float scaleY = 1.0;
uniform float freq = 30.0;
uniform float amp = 30.0;
uniform vec2 mouse;

vec4 rand(vec2 A,vec2 B,vec2 C,vec2 D)
{
    vec2 s=vec2(12.9898,78.233);
    vec4 tmp=vec4(dot(A,s),dot(B,s),dot(C,s),dot(D,s));
    
    return fract(sin(tmp) * 43758.5453)* 2.0 - 1.0;
}

float noise(vec2 coord,float d)
{
    vec2 C[4];
    float d1 = 1.0/d;
    
    C[0]=floor(coord*d)*d1;
    C[1]=C[0]+vec2(d1,0.0);
    C[2]=C[0]+vec2(d1,d1);
    C[3]=C[0]+vec2(0.0,d1);
    
    vec2 p=fract(coord*d);
    vec2 q=1.0-p;
    vec4 w=vec4(q.x*q.y,p.x*q.y,p.x*p.y,q.x*p.y);
    
    return dot(vec4(rand(C[0],C[1],C[2],C[3])),w);
}

void main()
{
    vec4 pos = gl_Vertex;
    
    float noiseAmntX = noise(vec2(-timeX + pos.x / 2000.0, timeY + pos.y / 2000.0), freq);
    float noiseAmntY = noise(vec2(-timeX + pos.x / 2000.0, timeY + pos.y / 2000.0), freq);
    
    pos.x += noiseAmntX * amp * scaleX;
    pos.y += noiseAmntY * amp * scaleY;
    
    gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * pos;
    gl_FrontColor =  gl_Color;
}
)END";

static const GLchar* kFragSrc = R"END(
#version 120

void main()
{
    gl_FragColor = gl_Color;
}
)END";

#pragma mark ___________________________________________________________________
void SceneDataDisplacement::initialize()
{
    dpDebugFunc();
    
    mUICanvas = new ofxUICanvas();
    mUICanvas->setName(getShortName());
    mUICanvas->addLabel(getShortName(), OFX_UI_FONT_SMALL);
    mUICanvas->addSpacer();
    mUICanvas->addSlider("freq", 10.f, 50.f, &mFreq);
    mUICanvas->addSlider("amp", 10.f, 50.f, &mAmp);
    
    mShader.setupShaderFromSource(GL_VERTEX_SHADER, kVertSrc);
    mShader.setupShaderFromSource(GL_FRAGMENT_SHADER, kFragSrc);
    mShader.linkProgram();
}

void SceneDataDisplacement::shutDown()
{
    dpDebugFunc();
    
    if (mUICanvas) {
        delete mUICanvas;
        mUICanvas = nullptr;
    }
    
    mShader.unload();
}

void SceneDataDisplacement::enter()
{
    dpDebugFunc();
    mEnterTime = ofGetElapsedTimef();
    mSphereMesh = ofMesh::sphere(kRadius, kResolution);
}

void SceneDataDisplacement::exit()
{
    dpDebugFunc();    
    mSphereMesh.clear();
}

void SceneDataDisplacement::update(ofxEventMessage& m)
{
    if (m.getAddress() == kOscAddrCameraUnitVectorTotal) {
        const float t{ofGetElapsedTimef()-mEnterTime};
        const float r{2.f};
        const float s{1.5};
        const float sx{::fabsf(ofClamp(m.getArgAsFloat(0), -r, r) * s)};
        const float sy{::fabsf(ofClamp(m.getArgAsFloat(1), -r, r) * s)};
        updateShader(t, sx, sy);
    }
}

void SceneDataDisplacement::updateShader(float t, float sx, float sy)
{
    mShader.begin();
    mShader.setUniform1f("timeX", t*mSpeed.x);
    mShader.setUniform1f("timeY", t*mSpeed.y);
    mShader.setUniform1f("scaleX", sx);
    mShader.setUniform1f("scaleY", sy);
    mShader.setUniform1f("freq", mFreq);
    mShader.setUniform1f("amp", mAmp);
    mShader.end();
}

void SceneDataDisplacement::draw()
{
    mShader.begin();
    mCam.begin();
    ofRotateX(30.f);
    ofRotateY(15.f);
    ofRotateZ(-20.f);
    
    ofRotateY(ofGetElapsedTimef() * 15.f);
    
    ofNoFill();
    ofSetLineWidth(1.5f);
    ofSetSphereResolution(50);
    ofSetColor(ofColor::white, 100);
    mSphereMesh.drawWireframe();
    glPointSize(4.f);
    ofSetColor(ofColor::white, 128);
    mSphereMesh.drawVertices();
    mCam.end();
    mShader.end();
}

DP_SCORE_NAMESPACE_END