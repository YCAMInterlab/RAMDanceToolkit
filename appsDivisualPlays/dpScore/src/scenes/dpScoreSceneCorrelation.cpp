
//
//  dpScoreSceneCorrelation.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 1/21/15.
//
//

#include "dpScoreSceneCorrelation.h"

DP_SCORE_NAMESPACE_BEGIN

static const string kKeyDancer{"Dancer"};
static const string kKeyRDTK{"RAM DTK"};
static const string kKeyHakoniwa{"Hakoniwa"};
static const string kKeyAnalyze{"Analyze"};
static const string kKeyOutput{"Output"};
static const string kKeyZoomOut{"ZoomOut"};

static const int kNumOutput{4};

static const float kFontSize{60.f};
static const float kFontSizeBig{100.f};

static const float kBoxSize{200.f};

static const int kNumDancers{3};
static const int kNumRDTK{1};

static const float kDistance{2000.f};

#pragma mark ___________________________________________________________________
SceneCorrelation::Node::Node(SceneCorrelation* _owner)
{
    owner = _owner;
}

void SceneCorrelation::Node::customDraw()
{
    ofPushStyle();
    ofSetColor(ofColor::white);
    drawName();
    ofPopStyle();
}

void SceneCorrelation::Node::drawName()
{
    ofPushMatrix();
    billboard();
    //string initial{name.substr(0, 1)};
    //string other{name.substr(1, name.size())};
    //auto& font = owner->getFontInitial();
    //font.drawString(initial, 0.f, 0.f);
    //ofTranslate(font.stringWidth(initial) + 20.f, 0.f);
    //owner->getFont().drawString(other, 0.f, 0.f);
    owner->getFontInitial().drawString(name, 0.f, 0.f);
    ofPopMatrix();
}

#pragma mark ___________________________________________________________________
SceneCorrelation::NodeDancer::NodeDancer(SceneCorrelation* _owner) :
Node(_owner),
rotAxis(randVec3f())
{
    for (auto& v : noiseVec) v = randVec3f();
    name = kKeyDancer;
}

void SceneCorrelation::NodeDancer::update()
{
    const float t{owner->getTime()};
    float angle{0.f};
    for (int i=0; i<noiseVec.size(); i++) {
        auto& v = noiseVec.at(i);
        angle += ofSignedNoise(t * v.x, t * v.y, t*v.z) * ::powf(10, (i + 1)) * 0.1f;
    }
    ofQuaternion quat(angle, rotAxis);
    setOrientation(quat);
}

void SceneCorrelation::NodeDancer::customDraw()
{
    ofPushStyle();
    ofSetColor(ofColor::white);
    ofDrawBox(kBoxSize);
    ofPopStyle();
    Node::customDraw();
}

#pragma mark ___________________________________________________________________
SceneCorrelation::NodeRDTK::NodeRDTK(SceneCorrelation* _owner) :
NodeDancer(_owner)
{
    name = kKeyRDTK;
}

void SceneCorrelation::NodeRDTK::update()
{
    NodeDancer::update();
}

void SceneCorrelation::NodeRDTK::customDraw()
{
    ofPushStyle();
    ofSetColor(ofColor::white);
    ofDrawBox(kBoxSize);
    ofPopStyle();
    Node::customDraw();
}

#pragma mark ___________________________________________________________________
SceneCorrelation::NodeHakoniwa::NodeHakoniwa(SceneCorrelation* _owner) :
NodeRDTK(_owner)
{
    name = kKeyHakoniwa;
    points.assign(ofRandom(1, 5), Point());
    for (auto& p : points) {
        p.spd = randVec3f() * 5.f;
    }
}

void SceneCorrelation::NodeHakoniwa::update()
{
    NodeDancer::update();
    
    for (auto& p : points) {
        p.pos += p.spd;
        for (int i=0; i<p.pos.DIM; i++) {
            if (::fabsf(p.pos[i]) >= kBoxSize * 0.5f - PointSize * 0.5f)
                p.spd[i] *= -1.f;
        }
    }
}

