#include "testApp.h"
#include "ofAppGlutWindow.h"

//--------------------------------------------------------------
int main()
{
	ofAppGlutWindow window; // create a window
    window.displayString = "rgba double depth alpha samples>=8 stencil";
	// set width, height, mode (OF_WINDOW or OF_FULLSCREEN)
	ofSetupOpenGL(&window, 1920, 1080, OF_WINDOW);
	ofRunApp(new testApp); // start the app
}