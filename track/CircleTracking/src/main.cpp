#include "testApp.h"
#include "ofAppGlutWindow.h"

int main() {
	ofAppGlutWindow window;
	ofSetupOpenGL(&window, 1680, 1050, OF_FULLSCREEN);
	ofRunApp(new testApp());
}