void SceneCorrelation::NodeHakoniwa::customDraw()
{
    ofPushStyle();
    ofSetColor(ofColor::white);
    ofDrawBox(kBoxSize);
    ofPopStyle();
    
    ofPushStyle();
    ofSetLineWidth(2.5f);
    ofSetColor(color::kMain);
    for (auto& p : points) ofDrawSphere(p.pos, PointSize);
    ofPopStyle();
    
    Node::customDraw();
}

#pragma mark ___________________________________________________________________
SceneCorrelation::NodeAnalyze::NodeAnalyze(SceneCorrelation* _owner) :
NodeHakoniwa(_owner)
{
    name = kKeyAnalyze;
}

void SceneCorrelation::NodeAnalyze::update()
{
    NodeDancer::update();
}

void SceneCorrelation::NodeAnalyze::customDraw()
{
    ofPushStyle();
    ofSetLineWidth(2.5f);
    ofSetColor(color::kMain);
    const float t0{::fmodf(owner->mTime * 0.5f, 1.f)};
    const float t1{::fmodf(owner->mTime * 1.1f, 1.f)};
    const float t2{::fmodf(owner->mTime * 0.7f, 1.f)};
    
    const float s{kBoxSize};
    const float tt0{(t0 - 0.5f) * s};
    const float tt1{(t1 - 0.5f) * s};
    const float tt2{(t2 - 0.5f) * s};
    
    ofLine( s*0.5f, tt0,  s*0.5f, -s*0.5f, tt0,  s*0.5f);
    ofLine( s*0.5f, tt0, -s*0.5f, -s*0.5f, tt0, -s*0.5f);
    ofLine( s*0.5f, tt0,  s*0.5f,  s*0.5f, tt0, -s*0.5f);
    ofLine(-s*0.5f, tt0,  s*0.5f, -s*0.5f, tt0, -s*0.5f);
    
    ofLine( s*0.5f,  s*0.5f, tt1, -s*0.5f,  s*0.5f, tt1);
    ofLine( s*0.5f, -s*0.5f, tt1, -s*0.5f, -s*0.5f, tt1);
    ofLine( s*0.5f,  s*0.5f, tt1,  s*0.5f, -s*0.5f, tt1);
    ofLine(-s*0.5f,  s*0.5f, tt1, -s*0.5f, -s*0.5f, tt1);
    
    ofLine(tt2,  s*0.5f, s*0.5f, tt2, -s*0.5f,  s*0.5f);
    ofLine(tt2,  s*0.5f,-s*0.5f, tt2, -s*0.5f, -s*0.5f);
    ofLine(tt2,  s*0.5f, s*0.5f, tt2,  s*0.5f, -s*0.5f);
    ofLine(tt2, -s*0.5f, s*0.5f, tt2, -s*0.5f, -s*0.5f);
    ofPopStyle();
    
    ofPushStyle();
    ofSetColor(ofColor::white);
    ofDrawBox(kBoxSize);
    ofPopStyle();
    
    Node::customDraw();
}

#pragma mark ___________________________________________________________________
SceneCorrelation::NodeSound::NodeSound(SceneCorrelation* _owner) :
NodeAnalyze(_owner)
{
    name = "Sound";
}

void SceneCorrelation::NodeSound::update()
{
    NodeDancer::update();
}

