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
    typedef ofxUICanvas super;
public:
    ofxUITabbedCanvas();
    virtual ~ofxUITabbedCanvas();
    
    void setup(const string &fontFile);
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
    
    void setPosition(const ofVec2f &pos);
    void setPosition(float x, float y);
    ofVec2f getPosition() const;
    
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
    void onTabChanged(ofxUIEventArgs &e);
    
    void rebuildTabs();
    
    ofxUICanvas *mTabs;
    ofxUIToggleMatrix *mTabsMatrix;
    vector<ofxUICanvas *> mCanvases;
    int mCurrentTab;
    bool mVisible;
    ofRectangle mTabSize;
    string mFontFile;
    
    ofVec2f mPosition;
};


#endif /* defined(__ofxUISimpleExample__ofxUITabbedCanvas__) */
