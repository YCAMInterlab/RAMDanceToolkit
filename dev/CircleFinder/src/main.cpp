#include "testApp.h"
#include "ofAppGlutWindow.h"

int main() {
	ofAppGlutWindow window;
	//window.setGlutDisplayString("rgba double samples>=8 depth");
	ofSetupOpenGL(&window, 1280, 480, OF_WINDOW);
	ofRunApp(new testApp());
}
