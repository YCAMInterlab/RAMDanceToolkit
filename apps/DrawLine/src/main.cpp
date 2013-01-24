#include "testApp.h"
#include "ofAppGlutWindow.h"

//--------------------------------------------------------------
int main()
{
	ofAppGlutWindow window; // create a window
	window.setGlutDisplayString("rgba double samples>=8 depth stencil");
	// set width, height, mode (OF_WINDOW or OF_FULLSCREEN)
	ofSetupOpenGL(&window, 1024, 1200, OF_WINDOW);
	ofRunApp(new testApp); // start the app
}