void SceneCorrelation::NodeSound::customDraw()
{
    ofPushStyle();
    
    float t{::fmodf(owner->mTime, 0.5f) * 2.f};
    t = easeInSine(1.f - t);
    auto drawNode = [&]() {
        ofPushMatrix();
        ofTranslate(ofRandom(ofRandom(-100, -20), ofRandom(20, 100)) * t,
                    ofRandom(ofRandom(-100, -20), ofRandom(20, 100)) * t,
                    ofRandom(ofRandom(-100, -20), ofRandom(20, 100)) * t);
        ofSetColor(ofRandom(255) * t+ 255 * (1.f - t));
        const float s{ofRandom(1.f, 2.f) * t + 1 * (1.f - t)};
        ofScale(s, s, s);
        ofDrawBox(kBoxSize);
        ofPopMatrix();
    };
    for (int i=0; i<3; i++) drawNode();
    ofPopStyle();
    
    Node::customDraw();
}

#pragma mark ___________________________________________________________________
SceneCorrelation::NodeLight::NodeLight(SceneCorrelation* _owner) :
NodeAnalyze(_owner)
{
    name = "Light";
}

void SceneCorrelation::NodeLight::update()
{
    NodeDancer::update();
}

void SceneCorrelation::NodeLight::customDraw()
{
    ofPushStyle();
    
    ofSetColor(ofColor::white);
    ofDrawBox(kBoxSize);
    const float t{::fmodf(owner->mTime, 2.f) * 0.5f};
    if (t < prevT) fixedQuat = getOrientationQuat();
    prevT = t;
    restoreTransformGL();
    
    setOrientation(fixedQuat);
    
    transformGL();
    ofColor c;
    c.setHsb(t * 255.f, 255, 255, 255);
    
    const float s{1.f + t * 5.f};
    const float a{easeInSine(1.f - t) * 255.f};
    ofSetColor(c, a);
    ofScale(s, s, s);
    ofDrawBox(kBoxSize);
    ofPopStyle();
    
    Node::customDraw();
}

#pragma mark ___________________________________________________________________
SceneCorrelation::NodeHakoVis::NodeHakoVis(SceneCorrelation* _owner) :
NodeAnalyze(_owner)
{
    name = "Visual";
}

void SceneCorrelation::NodeHakoVis::update()
{
    NodeDancer::update();
    unsigned char pixels[dim * dim * 4];
    for (int i=0; i<dim; i++) {
        for (int j=0; j<dim; j++) {
            const int idx{(i * dim + j) * 4};
            ofColor col;
            col.setHsb(ofRandom(255.f), 255, 255);
            pixels[idx + 0] = col.r;
            pixels[idx + 1] = col.g;
            pixels[idx + 2] = col.b;
            if (idx + 4 < dim * dim * 4)
                pixels[idx + 4] = 50;
        }
    }
    
    ofPushStyle();
    ofDisableArbTex();
    tex.setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
    tex.loadData(pixels, dim, dim, GL_RGBA);
    ofPopStyle();
}

void SceneCorrelation::NodeHakoVis::customDraw()
{
    const float t{::fmodf(owner->mTime * 0.5f, 1.f)};
    ofPushStyle();
    ofNoFill();
    ofSetColor(ofColor::white);
    ofDrawBox(kBoxSize);
    ofFill();
    tex.bind();
    ofDrawBox(kBoxSize);
    tex.unbind();
    ofPopStyle();
    
    Node::customDraw();
}

#pragma mark ___________________________________________________________________
SceneCorrelation::NodeScore::NodeScore(SceneCorrelation* _owner) :
NodeAnalyze(_owner)
{
    name = "Score";
    points.assign(numPoints, ofVec3f::zero());
    vbo.setVertexData(&points.at(0), points.size(), GL_DYNAMIC_DRAW);
}

void SceneCorrelation::NodeScore::update()
{
    NodeDancer::update();
    const float t{owner->mTime * 2.f};
    for (int i=0; i<points.size(); i++) {
        const float step{kBoxSize / numPoints};
        points.at(i).x = step * i - kBoxSize * 0.5f;
        points.at(i).y = ofSignedNoise(i*0.1f+t) * kBoxSize * 0.65f;
        points.at(i).z = 0.f;
    }
    vbo.updateVertexData(&points.at(0), points.size());
}

