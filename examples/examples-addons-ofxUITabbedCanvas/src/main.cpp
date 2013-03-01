#include "ofAppGlutWindow.h"
#include "ofMain.h"

#include "ofxUI.h"

class ofxUITab : public ofxUICanvas {
protected:
	string tabName;
	bool visible;
public:
	ofxUITab(string tabName)
	:tabName(tabName)
	,visible(false)
	{
		addLabel(tabName);
		addSpacer();
	}
	void setTabName(const string& tabName) {this->tabName = tabName;}
	string getTabName() const {return tabName;}
	bool& getVisible() {return visible;}
};

class ofxUITabbedCanvas : public ofxUICanvas {
protected:
	int currentTab;
	float tabWidth;
	bool visible;
	vector<ofxUITab*> tabs;
	vector<ofxUILabelToggle*> buttons;
public:
	ofxUITabbedCanvas()
	:currentTab(0)
	,tabWidth(100)
	,visible(true) {
	}
	void add(ofxUITab* canvas) {
		canvas->disableAppEventCallbacks();
		canvas->disableMouseEventCallbacks();
		canvas->disableKeyEventCallbacks();
		canvas->disableWindowEventCallbacks();
		if(tabs.empty()) {
			canvas->getVisible() = true;
		}
		tabs.push_back(canvas);
		ofxUILabelToggle* button = new ofxUILabelToggle(canvas->getTabName(), canvas->getVisible(), tabWidth, 0, 0, 0, OFX_UI_FONT_SMALL, true);
        addWidgetPosition(button);
		buttons.push_back(button);
		autoSizeToFitWidgets();
	}
	void select(string name) {
		for(int i = 0; i < buttons.size(); i++) {
			ofxUILabelToggle *button = buttons[i];		
			if(button->getName() == name) {
				button->setValue(true);
				currentTab = i;
			} else {
				button->setValue(false); 
			}			
		}
	}
	void triggerEvent(ofxUIWidget *child) {
		select(child->getName());
		ofxUICanvas::triggerEvent(child);
	} 
	ofxUITab* getCurrent() {
		return tabs[currentTab];
	}
	void update() {
		if (!visible || tabs.empty()) return;
		getCurrent()->update();
	}
	void draw() {
		glDisable(GL_DEPTH_TEST);
        glDisable(GL_LIGHTING);
		
        ofPushStyle();
        ofPushMatrix();
        ofNoFill();
        ofTranslate(getRect()->width, 0);
        tabs[currentTab]->draw();
        ofPopMatrix();
        ofPopStyle();
		
		ofxUICanvas::draw();
	}
	void keyPressed(int key) {
		if (!visible) return;
		ofxUICanvas::keyPressed(key);
		if (tabs.empty()) return;
		getCurrent()->keyPressed(key);
	}
	void keyReleased(int key) {
		if (!visible) return;
		ofxUICanvas::keyReleased(key);
		if (tabs.empty()) return;
		getCurrent()->keyReleased(key);
	}
	void mouseMoved(int x, int y) {
		if (!visible) return;
		ofxUICanvas::mouseMoved(x, y);
		if (tabs.empty()) return;
		getCurrent()->mouseMoved(x - getRect()->width, y);
	}
	void mouseDragged(int x, int y, int button) {
		if (!visible) return;
		ofxUICanvas::mouseDragged(x, y, button);
		if (tabs.empty()) return;
		getCurrent()->mouseDragged(x - getRect()->width, y, button);
	}
	void mousePressed(int x, int y, int button) {
		if (!visible) return;
		ofxUICanvas::mousePressed(x, y, button);
		if (tabs.empty()) return;
		getCurrent()->mousePressed(x - getRect()->width, y, button);
	}	
	void mouseReleased(int x, int y, int button) {
		if (!visible) return;
		ofxUICanvas::mouseReleased(x, y, button);
		if (tabs.empty()) return;
		getCurrent()->mouseReleased(x - getRect()->width, y, button);
	}	
	
};


class ofApp : public ofBaseApp {
public:
	
	ofxUITabbedCanvas* tabbedCanvas;
	vector<ofxUITab*> tabs;
	
	float radius, red, green, blue, alpha;
	ofPoint position;
	bool drawFill;
	
	void setup() {
		ofSetVerticalSync(true);
		ofSetFrameRate(120);

		tabbedCanvas = new ofxUITabbedCanvas();
		
		for(int i = 0; i < 24; i++) {
			string title = "Tab " + ofToString(i);
			if(i == 0) title = "RAM Dance Toolkit";
			ofxUITab* tab = new ofxUITab(title);
			tabs.push_back(tab);
			switch(i % 3) {
				case 0:
					tab->addFPSSlider("FPS");
					tab->addSpacer();
					tab->addSlider("RADIUS", 0.0, 255.0, &radius);
					tab->addSlider("RED", 0.0, 255.0, &red);
					tab->addSlider("GREEN", 0.0, 255.0, &green);
					tab->addSlider("BLUE", 0.0, 255.0, &blue);
					tab->addSlider("ALPHA", 0.0, 255.0, &alpha);
					break;
				case 1:
					tab->addFPSSlider("FPS");
					tab->addSpacer();
					tab->add2DPad("CENTER", ofPoint(0,ofGetWidth()), ofPoint(0, ofGetHeight()), &position);
					tab->addLabelToggle("DRAWFILL", &drawFill);
					break;
				case 2:
					tab->addFPSSlider("FPS");
					break;
			}
			tab->autoSizeToFitWidgets();
			tabbedCanvas->add(tab);
			
			if(i == 3) {
				tabbedCanvas->addSpacer();
			}
		}
	}
	
	void update() {
	}
	
	void draw() {
		ofBackground(128);
	}
};

int main() {
	ofAppGlutWindow window;
	ofSetupOpenGL(&window, 1280, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}
