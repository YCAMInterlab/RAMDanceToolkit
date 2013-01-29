//
//  ofxUITabbedCanvas.cpp
//  ofxUISimpleExample
//
//  Created by Onishi Yoshito on 1/29/13.
//
//

#include "ofxUITabbedCanvas.h"

//--------------------------------------------------------------
ofxUITabbedCanvas::ofxUITabbedCanvas() :
mCurrentTab(0),
mVisible(true),
mTabs(NULL),
mTabsMatrix(NULL)
{
    mCanvases.clear();
    enableAppEventCallbacks();
    enableMouseEventCallbacks();
    enableKeyEventCallbacks();
    
    mTabSize.set(0.0f, 0.0f, 10.0f, 20.0f);
    rebuildTabs();
}

//--------------------------------------------------------------
ofxUITabbedCanvas::~ofxUITabbedCanvas()
{
    disableAppEventCallbacks();
    clear();
}

//--------------------------------------------------------------
void ofxUITabbedCanvas::update()
{
    at(mCurrentTab)->update();
}

//--------------------------------------------------------------
void ofxUITabbedCanvas::draw()
{
    if (mVisible) {
        ofPushMatrix();
        ofTranslate(0.0f, mTabSize.height);
        at(mCurrentTab)->draw();
        ofPopMatrix();
    }
}

//--------------------------------------------------------------
void ofxUITabbedCanvas::exit()
{
    at(mCurrentTab)->exit();
}

//--------------------------------------------------------------
void ofxUITabbedCanvas::resize()
{
    mTabs->getRect()->setWidth(mTabSize.width);
    for (int i=0; i<size(); i++) {
        at(i)->getRect()->setWidth(mTabSize.width);
    }
}

//--------------------------------------------------------------
void ofxUITabbedCanvas::keyPressed(int key)
{
    if (mVisible)
        at(mCurrentTab)->keyPressed(key);
}

//--------------------------------------------------------------
void ofxUITabbedCanvas::keyReleased(int key)
{
    if (mVisible)
        at(mCurrentTab)->keyReleased(key);
}

//--------------------------------------------------------------
void ofxUITabbedCanvas::mouseMoved(int x, int y)
{
    if (mVisible)
        at(mCurrentTab)->mouseMoved(x, y-mTabSize.height);
}

//--------------------------------------------------------------
void ofxUITabbedCanvas::mouseDragged(int x, int y, int button)
{
    if (mVisible)
        at(mCurrentTab)->mouseDragged(x, y-mTabSize.height, button);
}

//--------------------------------------------------------------
void ofxUITabbedCanvas::mousePressed(int x, int y, int button)
{
    if (mVisible)
        at(mCurrentTab)->mousePressed(x, y-mTabSize.height, button);
}

//--------------------------------------------------------------
void ofxUITabbedCanvas::mouseReleased(int x, int y, int button)
{
    if (mVisible)
        at(mCurrentTab)->mouseReleased(x, y-mTabSize.height, button);
}

//--------------------------------------------------------------
void ofxUITabbedCanvas::enableAppEventCallbacks()
{
#if OF_VERSION >= 7 && OF_VERSION_MINOR > 0
    ofAddListener(ofEvents().update, this, &ofxUITabbedCanvas::onUpdate);
    ofAddListener(ofEvents().draw, this, &ofxUITabbedCanvas::onDraw);
    ofAddListener(ofEvents().exit, this, &ofxUITabbedCanvas::onExit);
#else
    ofAddListener(ofEvents.update, this, &ofxUITabbedCanvas::onUpdate);
    ofAddListener(ofEvents.draw, this, &ofxUITabbedCanvas::onDraw);
    ofAddListener(ofEvents.exit, this, &ofxUITabbedCanvas::onExit);
#endif
}

//--------------------------------------------------------------
void ofxUITabbedCanvas::disableAppEventCallbacks()
{
#if OF_VERSION >= 7 && OF_VERSION_MINOR > 0
    ofRemoveListener(ofEvents().update, this, &ofxUITabbedCanvas::onUpdate);
    ofRemoveListener(ofEvents().draw, this, &ofxUITabbedCanvas::onDraw);
    ofRemoveListener(ofEvents().exit, this, &ofxUITabbedCanvas::onExit);
#else
    ofRemoveListener(ofEvents.update, this, &ofxUITabbedCanvas::onUpdate);
    ofRemoveListener(ofEvents.draw, this, &ofxUITabbedCanvas::onDraw);
    ofRemoveListener(ofEvents.exit, this, &ofxUITabbedCanvas::onExit);
#endif
}