void SceneCorrelation::NodeScore::customDraw()
{
    ofPushStyle();
    ofEnableDepthTest();
    ofSetColor(ofColor::white);
    ofDrawBox(kBoxSize);
    ofSetColor(color::kMain);
    vbo.draw(GL_LINE_STRIP, 0, points.size());
    ofDisableDepthTest();
    ofPopStyle();
    
    Node::customDraw();
}

#pragma mark ___________________________________________________________________
void SceneCorrelation::initialize()
{
    dpDebugFunc();
    
    mUICanvas = new ofxUICanvas();
    mUICanvas->setName(getShortName());
    mUICanvas->addLabel(getShortName(), OFX_UI_FONT_SMALL);
    mUICanvas->addSpacer();
}

void SceneCorrelation::shutDown()
{
    dpDebugFunc();
    
    if (mUICanvas) {
        delete mUICanvas;
        mUICanvas = nullptr;
    }
}

void SceneCorrelation::enter()
{
    dpDebugFunc();
    
    mPrevTarget = -1;
    mCurrentTarget = 0;
    mPrevT = 0.f;
    
    deleteNodes();
    
    const float dist{2000.f};
    for (int i=0; i<kNumDancers; i++) {
        auto* daner = new NodeDancer(this);
        ofVec3f p;
        p.x = -dist * 1.5f;
        p.y = getLineUped(3000.f, i, kNumDancers);
        daner->setPosition(p);
        mNodes.insert(NodePair(kKeyDancer, daner));
    }
    
    for (int i=0; i<kNumRDTK; i++) {
        auto* rdtk = new NodeRDTK(this);
        ofVec3f p;
        p.x = -dist * 0.5f;
        p.y = getLineUped(2000.f, i, kNumRDTK);
        rdtk->setPosition(p);
        mNodes.insert(NodePair(kKeyRDTK, rdtk));
    }
    
    ofxXmlSettings xml;
    xml.load(kXmlSettingsPath);
    
    xml.pushTag("rdtk");
    xml.pushTag("phase", 0);
    vector<string> sceneNames;
    for (int j=0; j<xml.getNumTags("scene"); j++) {
        const string name{xml.getAttribute("scene", "name", "error", j)};
        sceneNames.push_back(name);
    }
    xml.popTag();
    
    vector<NodeHakoniwa*> hakoniwas;
    for (int i=0; i<sceneNames.size(); i++) {
        auto* hakoniwa = new NodeHakoniwa(this);
        ofVec3f p;
        float t{i/(float)sceneNames.size()*2.f - 1.f};
        t *= PI * 0.8f;
        p.x = ::cos(t);
        p.y = ::sin(t);
        p.z = ofRandom(-0.35f, 0.35f);
        p *= dist * ofRandom(0.7f, 1.f);
        
        hakoniwa->setPosition(p);
        mNodes.insert(NodePair(kKeyHakoniwa, hakoniwa));
        
        string name{sceneNames.at(i)};
        ofStringReplace(name, "dpVis", "");
        ofStringReplace(name, "dpH", "");
        ofStringReplace(name, "RE", "");
        ofStringReplace(name, "Theta", "FishEye");
        hakoniwa->name = name;
        hakoniwas.push_back(hakoniwa);
    }
    
    {
        auto* analyze = new NodeAnalyze(this);
        ofVec3f p;
        p.x = dist * 2.f;
        analyze->setPosition(p);
        mNodes.insert(NodePair(kKeyAnalyze, analyze));
    }
    
    vector<Node*> outputs;
    for (int i=0; i<kNumOutput; i++) {
        Node* output{nullptr};
        
        switch (i) {
            case 0: output = new NodeSound(this); break;
            case 1: output = new NodeLight(this); break;
            case 2: output = new NodeHakoVis(this); break;
            case 3: output = new NodeScore(this); break;
            default: output = new NodeHakoVis(this); break;
        }
        ofVec3f p;
        float t{i/(float)kNumOutput*2.f - 1.f};
        t *= PI * 0.8f;
        p.x = ::cos(t);
        p.y = ::sin(t);
        p.z = 0.f;
        p *= dist;
        p.x += dist * 3.f;
        
        output->setPosition(p);
        mNodes.insert(NodePair(kKeyOutput, output));
        
        outputs.push_back(output);
    }
    
    {
        auto* zoomout = new NodeVoid(this);
        ofVec3f p;
        p.x = 0.f;
        p.z = 2600.f;
        zoomout->setPosition(p);
        mNodes.insert(NodePair(kKeyZoomOut, zoomout));
    }
    
    mFontInitial.loadFont(kFontPath, kFontSizeBig);
    mFont.loadFont(kFontPath, kFontSize);
    
    mCam.setFarClip(50000.f);
    
    mEnterTime = ofGetElapsedTimef();
}

