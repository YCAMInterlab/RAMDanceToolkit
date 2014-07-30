-- :usage
-- $ premake4 --project-name=MyProject xcode3
-- $ premake4 --project-name=MyProject vs2010

newoption {
   trigger = "project-name",
   value = "empty-example",
   description = "name of new project",
}

local project_name = _OPTIONS['project-name']
if not project_name then
	project_name = 'empty-example'
end

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
	void drawActor(const ramActor &actor);
	void drawRigid(const ramRigidBody &rigid);
	void onActorSetup(const ramActor &actor);
	void onActorExit(const ramActor &actor);
	void onRigidSetup(const ramRigidBody &rigid);
	void onRigidExit(const ramRigidBody &rigid);
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
void testApp::drawActor(const ramActor &actor)
{
}

//--------------------------------------------------------------
void testApp::drawRigid(const ramRigidBody &rigid)
{
}


#pragma mark - ram Events
//--------------------------------------------------------------
void testApp::onActorSetup(const ramActor &actor)
{
}

//--------------------------------------------------------------
void testApp::onActorExit(const ramActor &actor)
{
}

//--------------------------------------------------------------
void testApp::onRigidSetup(const ramRigidBody &rigid)
{
}

//--------------------------------------------------------------
void testApp::onRigidExit(const ramRigidBody &rigid)
{
}


#pragma mark - of Events
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

