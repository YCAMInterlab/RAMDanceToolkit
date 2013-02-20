local project_name = _ARGS[1]

local main_cpp = [[
#include "testApp.h"
#include "ofAppGlutWindow.h"

//--------------------------------------------------------------
int main()
{
	ofAppGlutWindow window; // create a window

	// set width, height, mode (OF_WINDOW or OF_FULLSCREEN)
	ofSetupOpenGL(&window, 1024, 768, OF_WINDOW);
	ofRunApp(new testApp); // start the app
}
]]

local test_app_h = [[
#pragma once

#include "ofMain.h"
#include "ramMain.h"

class testApp : public ramBaseApp
{
public:

	// of methods
	// ------------------------
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	// ram methods
	// ------------------------
	void drawActor(ramActor &actor);
	void drawRigid(ramRigidBody &rigid);
};
]]

local test_app_cpp = [[
#include "testApp.h"

#pragma mark - oF methods
//--------------------------------------------------------------
void testApp::setup()
{
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	ofBackground(ramColor::WHITE);


	/// ram setup
	// ------------------
	ramInitialize(10000);
}

//--------------------------------------------------------------
void testApp::update()
{
	
}

//--------------------------------------------------------------
void testApp::draw()
{
}

#pragma mark - ram methods
//--------------------------------------------------------------
void testApp::drawActor(ramActor &actor)
{
}

//--------------------------------------------------------------
void testApp::drawRigid(ramRigidBody &rigid)
{
}


#pragma mark - oF Events
//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
}

//--------------------------------------------------------------
void testApp::keyReleased(int key)
{
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y)
{
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button)
{
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h)
{
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg)
{
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo)
{
}
]]

function create_file( path, content )
	local fp = io.open(path, 'w')
	fp:write(content)
	fp:close()
end

solution (project_name)
	language "C++"

	configurations { "Debug", "Release" }
	platforms { "x32" }

	project (project_name)
		kind "WindowedApp"

		targetname( project_name )
		location ( project_name )

		includedirs {
	   		-- RAM
	   		"../libs/ram/",
	   		"../libs/ram/**",

	   		-- RAM addons
	   		'../addons/ofxBt/src/',
	   		'../addons/ofxBt/src/**',
	   		'../addons/ofxBt/libs/',
	   		'../addons/ofxBt/libs/**',
	   		'../addons/ofxInteractivePrimitives/src/',
	   		'../addons/ofxInteractivePrimitives/src/**',
	   		'../addons/ofxUI/src/',
	   		'../addons/ofxUITabbedCanvas/',

	   		-- oF
	   		'../../libs/**/include',
	   		'../../libs/cairo/include/**',
	   		'../../libs/openFrameworks',
	   		'../../libs/openFrameworks/**',

	   		-- oF addons
	   		'../../addons/ofxOsc/**',
	   		'../../addons/ofxXMLSettings/**',
	   	}

	   	-- excludes { "**/Win32Specific/**" }
	   	-- libdirs { os.findlib("X11") }
	   	-- links { "Cocoa.framework" }
	   	-- libdirs { "libs", "../mylibs" }

	   	libdirs {
		   	-- RAM
	   		'../libs/project/',
		}

		configuration 'macosx'
			links {
				-- RAM
				'ram',

				-- oF
				'Accelerate.framework',
				'AGL.framework',
				'ApplicationServices.framework',
				'AudioToolbox.framework',
				'Carbon.framework',
				'CoreAudio.framework',
				'CoreFoundation.framework',
				'CoreServices.framework',
				'CoreVideo.framework',
				'OpenGL.framework',
				'QuickTime.framework',
				'QTKit.framework',
			}

			linkoptions {
				-- RAM
				'../../libs/project/libram.a',
				'../../addons/ofxBt/libs/bullet/lib/osx/libBulletCollision.a',
				'../../addons/ofxBt/libs/bullet/lib/osx/libBulletDynamics.a',
				'../../addons/ofxBt/libs/bullet/lib/osx/libBulletSoftBody.a',
				'../../addons/ofxBt/libs/bullet/lib/osx/libLinearMath.a',

				-- oF
				'../../../libs/fmodex/lib/osx/libfmodex.dylib',
				'../../../libs/glut/lib/osx/GLUT.framework/Versions/A/GLUT',
				'../../../libs/assimp/lib/osx/assimp.a',
				'../../../libs/cairo/lib/osx/cairo-script-interpreter.a',
				'../../../libs/cairo/lib/osx/cairo.a',
				'../../../libs/cairo/lib/osx/pixman-1.a',
				'../../../libs/FreeImage/lib/osx/freeimage.a',
				'../../../libs/freetype/lib/osx/freetype.a',
				'../../../libs/glew/lib/osx/glew.a',
				'../../../libs/openFrameworksCompiled/lib/osx/openFrameworks.a',
				'../../../libs/poco/lib/osx/PocoCrypto.a',
				'../../../libs/poco/lib/osx/PocoData.a',
				'../../../libs/poco/lib/osx/PocoDataODBC.a',
				'../../../libs/poco/lib/osx/PocoDataSQLite.a',
				'../../../libs/poco/lib/osx/PocoFoundation.a',
				'../../../libs/poco/lib/osx/PocoNet.a',
				'../../../libs/poco/lib/osx/PocoNetSSL.a',
				'../../../libs/poco/lib/osx/PocoUtil.a',
				'../../../libs/poco/lib/osx/PocoXML.a',
				'../../../libs/poco/lib/osx/PocoZip.a',
				'../../../libs/rtAudio/lib/osx/rtAudio.a',
				'../../../libs/tess2/lib/osx/tess2.a',
			}

		configuration {}

	   	create_file(project_name .. '/src/main.cpp', main_cpp)
	   	create_file(project_name .. '/src/testApp.h', test_app_h)
	   	create_file(project_name .. '/src/testApp.cpp', test_app_cpp)

	   	files {
	   		project_name .. '/src/main.cpp',
	   		project_name .. '/src/testApp.h',
	   		project_name .. '/src/testApp.cpp'
	   	}

