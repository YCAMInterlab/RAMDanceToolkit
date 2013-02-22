solution "RAMDanceToolkit"
	language "C++"

	configuration "Debug"
		targetname( "ramDebug" )

	configuration "Release"
		targetname( "ram" )

	configurations { "Debug", "Release" }
	platforms { "x32" }

	local dirname = _ACTION
	location ( dirname )

	project "RAMDanceToolkit"
		kind "StaticLib"
		
		targetdir( '../lib' )

	   	includedirs {
	   		-- RAM
	   		"../ram/",
	   		"../ram/**",

	   		-- RAM addons
	   		'../../addons/ofxBt/src/',
	   		'../../addons/ofxBt/src/**',
	   		'../../addons/ofxBt/libs/',
	   		'../../addons/ofxBt/libs/**',
	   		'../../addons/ofxInteractivePrimitives/src/',
	   		'../../addons/ofxInteractivePrimitives/src/**',
	   		'../../addons/ofxUI/**',
	   		'../../addons/ofxUITabbedCanvas/',

	   		-- oF
	   		'../../../libs/**/include',
	   		'../../../libs/cairo/include/**',
	   		'../../../libs/openFrameworks',
	   		'../../../libs/openFrameworks/**',

	   		-- oF addons
	   		'../../../addons/ofxOsc/**',
	   		'../../../addons/ofxXMLSettings/**',
	   	}

	   	files {
	   		'../../libs/openFrameworks/*.h',
	   		'../../libs/openFrameworks/**/*.h',
	   		'../ram/**.h',
	   		'../ram/**.cpp',
	   		'../../addons/ofxBt/src/*.cpp',
	   		'../../addons/ofxInteractivePrimitives/src/**.cpp',
	   		'../../addons/ofxUITabbedCanvas/*.cpp',
	   		'../../../addons/ofxOsc/**/*.cpp',
	   		'../../../addons/ofxXmlSettings/**/*.cpp',
	   	}

	   	configuration { 'vs*' }
	   		defines {
				"WIN32",
				"POCO_STATIC",
				"CAIRO_WIN32_STATIC_BUILD",
				"DISABLE_SOME_FLOATING_POINT",
			}

	   	configuration { 'vs*', "Debug" }
			defines {
				"_DEBUG",
			}

			linkoptions {
				"/MTd"
			}

	   	configuration { 'vs*', "Release" }
	   		defines {
				"NODEBUG",
			}