local gitkeep = [[

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

		debugdir ( project_name .. '/bin' )

		-- empty sorcecode if directory not exsits
		if not os.isdir(project_name) then
			create_file(project_name .. '/src/main.cpp', main_cpp)
			create_file(project_name .. '/src/testApp.h', test_app_h)
			create_file(project_name .. '/src/testApp.cpp', test_app_cpp)
			create_file(project_name .. '/bin/data/.gitkeep', gitkeep)
		end

		includedirs {
			-- src
			project_name .. '/src/**',

			-- RAM
			"../libs/ram/",
			"../libs/ram/**",

			-- RAM addons
			'../addons/ofxBt/src/',
			'../addons/ofxBt/src/**',
			'../addons/ofxBt/libs/',
			'../addons/ofxBt/libs/**',

			'../addons/ofxCv/src/',
			'../addons/ofxCv/src/**',
			'../addons/ofxCv/libs/',
			'../addons/ofxCv/libs/**',

			'../addons/ofxOpenCv/src/',
			'../addons/ofxOpenCv/src/**',
			'../addons/ofxOpenCv/libs/',
			'../addons/ofxOpenCv/libs/**',

			'../addons/ofxNodeArray/src/',
			'../addons/ofxNodeArray/src/**',

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
			'../../addons/ofxOpenCv/**',
		}

		files {
			project_name .. '/src/**.h',
			project_name .. '/src/**.cpp',

			'../../libs/openFrameworks/*.h',
			'../../libs/openFrameworks/**/*.h',

			'../libs/ram/*.h',
			'../libs/ram/**/*.h',
			'../libs/ram/**/*.cpp',

			'../addons/ofxBt/src/*.h',
			'../addons/ofxBt/src/*.cpp',

			'../addons/ofxCv/src/*.h',
			'../addons/ofxCv/src/*.cpp',
			'../addons/ofxCv/libs/**/*.h',
			'../addons/ofxCv/libs/**/*.cpp',

			'../addons/ofxNodeArray/src/*.h',
			'../addons/ofxNodeArray/src/*.cpp',

			'../addons/ofxInteractivePrimitives/src/*.h',
			'../addons/ofxInteractivePrimitives/src/*.cpp',

			'../addons/ofxUI/src/**.h',

			'../addons/ofxUITabbedCanvas/*.h',
			'../addons/ofxUITabbedCanvas/*.cpp',

			'../../addons/ofxOsc/**/*.h',
			'../../addons/ofxOsc/**/*.cpp',

			'../../addons/ofxXmlSettings/**/*.h',
			'../../addons/ofxXmlSettings/**/*.cpp',

			'../../addons/ofxOpenCv/src/*.h',
			'../../addons/ofxOpenCv/src/*.cpp',
			'../../addons/ofxOpenCv/libs/**/*.h',
			'../../addons/ofxOpenCv/libs/**/*.cpp',

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
				'../addons/ofxBt/libs/bullet/lib/vs/',
				'../../addons/ofxOpenCv/libs/opencv/lib/vs/',

				-- oF
				'../../libs/cairo/lib/vs',
				'../../libs/fmodex/lib/vs',
				'../../libs/FreeImage/lib/vs',
				'../../libs/freetype/lib/vs',
				'../../libs/glew/lib/vs',
				'../../libs/glfw/lib/vs',
				'../../libs/glu/lib/vs',
				'../../libs/glut/lib/vs',
				'../../libs/openFrameworksCompiled/lib/vs',
				'../../libs/openssl/lib/vs',
				'../../libs/poco/lib/vs',
				'../../libs/quicktime/lib/vs',
				'../../libs/rtAudio/lib/vs',
				'../../libs/tess2/lib/vs',
				'../../libs/videoInput/lib/vs',
			}

			buildoptions {
				'/MP'
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

			postbuildcommands { 'mkdir $(ProjectDir)bin\\data' }
			postbuildcommands { 'xcopy /e /i /y "$(ProjectDir)..\\..\\..\\export\\vs\\*.dll" "$(ProjectDir)bin"' }


		configuration {'vs*' , "Debug"}
			defines {
				"_DEBUG",
			}

			linkoptions {
				"/DEBUG"
			}

			links {
				-- RAM
				'BulletCollision_Debug',
				'BulletDynamics_Debug',
				'BulletSoftBody_Debug',
				'LinearMath_Debug',

				-- oF
				'cairo-static',
				'pixman-1',
				'fmodexL_vc',
				'fmodex_vc',
				'FreeImage',
				'libfreetype',
				'glew32s',
				'glfw3',
				'glu32',
				'glut32',
				'openframeworksLib_debug',
				'libeay32MD',
				'ssleay32MD',
				'PocoCryptomdd',
				'PocoFoundationmdd',
				'PocoNetmdd',
				'PocoNetSSLmdd',
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
				'crypt32',

				-- oF addons
				'opencv_calib3d231d',
				'opencv_contrib231d',
				'opencv_core231d',
				'opencv_features2d231d',
				'opencv_flann231d',
				'opencv_gpu231d',
				'opencv_haartraining_engined',
				'opencv_highgui231d',
				'opencv_imgproc231d',
				'opencv_legacy231d',
				'opencv_ml231d',
				'opencv_objdetect231d',
				'opencv_video231d',
				'zlibd',
			}

		configuration {'vs*' , "Release"}
			defines {
				"NODEBUG",
			}

			links {
				-- RAM
				'LinearMath',
				'BulletCollision',
				'BulletDynamics',
				'BulletSoftBody',

				-- oF
				'cairo-static',
				'pixman-1',
				'fmodexL_vc',
				'fmodex_vc',
				'FreeImage',
				'libfreetype',
				'glew32s',
				'glfw3',
				'glu32',
				'glut32',
				'openframeworksLib',
				'libeay32MD',
				'ssleay32MD',
				'PocoCryptomd',
				'PocoFoundationmd',
				'PocoNetmd',
				'PocoNetSSLmd',
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
				'crypt32',
				
				-- oF addons
				'opencv_calib3d231',
				'opencv_contrib231',
				'opencv_core231',
				'opencv_features2d231',
				'opencv_flann231',
				'opencv_gpu231',
				'opencv_haartraining_engine',
				'opencv_highgui231',
				'opencv_imgproc231',
				'opencv_legacy231',
				'opencv_ml231',
				'opencv_objdetect231',
				'opencv_video231',
				'zlib',
			}

		configuration {}