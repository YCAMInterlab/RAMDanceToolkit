//
//  ofxUITabbedCanvas.h
//  ofxUISimpleExample
//
//  Created by Onishi Yoshito on 1/29/13.
//
//

#ifndef __ofxUISimpleExample__ofxUITabbedCanvas__
#define __ofxUISimpleExample__ofxUITabbedCanvas__

#include "ofMain.h"
#include "ofxUI.h"

//--------------------------------------------------------------
class ofxUITabbedCanvas {
public:
    ofxUITabbedCanvas();
    virtual ~ofxUITabbedCanvas();
    
    void update();
    void draw();
    void exit();
    
    void keyPressed(int key);
	void keyReleased(int key);
    
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    
    void resize();
    
    void enableAppEventCallbacks();
    void disableAppEventCallbacks();
    
    void enableMouseEventCallbacks();
    void disableMouseEventCallbacks();
    
    void enableKeyEventCallbacks();
    void disableKeyEventCallbacks();
    
    void add(ofxUICanvas *canvas);
    void remove(ofxUICanvas *canvas);
    void remove(int index);
    void clear();
    
    void select(int index);
    
    void toggleVisible();
    void setVisible(bool bShow);
    
    ofxUICanvas *operator [] (int index);
    ofxUICanvas *at(int index);
    int size() const;
    
    void loadSettings(const string &fileName);
    void saveSettings(const string &fileName);
    
private:
    inline void onUpdate(ofEventArgs &) { update(); }
    inline void onDraw(ofEventArgs &) { draw(); }
    inline void onExit(ofEventArgs &) { exit(); }
    
    inline void onKeyPressed(ofKeyEventArgs& e) { keyPressed(e.key); }
    inline void onKeyReleased(ofKeyEventArgs& e) { keyReleased(e.key); }
    
    inline void onMouseReleased(ofMouseEventArgs& e) { mouseReleased(e.x, e.y, e.button); }
    inline void onMousePressed(ofMouseEventArgs& e)  { mousePressed(e.x, e.y, e.button); }
    inline void onMouseMoved(ofMouseEventArgs& e)  { mouseMoved(e.x, e.y); }
    inline void onMouseDragged(ofMouseEventArgs& e)  { mouseDragged(e.x, e.y, e.button); }
    
    void onTabChanged(ofxUIEventArgs &e);
    
    void rebuildTabs();
    
    ofxUICanvas *mTabs;
    ofxUIToggleMatrix *mTabsMatrix;
    vector<ofxUICanvas *> mCanvases;
    int mCurrentTab;
    bool mVisible;
    ofRectangle mTabSize;
};


#endif /* defined(__ofxUISimpleExample__ofxUITabbedCanvas__) */
