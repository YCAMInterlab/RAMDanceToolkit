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
mTabsMatrix(NULL),
mFontFile(""),
mDraggBarHeight(10.0f),
mTabButtonSize(14.0f),
mMouseState(MOUSE_IDLE),
mTabSpacing(2.0f)
{
    /// tabs
    mCanvases.clear();
    
    /// enable all event callbacks
    super::enableAppEventCallbacks();
    super::enableMouseEventCallbacks();
    super::enableKeyEventCallbacks();
    super::disableWindowEventCallbacks();
    
    mTabSize.set(0.0f, mDraggBarHeight, 1.0f, mTabButtonSize); /// default size
    super::getRect()->set(mTabSize);
}

//--------------------------------------------------------------
ofxUITabbedCanvas::~ofxUITabbedCanvas()
{
    super::disableAppEventCallbacks();
    
    /// delete all objects
    clear();
}

//--------------------------------------------------------------
void ofxUITabbedCanvas::setup(const string &fontFile)
{
    /// set ofxUI font
    mFontFile = fontFile;
}

//--------------------------------------------------------------
void ofxUITabbedCanvas::update()
{    
    if (mCanvases.empty()) return;
    
    at(mCurrentTab)->update();
}

//--------------------------------------------------------------
void ofxUITabbedCanvas::draw()
{
    if (mVisible && !mCanvases.empty()) {
        
        /// like ofxUI implementation
        /// draw tab's frme
        ofPushMatrix();
        ofPushStyle();
		glDisable(GL_DEPTH_TEST);
        glDisable(GL_LIGHTING);
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);
        ofSetRectMode(OF_RECTMODE_CORNER);
        ofSetLineWidth(1.0);
        
        ofTranslate(mPosition);
        
        drawPadded();
        drawPaddedOutline();
        drawBack();
        drawFill();
        drawFillHighlight();
        drawOutline();
        drawOutlineHighlight();
        
		glDisable(GL_DEPTH_TEST);
        ofPopStyle();
        ofPopMatrix();
        
        /// draw draggable toolbar
        ofPushStyle();
        ofPushMatrix();
        ofNoFill();
        (mMouseState==MOUSE_IDLE) ? ofSetColor(getColorOutline()) : ofSetColor(getColorOutlineHighlight());
        ofRect(mDraggableRect);
        
        ofTranslate(mPosition);
        ofTranslate(0.0f, mTabSize.height+mDraggableRect.height);
        at(mCurrentTab)->draw();
        ofPopMatrix();
        ofPopStyle();
        
    }
}

//--------------------------------------------------------------
void ofxUITabbedCanvas::exit()
{
    for (int i=0; i<mCanvases.size(); i++)
        at(i)->exit();
}

//--------------------------------------------------------------
void ofxUITabbedCanvas::keyPressed(int key)
{
    if (!mVisible || mCanvases.empty()) return;
    
    at(mCurrentTab)->keyPressed(key);
}

//--------------------------------------------------------------
void ofxUITabbedCanvas::keyReleased(int key)
{
    if (!mVisible || mCanvases.empty()) return;
    
    at(mCurrentTab)->keyReleased(key);
}

//--------------------------------------------------------------
void ofxUITabbedCanvas::mouseMoved(int x, int y)
{
    (mDraggableRect.inside(x, y)) ? mMouseState = MOUSE_OVER : mMouseState = MOUSE_IDLE;
    
    if (!mVisible || mCanvases.empty()) return;
    
    at(mCurrentTab)->mouseMoved(x-mPosition.x, y-mTabSize.height-mPosition.y-mDraggableRect.height);
}

//--------------------------------------------------------------
void ofxUITabbedCanvas::mouseDragged(int x, int y, int button)
{
    if (mMouseState == MOUSE_DOWN)
        setPosition(ofVec2f(x-mDraggOrigin.x, y-mDraggOrigin.y));
    
    if (!mVisible || mCanvases.empty()) return;
    
    at(mCurrentTab)->mouseDragged(x-mPosition.x, y-mTabSize.height-mPosition.y-mDraggableRect.height, button);
}

//--------------------------------------------------------------
void ofxUITabbedCanvas::mousePressed(int x, int y, int button)
{
    if (mMouseState == MOUSE_OVER && mDraggableRect.inside(x, y)) {
        mMouseState = MOUSE_DOWN;
        mDraggOrigin.set(x-mDraggableRect.x, y-mDraggableRect.y);
    }
    else {
        mMouseState = MOUSE_IDLE;
    }
    
    if (!mVisible || mCanvases.empty()) return;
    
    at(mCurrentTab)->mousePressed(x-mPosition.x, y-mTabSize.height-mPosition.y-mDraggableRect.height, button);
}

//--------------------------------------------------------------
void ofxUITabbedCanvas::mouseReleased(int x, int y, int button)
{
    (mDraggableRect.inside(x, y)) ? mMouseState = MOUSE_OVER : mMouseState = MOUSE_IDLE;
    
    if (!mVisible || mCanvases.empty()) return;
    
    at(mCurrentTab)->mouseReleased(x-mPosition.x, y-mTabSize.height-mPosition.y-mDraggableRect.height, button);
}

