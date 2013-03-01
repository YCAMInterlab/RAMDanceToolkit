#include "ofAppGlutWindow.h"
#include "ofMain.h"

#include "ofxUITabbedCanvas.h"

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
		ofSetWindowTitle("RAM Dance Toolkit");

		tabbedCanvas = new ofxUITabbedCanvas();
		
		int n = 24;
		string titles[] = {
			"Presets",
			"Preferences",
			"Playback",
			"Actors",
			"Basic actor",
			"Draw lines",
			"Big box",
			"Future",
			"Donuts",
			"Stamp",
			"Expansion",
			"Particles",
			"Abacus",
			"Sound cube",
			"Upside down",
			"Kepler",
			"Hasty chase",
			"Color grid",
			"Three points",
			"Four points",
			"Chain",
			"Monster",
			"Laban",
			"Notation"
		};
		
		for(int i = 0; i < n; i++) {
			ofxUITab* tab = new ofxUITab(titles[i], i > 3);
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
