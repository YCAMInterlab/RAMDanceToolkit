#include "testApp.h"
#include "ofAppGlutWindow.h"

int main() {
	ofAppGlutWindow window;
	window.setGlutDisplayString("rgba double samples>=8 depth");
	ofSetupOpenGL(&window, 1680, 1050, OF_FULLSCREEN);
	ofRunApp(new testApp());
}
