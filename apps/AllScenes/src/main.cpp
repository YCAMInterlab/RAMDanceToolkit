#include "testApp.h"
#include "ofAppGlutWindow.h"

//--------------------------------------------------------------
int main()
{
	ofAppGlutWindow window; // create a window

#ifdef TARGET_MAC
	window.setGlutDisplayString("rgba double samples>=4 depth stencil");
#endif

	// set width, height, mode (OF_WINDOW or OF_FULLSCREEN)
	ofSetupOpenGL(&window, 1024, 1024, OF_WINDOW);
	ofRunApp(new testApp); // start the app
}