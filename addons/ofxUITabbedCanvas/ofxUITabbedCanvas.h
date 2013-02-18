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
class ofxUITabbedCanvas : public ofxUICanvas {
    
    typedef ofxUICanvas super; /// for readableness
    
public:
    //--------------------
    ofxUITabbedCanvas();
    virtual ~ofxUITabbedCanvas();
    
    /// callbacks
    //--------------------
    /// app events
    void setup(const string &fontFile);
    void update();
    void draw();
    void exit();
    /// key events
    void keyPressed(int key);
	void keyReleased(int key);
    /// mouse events
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    
    /// setters
    //--------------------
    void setPosition(const ofVec2f &pos);
    void setPosition(float x, float y);
    
    /// getters
    //--------------------
    ofVec2f getPosition() const;
    
    //--------------------
    void resize();
    
    /// tab management
    //--------------------
    void add(ofxUICanvas *aTabCanvas);
    void remove(ofxUICanvas *aTabCanvas);
    void remove(int index);
    void clear();
    
    ofxUICanvas *at(int index);
    ofxUICanvas *operator [] (int index);
    /// num of tabs
    int size() const;
    
    //--------------------
    void select(int index);
    
    /// utils
    //--------------------
    /// show/hide ofxUITabbedCanvas
    void toggleVisible();
    void setVisible(bool bShow);

    /// setting
    //--------------------
    /// load xml files for each tabs
    void loadSettings(const string &fileName);
    /// generate and save xml files for each tabs
    void saveSettings(const string &fileName);
    
private:
    /// internal utils
    //--------------------
    void onTabChanged(ofxUIEventArgs &e);
    
    void rebuildTabs();
    
    /// for dragging
    //--------------------
    enum MouseState {
        MOUSE_IDLE,
        MOUSE_OVER,
        MOUSE_DOWN,
    };
    
    //--------------------
    ofxUICanvas             *mTabs;
    ofxUIToggleMatrix       *mTabsMatrix;
    vector<ofxUICanvas *>   mCanvases;
    int         mCurrentTab;
    
    //--------------------
    bool        mVisible;
    
    string      mFontFile;
    
    //--------------------
    float       mTabButtonSize;
    float       mDraggBarHeight;
    
    /// position, size
    //--------------------
    ofVec2f     mPosition;
    ofRectangle mDraggableRect;
    ofRectangle mTabSize;
    
    //--------------------
    int     mMouseState;
    ofVec2f mDraggOrigin;
    float   mTabSpacing;

};


#endif /* defined(__ofxUISimpleExample__ofxUITabbedCanvas__) */