//--------------------------------------------------------------
void ofxUITabbedCanvas::setPosition(const ofVec2f &pos)
{
    if (mTabs) {
        mPosition = pos;
        if (mPosition.y<0.0f)
            mPosition.y = 0.0f;
        mTabs->getRect()->setPosition(mPosition);
        mDraggableRect.setPosition(ofVec2f(mPosition.x, mPosition.y));
    }
}

//--------------------------------------------------------------
void ofxUITabbedCanvas::setPosition(float x, float y)
{
    setPosition(ofVec2f(x, y));
}

//--------------------------------------------------------------
ofVec2f ofxUITabbedCanvas::getPosition() const
{
    return mPosition;
}

//--------------------------------------------------------------
void ofxUITabbedCanvas::resize()
{
    if (mTabs)
        mTabs->getRect()->setWidth(mTabSize.width);
    
    super::getRect()->set(mTabSize);
    
    for (int i=0; i<size(); i++) {
        at(i)->getRect()->setWidth(mTabSize.width);
    }
    
    mDraggableRect.setWidth(mTabSize.width);
    mDraggableRect.setHeight(mDraggBarHeight);
}

//--------------------------------------------------------------
void ofxUITabbedCanvas::add(ofxUICanvas *aTabCanvas)
{
    /// desable child tab canvas events
    /// it will be handled by ofxUITabbedCanvas
    aTabCanvas->disableAppEventCallbacks();
    aTabCanvas->disableMouseEventCallbacks();
    aTabCanvas->disableKeyEventCallbacks();
    aTabCanvas->disableWindowEventCallbacks();
    
    /// tabs
    mCanvases.push_back(aTabCanvas);
    
    select(mCurrentTab);
    
    /// make ofxUITabbedCanvas bigger if it is smaller than child canvas
    const float w = aTabCanvas->getRect()->getWidth();
    if (w>mTabSize.width)
        mTabSize.width = w;
    resize();
    
    /// delete all tabs and rebuild them
    rebuildTabs();
}

//--------------------------------------------------------------
void ofxUITabbedCanvas::remove(ofxUICanvas *aTabCanvas)
{
    for (int i=0; i<mCanvases.size(); i++) {
        if (mCanvases.at(i)) {
            delete mCanvases.at(i);
            mCanvases.erase(std::remove(mCanvases.begin(), mCanvases.end(), aTabCanvas), mCanvases.end());
        }
    }
    
    if (mCanvases.empty()) {
        mCurrentTab = 0;
    }
    else if (mCurrentTab>=mCanvases.size()) {
        mCurrentTab = mCanvases.size()-1;
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
    mCurrentTab = 0;
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
    /// (0, 1) -> (row, col)
    const string name = e.widget->getName();
    /// OFX_UI_WIDGET_TOGGLE
	const int kind = e.widget->getKind();
    
    if (kind == OFX_UI_WIDGET_TOGGLE) {
        ofxUIToggle *t = static_cast<ofxUIToggle *>(e.widget);
        if (t->getValue()) {
            /// get current active tab index from ofxUIToggleMatrix column at 0
            vector<string> values = ofSplitString(name, ",");
            if (values.size()>1) {
                //cout << ofToInt(values.at(1)) << endl;
                /// remove '(' and ')'
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
        
    mTabs = new ofxUICanvas(0.0f, 0.0f, mTabSize.width, mTabSize.height);
    mTabs->setWidgetSpacing(mTabSpacing);
    mTabs->setFont(mFontFile, false);
    super::addWidget(mTabs);
    
    if (!mCanvases.empty()) {
        const int nTabs = mCanvases.size();
        const float tabW = mTabSize.width/mCanvases.size()-mTabSpacing*2.0f;
        const float tabH = mTabSize.height-mTabSpacing*2.0f;
        
        ofxUIWidget *w = mTabs->addWidgetDown(new ofxUIToggleMatrix(tabW, tabH, 1, nTabs, ""));
        ofAddListener(mTabs->newGUIEvent, this, &ofxUITabbedCanvas::onTabChanged);
        mTabsMatrix = static_cast<ofxUIToggleMatrix *>(w);
        mTabsMatrix->setAllowMultiple(false);
        mTabsMatrix->setToggle(mCurrentTab, 0, true);
        vector<ofxUIToggle *> toggles = mTabsMatrix->getToggles();
        
        for (int i=0; i<toggles.size(); i++) {
            toggles.at(i)->setDrawBack(true);
            toggles.at(i)->setDrawOutline(true);
            toggles.at(i)->setColorFill(super::getColorFill());
            toggles.at(i)->setColorBack(super::getColorBack());
            toggles.at(i)->setColorFillHighlight(super::getColorBack());
            toggles.at(i)->setColorOutline(super::getColorOutline());
            toggles.at(i)->setColorOutlineHighlight(super::getColorOutlineHighlight());
            toggles.at(i)->setColorPadded(super::getColorPadded());
            toggles.at(i)->setColorPaddedOutline(super::getColorOutline());
        }
    }
}



