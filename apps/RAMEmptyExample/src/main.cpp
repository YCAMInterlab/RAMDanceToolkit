// 
// main.cpp - RAMDanceToolkit
// 
// Copyright 2012-2013 YCAM InterLab, Yoshito Onishi, Satoru Higa, Motoi Shimizu, and Kyle McDonald
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//    http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "ofApp.h"
#include "ofAppGlutWindow.h"

//--------------------------------------------------------------
int main()
{
	ofAppGlutWindow window; // create a window

#ifdef TARGET_OSX
	window.setGlutDisplayString("rgba double samples>=4 depth stencil");
#endif

	// these values are only for initialization, actual settings are in ramControlPanel
	ofSetupOpenGL(&window, 1280, 720, OF_WINDOW);
	ofRunApp(new ofApp); // start the app
}
