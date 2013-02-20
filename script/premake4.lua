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

	location ( project_name )

	configuration "Debug"
		targetname( project_name .. "Debug" )

	configuration "Release"
		targetname( project_name )

	configurations { "Debug", "Release" }
	platforms { "x32" }

	project (project_name)

		targetdir( project_name .. '/bin' )

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

		-- empty sorcecode
		create_file(project_name .. '/src/main.cpp', main_cpp)
		create_file(project_name .. '/src/testApp.h', test_app_h)
		create_file(project_name .. '/src/testApp.cpp', test_app_cpp)

		files {
			'../../libs/openFrameworks/*.h',
	   		'../../libs/openFrameworks/**/*.h',

			'../libs/ram/*.h',
			'../libs/ram/**/*.h',
			'../libs/ram/**/*.cpp',

			'../addons/ofxBt/src/*.h',
			'../addons/ofxBt/src/*.cpp',

			'../addons/ofxInteractivePrimitives/src/**.h',
			'../addons/ofxInteractivePrimitives/src/**.cpp',

			'../addons/ofxUI/src/**.h',

			'../addons/ofxUITabbedCanvas/*.h',
			'../addons/ofxUITabbedCanvas/*.cpp',

			'../../addons/ofxOsc/**/*.h',
			'../../addons/ofxOsc/**/*.cpp',

			'../../addons/ofxXmlSettings/**/*.h',
			'../../addons/ofxXmlSettings/**/*.cpp',

			project_name .. '/src/main.cpp',
			project_name .. '/src/testApp.h',
			project_name .. '/src/testApp.cpp'
		}

		-- platform specific

		configuration 'macosx'
			kind "WindowedApp"

			links {
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

			postbuildcommands { 'cp -f ../../../libs/fmodex/lib/osx/libfmodex.dylib "$TARGET_BUILD_DIR/$PRODUCT_NAME.app/Contents/MacOS/libfmodex.dylib"; install_name_tool -change ./libfmodex.dylib @executable_path/libfmodex.dylib "$TARGET_BUILD_DIR/$PRODUCT_NAME.app/Contents/MacOS/$PRODUCT_NAME"; mkdir -p "$TARGET_BUILD_DIR/$PRODUCT_NAME.app/Contents/"; cp -Rf ../../../libs/glut/lib/osx/GLUT.framework "$TARGET_BUILD_DIR/$PRODUCT_NAME.app/Contents/Frameworks"; mkdir -p "$TARGET_BUILD_DIR/data/";' }

		configuration 'vs*'
			kind "ConsoleApp"

			libdirs {
				-- RAM
				'../addons/ofxBt/libs/bullet/lib/vs2010/',

				-- oF
				'../../libs/assimp/lib/vs2010',
				'../../libs/cairo/lib/vs2010',
				'../../libs/fmodex/lib/vs2010',
				'../../libs/FreeImage/lib/vs2010',
				'../../libs/freetype/lib/vs2010',
				'../../libs/glew/lib/vs2010',
				'../../libs/glu/lib/vs2010',
				'../../libs/glut/lib/vs2010',
				'../../libs/openFrameworks/lib/vs2010',
				'../../libs/openFrameworksCompiled/lib/vs2010',
				'../../libs/poco/lib/vs2010',
				'../../libs/quicktime/lib/vs2010',
				'../../libs/rtAudio/lib/vs2010',
				'../../libs/tess2/lib/vs2010',
				'../../libs/videoInput/lib/vs2010',
			}

			linkoptions {
				'/NODEFAULTLIB:"PocoFoundationd.lib"',
				'/NODEFAULTLIB:"atlthunk.lib"',
			}

			defines {
				"WIN32",
				"_CONSOLE",
				"POCO_STATIC",
				"CAIRO_WIN32_STATIC_BUILD",
				"DISABLE_SOME_FLOATING_POINT",
			}

		configuration {'vs*' , "Debug"}
			defines {
				"_DEBUG",
			}

			links {
				-- RAM
				'BulletCollision_vs2010_d',
				'BulletDynamics_vs2010_d',
				'BulletSoftBody_vs2010_d',
				'LinearMath_vs2010_d',

				-- oF
				'cairo-static',
				'pixman-1',
				'fmodexL_vc',
				'fmodex_vc',
				'FreeImage',
				'libfreetype',
				'glew32s',
				'glu32',
				'glut32',
				'openframeworksLibDebug',
				'PocoFoundationmdd',
				'PocoNetmdd',
				'PocoUtilmdd',
				'PocoXMLmdd',
				'qtmlClient',
				'QTSClient',
				'Rave',
				'dsound',
				'rtAudioD',
				'tess2',
				'videoInput',
				'msimg32',
			}

		configuration {'vs*' , "Release"}
			defines {
				"NODEBUG",
			}

			links {
				-- RAM
				'LinearMath_vs2010',
				'BulletCollision_vs2010',
				'BulletDynamics_vs2010',
				'BulletSoftBody_vs2010',

				-- oF
				'cairo-static',
				'pixman-1',
				'fmodexL_vc',
				'fmodex_vc',
				'FreeImage',
				'libfreetype',
				'glew32s',
				'glu32',
				'glut32',
				'openframeworksLib',
				'PocoFoundationmd',
				'PocoNetmd',
				'PocoUtilmd',
				'PocoXMLmd',
				'qtmlClient',
				'QTSClient',
				'Rave',
				'dsound',
				'rtAudio',
				'tess2',
				'videoInput',
				'msimg32',
			}

		configuration {}