void SceneCorrelation::exit()
{
    dpDebugFunc();
    
    mFont = ofTrueTypeFont();
    mFontInitial = ofTrueTypeFont();
    
    mPrevTarget = -1;
    mCurrentTarget = 0;
    mPrevT = 0.f;
    
    deleteNodes();
}

void SceneCorrelation::deleteNodes()
{
    for (auto& p : mNodes) {
        delete p.second;
        p.second = nullptr;
    }
    
    mNodes.clear();
    
    mFrom = nullptr;
    mTo =  nullptr;
    mCurrentHakoniwa = nullptr;
    mCurrentDancer = nullptr;
    mCurrentOutput = nullptr;
}

void SceneCorrelation::update(ofxEventMessage& m)
{
    mTime = ofGetElapsedTimef() - mEnterTime;
    
    const int nAnimSeq{7};
    const int zoomOutSeq{5};
    const int zoomInSeq{6};
    
    float t{::fmodf(mTime, 6.f)};
    if (t < mPrevT) {
        ++mCurrentTarget %= nAnimSeq;
    }
    mPrevT = t;
    
    t = ofMap(t, 0.f, 6.f, -1.f, 3.f);
    t = ofClamp(t, 0.f, 1.f);
    t = easeInOutExpo(t);
    float fov{0.f};
    
    if (mCurrentTarget != mPrevTarget) {
        auto updateTarget = [&](const string& key, Node** target){
            auto count = mNodes.count(key);
            auto it = mNodes.find(key);
            for (int i=0; i<(int)ofRandom(count-FLT_EPSILON); i++) {
                ++it;
            }
            if (it != mNodes.end()) *target = it->second;
        };
        
        switch (mCurrentTarget) {
            case 0:
                if (mCurrentDancer)
                    mFrom = mCurrentDancer;
                else
                    updateTarget(kKeyDancer, &mFrom);
                updateTarget(kKeyRDTK, &mTo);
                break;
            case 1:
                mFrom = mTo;
                updateTarget(kKeyHakoniwa, &mTo);
                mCurrentHakoniwa = mTo;
                break;
            case 2:
                mFrom = mTo;
                updateTarget(kKeyAnalyze, &mTo);
                break;
            case 3:
                mFrom = mTo;
                updateTarget(kKeyOutput, &mTo);
                mCurrentOutput = mTo;
                break;
            case 4:
                mFrom = mTo;
                updateTarget(kKeyDancer, &mTo);
                mCurrentDancer = mTo;
                break;
            case zoomOutSeq:
                mFrom = mTo;
                updateTarget(kKeyZoomOut, &mTo);
                break;
            case zoomInSeq:
                mFrom = mTo;
                if (mCurrentDancer)
                    mTo = mCurrentDancer;
                else
                    updateTarget(kKeyDancer, &mTo);
                break;
            default:
                break;
        }
    }
    if (mCurrentTarget == zoomOutSeq) fov = t;
    else if (mCurrentTarget == zoomInSeq) fov = 1 - t;
    mPrevTarget = mCurrentTarget;
    
    mCam.setFov(45.f + fov * 35.f);
    
    if (mFrom && mTo) {
        const ofVec3f fromPos{mFrom->getGlobalPosition()};
        const ofVec3f toPos{mTo->getGlobalPosition()};
        ofVec3f camOffset{ofVec3f(0.f, 0.f, 1.f) * 750.f};
        
        const ofVec3f camPos{fromPos.interpolated(toPos, t) + camOffset};
        const ofVec3f lookAt{fromPos.interpolated(toPos, t)};
        
        if (mMoveCamera) {
            mCam.setGlobalPosition(camPos);
            mCam.lookAt(ofVec3f(lookAt.x+150.f, lookAt.y, lookAt.z));
        }
        
        if (mCurrentTarget != zoomOutSeq && mCurrentTarget != zoomInSeq)
            mLookAt = lookAt;
    }
    
    for (auto& p : mNodes) {
        p.second->update();
    }
}

