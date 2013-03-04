#include "testApp.h"
#include "ofAppGlutWindow.h"

//--------------------------------------------------------------
int main()
{
	ofAppGlutWindow window; // create a window

#ifdef TARGET_OSX
	window.setGlutDisplayString("rgba double samples>=4 depth stencil");
#endif

	// these values are only for initialization, actual settings are in ramControlPanel
	ofSetupOpenGL(&window, 1024, 1024, OF_WINDOW);
	ofRunApp(new testApp); // start the app
}