//--------------------------------------------------------------
void ofxUITabbedCanvas::enableMouseEventCallbacks()
{
#if OF_VERSION >= 7 && OF_VERSION_MINOR > 0
    ofAddListener(ofEvents().mouseReleased, this, &ofxUITabbedCanvas::onMouseReleased);
    ofAddListener(ofEvents().mousePressed, this, &ofxUITabbedCanvas::onMousePressed);
    ofAddListener(ofEvents().mouseMoved, this, &ofxUITabbedCanvas::onMouseMoved);
    ofAddListener(ofEvents().mouseDragged, this, &ofxUITabbedCanvas::onMouseDragged);
#else
    ofAddListener(ofEvents.mouseReleased, this, &ofxUITabbedCanvas::onMouseReleased);
    ofAddListener(ofEvents.mousePressed, this, &ofxUITabbedCanvas::onMousePressed);
    ofAddListener(ofEvents.mouseMoved, this, &ofxUITabbedCanvas::onMouseMoved);
    ofAddListener(ofEvents.mouseDragged, this, &ofxUITabbedCanvas::onMouseDragged);
#endif
}

//--------------------------------------------------------------
void ofxUITabbedCanvas::disableMouseEventCallbacks()
{
#if OF_VERSION >= 7 && OF_VERSION_MINOR > 0
    ofRemoveListener(ofEvents().mouseReleased, this, &ofxUITabbedCanvas::onMouseReleased);
    ofRemoveListener(ofEvents().mousePressed, this, &ofxUITabbedCanvas::onMousePressed);
    ofRemoveListener(ofEvents().mouseMoved, this, &ofxUITabbedCanvas::onMouseMoved);
    ofRemoveListener(ofEvents().mouseDragged, this, &ofxUITabbedCanvas::onMouseDragged);
#else
    ofRemoveListener(ofEvents.mouseReleased, this, &ofxUITabbedCanvas::onMouseReleased);
    ofRemoveListener(ofEvents.mousePressed, this, &ofxUITabbedCanvas::onMousePressed);
    ofRemoveListener(ofEvents.mouseMoved, this, &ofxUITabbedCanvas::onMouseMoved);
    ofRemoveListener(ofEvents.mouseDragged, this, &ofxUITabbedCanvas::onMouseDragged);
#endif
}

//--------------------------------------------------------------
void ofxUITabbedCanvas::enableKeyEventCallbacks()
{
#if OF_VERSION >= 7 && OF_VERSION_MINOR > 0
    ofAddListener(ofEvents().keyPressed, this, &ofxUITabbedCanvas::onKeyPressed);
    ofAddListener(ofEvents().keyReleased, this, &ofxUITabbedCanvas::onKeyReleased);
#else
    ofAddListener(ofEvents.keyPressed, this, &ofxUITabbedCanvas::onKeyPressed);
    ofAddListener(ofEvents.keyReleased, this, &ofxUITabbedCanvas::onKeyReleased);
#endif
}

//--------------------------------------------------------------
void ofxUITabbedCanvas::disableKeyEventCallbacks()
{
#if OF_VERSION >= 7 && OF_VERSION_MINOR > 0
    ofRemoveListener(ofEvents().keyPressed, this, &ofxUITabbedCanvas::onKeyPressed);
    ofRemoveListener(ofEvents().keyReleased, this, &ofxUITabbedCanvas::onKeyReleased);
#else
    ofRemoveListener(ofEvents.keyPressed, this, &ofxUITabbedCanvas::onKeyPressed);
    ofRemoveListener(ofEvents.keyReleased, this, &ofxUITabbedCanvas::onKeyReleased);
#endif
}

//--------------------------------------------------------------
void ofxUITabbedCanvas::add(ofxUICanvas *canvas)
{
    canvas->disableAppEventCallbacks();
    canvas->disableMouseEventCallbacks();
    canvas->disableKeyEventCallbacks();
    mCanvases.push_back(canvas);
    select(mCurrentTab);
    
    const float w = canvas->getRect()->getWidth();
    if (w>mTabSize.width) {
        mTabSize.width = w;
    }
    
    resize();
    
    rebuildTabs();
}