void SceneCorrelation::draw()
{
    ofSetColor(ofColor::white);
    ofNoFill();
    ofDisableDepthTest();
    ofEnableAlphaBlending();
    
    ofSetSphereResolution(2);
    
    mCam.begin();
    ofSetLineWidth(2.5f);
    ofSetColor(150);
    for (auto it0 = mNodes.begin(); it0 != mNodes.end(); ++it0) {
        for (auto it1 = it0; it1 != mNodes.end(); ++it1) {
            if (it0 != it1) {
                bool drawLine{false};
                if (it0->first == kKeyDancer && it1->first == kKeyRDTK)
                    drawLine = true;
                else if (it1->first == kKeyDancer && it0->first == kKeyRDTK)
                    drawLine = true;
                else if (it0->first == kKeyRDTK && it1->first == kKeyHakoniwa)
                    drawLine = true;
                else if (it1->first == kKeyRDTK && it0->first == kKeyHakoniwa)
                    drawLine = true;
                else if (it0->first == kKeyRDTK && it1->first == kKeyAnalyze)
                    drawLine = true;
                else if (it1->first == kKeyAnalyze && it0->first == kKeyRDTK)
                    drawLine = true;
                if (drawLine) {
                    ofLine(it0->second->getGlobalPosition(),
                           it1->second->getGlobalPosition());
                }
            }
        }
    }
    
    ofSetLineWidth(2.5f);
    ofSetColor(150);
    if (mCurrentHakoniwa) {
        auto* analuze = mNodes.find(kKeyAnalyze)->second;
        ofLine(mCurrentHakoniwa->getGlobalPosition(), analuze->getGlobalPosition());
        
        for (auto& p : mNodes) {
            if (p.first == kKeyOutput) {
                ofLine(p.second->getGlobalPosition(), analuze->getGlobalPosition());
            }
        }
    }
    if (mCurrentDancer && mCurrentOutput) {
        ofLine(mCurrentDancer->getGlobalPosition(),
               mCurrentOutput->getGlobalPosition());
    }
    
    
    ofPushMatrix();
    ofTranslate(mLookAt);
    ofRotateZ(mTime * 41.f);
    ofRotateY(mTime * 28.f);
    ofRotateX(mTime * 13.f);
    //ofSetColor(ofColor::white - color::kMain);
    ofSetColor(color::kMain);
    ofDrawBox(kBoxSize * 0.25f);
    ofPopMatrix();
    
    for (auto& p : mNodes) {
        ofSetLineWidth(2.5f);
        ofSetColor(ofColor::white);
        p.second->draw();
    }
    
    mCam.end();
    
    
    // invert
    //    ofPushStyle();
    //    ofFill();
    //    glEnable(GL_BLEND);
    //    glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ZERO);
    //    ofSetColor(ofColor::white);
    //    ofRect(ofGetWindowRect());
    //    ofPopStyle();
}

DP_SCORE_NAMESPACE_END
