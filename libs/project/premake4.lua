solution "RAMDanceToolkit"
	language "C++"

	configurations { "Debug", "Release" }
	platforms { "x32" }

	project "RAMDanceToolkit"
		kind "StaticLib"

		targetname( "ram" )

	   	location ( os.get() .. "/" .. _ACTION )

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

	   	-- excludes { "**/Win32Specific/**" }
	   	-- libdirs { os.findlib("X11") }
	   	-- links { "Cocoa.framework" }
	   	-- libdirs { "libs", "../mylibs" }

	   	files {
	   		"../ram/**.h",
	   		"../ram/**.cpp",
	   		'../../addons/ofxBt/src/**.cpp',
	   		'../../addons/ofxInteractivePrimitives/src/**.cpp',
	   		'../../addons/ofxInteractivePrimitives/ofxUITabbedCanvas/**.cpp',
	   	}