//--------------------------------------------------------------
void ofxUITabbedCanvas::remove(ofxUICanvas *canvas)
{
    for (int i=0; i<mCanvases.size(); i++) {
        if (mCanvases.at(i)) {
            delete mCanvases.at(i);
            mCanvases.erase(std::remove(mCanvases.begin(), mCanvases.end(), canvas), mCanvases.end());
        }
    }
}

//--------------------------------------------------------------
void ofxUITabbedCanvas::remove(int index)
{
    remove(mCanvases.at(index));
}

//--------------------------------------------------------------
void ofxUITabbedCanvas::clear()
{
    for (int i=0; i<mCanvases.size(); i++) {
        if (mCanvases.at(i)) {
            delete mCanvases.at(i);
            mCanvases.at(i) = NULL;
        }
    }
    mCanvases.clear();
}

//--------------------------------------------------------------
void ofxUITabbedCanvas::select(int index)
{
    mCurrentTab = index;
    
    for (int i=0; i<size(); i++) {
        if (i==mCurrentTab) {
            at(i)->enableKeyEventCallbacks();
        }
        else {
            at(i)->disableKeyEventCallbacks();
        }
    }
}

//--------------------------------------------------------------
void ofxUITabbedCanvas::toggleVisible()
{
    mVisible ^= true;
}

//--------------------------------------------------------------
void ofxUITabbedCanvas::setVisible(bool bShow)
{
    mVisible = bShow;
}

//--------------------------------------------------------------
ofxUICanvas *ofxUITabbedCanvas::operator [] (int index)
{
    return at(index);
}

//--------------------------------------------------------------
ofxUICanvas *ofxUITabbedCanvas::at(int index)
{
    return mCanvases.at(index);
}

//--------------------------------------------------------------
int ofxUITabbedCanvas::size() const
{
    return mCanvases.size();
}

//--------------------------------------------------------------
void ofxUITabbedCanvas::loadSettings(const string &fileName)
{
    string baseName = fileName;
    ofStringReplace(baseName, "XML", "xml");
    
    for (int i=0; i<size(); i++) {
        string file = baseName;
        ofStringReplace(file, ".xml", ofToString(i)+".xml");
        at(i)->loadSettings(file);
    }
}

//--------------------------------------------------------------
void ofxUITabbedCanvas::saveSettings(const string &fileName)
{
    string baseName = fileName;
    ofStringReplace(baseName, "XML", "xml");
    
    for (int i=0; i<size(); i++) {
        string file = baseName;
        ofStringReplace(file, ".xml", ofToString(i)+".xml");
        at(i)->saveSettings(file);
    }
}

//--------------------------------------------------------------
void ofxUITabbedCanvas::onTabChanged(ofxUIEventArgs &e)
{
    const string name = e.widget->getName();
	const int kind = e.widget->getKind();
    
    if (kind == OFX_UI_WIDGET_TOGGLE) {
        ofxUIToggle *t = static_cast<ofxUIToggle *>(e.widget);
        if (t->getValue()) {
            vector<string> values = ofSplitString(name, ",");
            if (values.size()>1) {
                //cout << ofToInt(values.at(1)) << endl;
                const int tabIndex = ofToInt(values.at(1));
                select(tabIndex);
            }
        }
    }
}

//--------------------------------------------------------------
void ofxUITabbedCanvas::rebuildTabs()
{
    if (mTabs) {
        ofRemoveListener(mTabs->newGUIEvent, this, &ofxUITabbedCanvas::onTabChanged);
        delete mTabs;
        mTabs = NULL;
    }
    
    const float mgn = 2.0f;
    
    mTabs = new ofxUICanvas(0.0f, 0.0f, mTabSize.width, mTabSize.height);
    mTabs->setWidgetSpacing(mgn);
    
    if (!mCanvases.empty()) {
        const int nTabs = mCanvases.size();
        const float tabW = mTabSize.width/mCanvases.size()-mgn*2.0f;
        const float tabH = mTabSize.height-mgn*2.0f;
        
        ofxUIWidget *w = mTabs->addWidgetDown(new ofxUIToggleMatrix(tabW, tabH, 1, nTabs, ""));
        ofAddListener(mTabs->newGUIEvent, this, &ofxUITabbedCanvas::onTabChanged);
        mTabsMatrix = static_cast<ofxUIToggleMatrix *>(w);
        mTabsMatrix->setAllowMultiple(false);
        mTabsMatrix->setToggle(mCurrentTab, 0, true);
    }
}